# lru2q_cache — design notes and open work

Companion notes for `lru2q_cache.hpp`. The header carries the per-construct comments; this file
carries what does not fit next to a line of code: why things are the way they are, which
alternatives were considered and rejected, what looks broken but is not, and what is still open.

Read the class-level comment in the header first — it defines the three regions and the
`DerivedT` contract. Everything below assumes it.

---

## 1. Mental model

Two counters per element, easy to confuse, completely different jobs:

| field  | meaning | protects |
|--------|---------|----------|
| `refs` | how many `cached_value` handles are alive | the element from being *disposed* |
| `pin_` | "somebody is changing this element's region right now" | `state` and the queue membership |

The whole region state machine lives in **`release()`**, not in `acquire()`. An element changes
region only when its last handle dies. Consequences worth internalising:

* "Hotness" is measured in releases, not in accesses. An element that always has a live handle is
  never promoted; when it is finally released it enters the `in` region as a newcomer.
* The read path touches no queue and takes no queue mutex. A hit on an `in` region element costs
  one atomic increment, one decrement, one CAS and one store. This is the main reason the design
  is worth its complexity — do not move region bookkeeping into `acquire()`.
* Holding a handle does **not** take the element out of its queue. It can still be picked for
  eviction; the `1 == add_ref_fetch` check in `remove_from_cache()` refuses the erase, parks the
  element in the `removing` state, and the holder's final `release()` retries.

## 2. Relation to the published 2Q

Structurally identical to Johnson & Shasha: `in` = A1in (correlated-reference filter, re-access
does nothing on purpose), `out` = A1out (probation, re-access promotes), `lru` = Am.

The one essential difference: **`out` is resident, not a ghost queue.** It stores values, not just
keys, so a hit there costs no `do_create_value()`. That is a deliberate trade — memory for avoided
reloads — and it is right when producing a value is expensive (a page read, a decompression). It
also means `in_quota = 0` gives the classic 2Q *admission policy* but never its memory profile.

## 3. Invariants

`check_structure()` encodes all of them; treat it as the executable spec. In prose:

* `queue_` holds two regions in one list: `[begin, next_out_it_)` is `out`, `[next_out_it_, end)`
  is `in`. `next_out_it_` is only positioned once the `in` region has filled up, so there are
  **three legal shapes**, and any code touching the border must handle all three:
  * `in_quota == 0` — border is `end()`, everything in `queue_` is `out`, `in_size_ == 0`;
  * `in_size_ < in_quota` — border is `end()`, everything is `in`, `out_size_ == 0`;
  * `in_size_ == in_quota > 0` — steady state, the border splits the two regions.
* Region counters match actual list contents: `out_size_`, `in_size_`, `lru_size_`.
* An element lives in at most one queue and, if queued, is reachable through its partition.
* An element outside every queue is either `not_in_queue` (created, never released) or `removing`
  (eviction lost the race), and in both cases `refs > 0`.
* `fallback_queue_` elements were erased from their partition first, so they are unreachable
  through one; its counter matches its contents.

## 4. Concurrency contract

**Lock hierarchy** is documented at the mutex declarations in the header. Summary: pin → dcl mutex
→ (partition `set_mutex_` | `fallback_queue_mtx_` | `queue_mtx_` | `lru_queue_mtx_`), and the last
group is never nested into itself.

**Memory ordering:**

* `pin_` is a real lock: acquire on the CAS, release on the store. This is what publishes `state`
  *and the value itself* between fibers, which matters because `cached_value::value()` hands out a
  mutable reference and `do_dispose_value()` may run on another fiber.
* `refs` uses acquire on increment, release on decrement. Needed because `release()` has exits that
  take no pin at all (`r > 1`, or a lost pin race) — there the counter is the only edge between a
  holder that mutated the value and whoever eventually disposes it.
* `state` is a plain enum on purpose. Every read and write happens under the element's pin, so it
  needs no atomicity of its own — only the pin's ordering, which it now has. Do not "fix" it into
  `std::atomic<queue_state>`; fix the pin instead if the ordering ever regresses.

## 5. Decisions already taken — do not re-litigate without a new reason

| Decision | Why |
|---|---|
| `release()` owns the reference decrement | Callers (`cached_value`) must not pre-decrement. Doing both underflows the counter and the element silently never reaches a queue. This was a real bug. |
| `dispose()` is `protected` and called from `DerivedT`'s destructor | It calls back into `DerivedT::do_dispose_value`; running it from `~lru2q_cache` would touch an already destroyed derived object. |
| `dispose()` is `noexcept` and swallows disposal failures | It runs from a destructor. Propagating terminates the process, and the backpressure argument does not apply — the whole cache is going away. |
| `acquire0()` *does* propagate a disposal failure | Opposite of the above by design: creating a value claims a scarce resource, so refusing to create while something is still unreclaimed is deliberate backpressure against exhaustion / the OOM killer. |
| `get_lru_quota() > 0` is required | With no `lru` region a re-accessed `out` element has nowhere to be promoted. The old `case_out_queue_no_lru` branch that recycled it into `in` was removed: it carried a deliberate unpinned write to `state` and degenerated (one second chance per element, then never again). |
| Buckets are configured explicitly, never grown | Owner's decision: sizes of the regions, partition count and bucket count are configured together at startup. `boost::intrusive::unordered_set` never rehashes on its own, so this is a real obligation, see §7. |
| The partition lock is **not** held across `do_create_value()` | `insert_check`/`insert_commit` would let the precomputed hash be reused on insert, but only by holding the exclusive partition lock across a user callback that may do I/O. Rejected. The dcl mutex, not the partition lock, is what prevents duplicate creation. |
| `store_hash<true>` not adopted | Saves a hash on insert/erase (cold paths) and speeds up collision chains. Worthless for cheap keys with a sane load factor, and costs a `size_t` per element. Reconsider only if keys become expensive to compare (strings, composite keys). |

## 6. Looks like a bug, is not

Everything here has been checked; leave it alone or read the linked reasoning first.

* **`in_size_` never decreases.** It is the size of the `in` region, which simply never shrinks
  once full — an element leaves `in` exactly when a new one arrives.
* **`lru_size_` may exceed `lru_quota`.** `case_out_queue()` does that on purpose when every `lru`
  element is busy: exceeding the quota beats leaving the promoted element pinned and outside all
  queues. The next promotion trims it back.
* **The `r == 0` branch in `release()` looks dead.** It is a guard; no reachable interleaving is
  known to produce it (the `remove_from_cache` catch-up path re-enters with at least 2). Kept
  deliberately, commented as such.
* **`hash_partition::erase()` takes no lock.** Its only caller is `dispose()`, which by contract
  runs when no other fiber can reach the cache.
* **A spin under `queue_mtx_`** in `put_in_queue()`. Safe only because the single element that spin
  can target has `state == in_queue`, whose `release()` branch takes no lock at all. The reasoning
  is written at the call site — if the state machine changes, re-verify it.
* **Exactly one weak CAS.** `try_acquire_pin_weak()` exists only for that spin. Every one-shot
  check must use the strong `try_acquire_pin()`: treating a spurious failure as "busy" silently
  drops an element out of the state machine.
* **`filled_` is never cleared.** It is a latch meaning "reached capacity at least once", not
  "full right now". Exposed through `is_filled()`.

## 7. Open work, roughly in priority order

1. **Bucket sizing guidance.** Since buckets never grow, an undersized `buckets_per_partition`
   turns `try_get()` from O(1) into a linear chain walk *on the hot path, under a shared lock*. The
   default of 17 is a placeholder suitable only for tiny caches. Rule of thumb: total buckets
   should be around `(in_quota + out_quota + lru_quota) * 1.3`, split across partitions, and the
   per-partition count kept **odd (prime is best)** — `get_partition()` selects on the low bits of
   the hash, so a power-of-two bucket count would collide with that choice and funnel each
   partition into a few buckets. Consider a `static` helper that derives the constructor arguments
   from the quotas so callers stop guessing.
2. **Queue sharding — the scalability ceiling.** Partitioning shards the hash set only. `queue_mtx_`
   and `lru_queue_mtx_` are cache-wide: every insertion, every promotion and every `lru` touch
   serialises on them, so concurrency will not scale past a handful of fibers no matter how many
   partitions exist. Options, increasing radicalism: (a) give each partition its own in/out/lru
   with a share of the quota; (b) buffer hits in per-thread ring buffers and reorganise the queues
   in batches under a try-lock (the Caffeine approach); (c) replace exact LRU with CLOCK, so a
   "touch" is one atomic bit write and no list surgery.
3. **Multi-fiber stress test.** Deferred until it can be written on the project's own thread/fiber
   infrastructure. All existing tests are single threaded, while every hard part of this component
   is concurrent. Shape: N fibers hammering a small key space with a mixed read/hold/release
   pattern, quotas small enough to force constant eviction, plus periodic `check_structure()` at a
   quiescent point. Expect this to find more than all current tests combined.
4. **Distinguish backpressure from a load failure.** `acquire0()` currently lets the original
   exception from `do_dispose_value()` escape, so a caller cannot tell "your key failed to load"
   from "the cache could not reclaim someone else's element". Wrap it at that boundary.
5. **Cache-line layout.** `in_size_`, `out_size_`, `lru_size_`, `queue_mtx_` and `next_out_it_` are
   hammered by every insertion and share lines; `refs`/`pin_` share a line with `value`, so
   refcount traffic invalidates the data line for readers. The header already has a placeholder
   comment about this.
6. **API polish.** `get_product()` discards the `lru2q_provenance` that `acquire()` computed, and
   `acquire()` is now `protected`, so a caller has no way to learn it. `product` is a `protected`
   typedef returned by a public function, so callers can only spell it `auto`.
7. **Striped locks instead of `keyed_refcount_pool`** for the dcl mutexes, *if* profiling ever shows
   the miss path matters. A fixed array indexed by the already-computed hash gives the same
   guarantee (same key → same mutex) with no lookup, no refcounting and no allocation; the cost is
   that two unrelated keys occasionally share a mutex, which is harmless here.

## 8. Testing status

`projects/vs/db-test/db-test.cpp` holds eight single-threaded cases built on the project's
`register_test` / `AUTOTEST` convention:

hit/miss, provenance, residency after release, a held element surviving pressure, eviction on quota
overflow, promotion protecting an element, disposal on destruction, and invariants under pressure
across four quota configurations (including the degenerate `in_quota = 0` and a never-filled `in`
region), calling `check_structure()` after every operation.

`instrumented_lru2q_cache` there keeps its stats in an object owned outside the cache, because
disposal of the remaining residents happens during destruction and the counts must still be
readable afterwards.

Gaps: nothing concurrent (see §7.3); no test drives `fallback_queue_` — that needs a
`do_dispose_value` that fails on demand, and it is worth adding since that path carries the
subtlest invariant in the file (§5, `acquire0`).

## 9. Notes for whoever picks this up

* This code attracts subtle bugs. The review that produced these notes found roughly eight real
  ones — a double decrement, uninitialised counters and iterator, a spin deadlock in the
  `in_quota == 0` path, an element lost when no `lru` victim was available, `dispose()` throwing
  out of a destructor, weak CAS in one-shot checks, and two memory-ordering gaps. Assume the next
  change has a similar bug in it until `check_structure()` and a concurrent test say otherwise.
* Every invariant that is not checked by `check_structure()` is an invariant that will be broken by
  a future refactor. When adding a rule, prefer extending the checker over adding a comment.
* The `queue_state` switch in `release()` deliberately has **no `default:`**, so that adding a state
  makes the compiler point at it. Do not add one back; `release()` is `noexcept` and a throwing
  default would not work anyway.
