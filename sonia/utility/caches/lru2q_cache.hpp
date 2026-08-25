//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <atomic>

#include <boost/intrusive/unordered_set.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/options.hpp>
#include <boost/intrusive/parent_from_member.hpp>

#include <boost/call_traits.hpp>

#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/concurrency/rw_fiber_mutex.hpp"

#include "bucket_set.hpp"
#include "keyed_refcount_pool.hpp"

namespace sonia::lru2q_detail {

enum class queue_state : uint8_t
{
    not_in_queue = 0,
    in_queue,
    out_queue,
    lru_queue,
    removing
};

enum class lru2q_provenance
{
    existing = 1,
    created
};

template <typename ElementT, typename TraitsT>
struct cache_elem_hasher
{
    using key_type = typename TraitsT::key_type;
    using key_param_type = typename boost::call_traits<key_type>::param_type;
    using hash_type = typename TraitsT::hash_type;

    inline size_t operator()(key_param_type s) const noexcept { return hash_type{}(s); }
    inline size_t operator()(ElementT const& d) const noexcept { return hash_type{}(key(d)); }
};

template <typename ElementT, typename TraitsT>
struct cache_elem_comparer
{
    using key_type = typename TraitsT::key_type;
    using key_param_type = typename boost::call_traits<key_type>::param_type;

    inline bool operator()(key_param_type k, ElementT const& s) const noexcept { return k == key(s); }
    inline bool operator()(ElementT const& s, key_param_type k) const noexcept { return key(s) == k; }
    inline bool operator()(ElementT const& l, ElementT const& r) const noexcept { return key(l) == key(r); }
};

template <typename CacheT>
class cached_value
{
    using KeyT = typename CacheT::key_type;
    using ValueT = typename CacheT::value_type;
    using element_type = typename CacheT::element_type;
    using traits_type = typename CacheT::traits_type;

public:
    inline cached_value(CacheT* cache, element_type* element) noexcept
        : cache_(cache), element_(element)
    {}

    inline cached_value(const cached_value& rhs)
        : cache_(rhs.cache_), element_(rhs.element_)
    {
        if (element_) {
            add_ref_fetch(element_);
        }
    }

    inline cached_value(cached_value&& rhs) noexcept
        : cache_(rhs.cache_), element_(rhs.element_)
    {
        rhs.element_ = nullptr;
    }

    inline cached_value& operator=(const cached_value& rhs) noexcept
    {
        if (element_ != rhs.element_) {
            if (element_) {
                cache_->release(element_); // release() owns the decrement
            }
            cache_ = rhs.cache_;
            element_ = rhs.element_;
            if (element_) {
                add_ref_fetch(element_);
            }
        }
        return *this;
    }

    inline cached_value& operator=(cached_value&& rhs) noexcept
    {
        if (element_ != rhs.element_) {
            if (element_) {
                cache_->release(element_); // release() owns the decrement
            }
            cache_ = rhs.cache_;
            element_ = rhs.element_;
            rhs.element_ = nullptr;
        }
        return *this;
    }

    inline ~cached_value()
    {
        if (element_) {
            cache_->release(element_); // release() owns the decrement
        }
    }

    // A moved from handle holds no element. The accessors below do not check for it, so test with
    // empty()/operator bool before using one that could have been moved from.
    inline bool empty() const noexcept { return !element_; }
    inline explicit operator bool() const noexcept { return !!element_; }

    inline ValueT& value() const noexcept
    {
        BOOST_ASSERT(element_);
        return element_->value;
    }

    inline ValueT& operator*() const noexcept
    {
        BOOST_ASSERT(element_);
        return element_->value;
    }

    inline ValueT* operator->() const noexcept
    {
        BOOST_ASSERT(element_);
        return &element_->value;
    }

    inline KeyT const& key() const noexcept
    {
        BOOST_ASSERT(element_);
        return element_->key;
    }

private:
    CacheT* cache_;
    element_type* element_;
};

template <typename TraitsT>
class hash_partition
{
    hash_partition(hash_partition const&) = delete;
    hash_partition& operator=(hash_partition const&) = delete;

    using key_type = typename TraitsT::key_type;
    using value_type = typename TraitsT::value_type;
    using key_param_type = boost::call_traits<key_type>::param_type;

public:
    using set_hook_type = boost::intrusive::unordered_set_member_hook<>;
    using list_hook_type = boost::intrusive::list_member_hook<boost::intrusive::link_mode<boost::intrusive::normal_link>>;

    struct element
    {
        set_hook_type set_hook_;
        list_hook_type queue_hook_;
        key_type key;
        value_type value;
        std::atomic<size_t> refs = 1;
        std::atomic<size_t> pin_ = 0;
        queue_state state = queue_state::not_in_queue;
    };

    inline friend key_type const& key(element const& element) noexcept
    {
        return element.key;
    }

    // Acquire on the way in, release on the way out - the usual reference counting pair. It matters
    // because release() has exits that take no pin at all (r > 1, or a lost try_acquire_pin race): a
    // holder may mutate the value through cached_value and then leave through one of them, and the
    // only thing linking it to whoever ends up calling do_dispose_value is this counter. Under
    // relaxed ordering the two RMWs would form no synchronizes-with edge and those mutations could
    // be invisible to the disposer on a weakly ordered platform.
    inline friend auto add_ref_fetch(element* element) noexcept
    {
        return element->refs.fetch_add(1, std::memory_order_acquire) + 1;
    }

    inline friend auto fetch_release(element* element) noexcept
    {
        return element->refs.fetch_sub(1, std::memory_order_release);
    }

    // The pin marks an element whose region state is being changed right now; it is not the
    // reference count (that one is refs, and a non zero refs means the element is in no queue).
    //
    // The pin is a real lock, not just a flag: everything it protects (element::state, and the
    // value itself, which a holder may mutate through cached_value) is passed between fibers by
    // the acquire/release pair below. Relaxed ordering here would still give mutual exclusion but
    // no happens-before edge, so the next pin owner would have no guarantee of seeing the previous
    // one's writes on a weakly ordered platform.
    //
    // One shot attempt, the form to reach for by default: a strong CAS, because treating a spurious
    // failure as "busy" would silently drop the element out of the state machine (release()) or
    // skip an eviction candidate.
    inline friend bool try_acquire_pin(element* elem) noexcept
    {
        size_t exp = 0;
        return elem->pin_.compare_exchange_strong(exp, 1, std::memory_order_acquire, std::memory_order_relaxed);
    }

    // Weak variant, only for spin loops, where a spurious failure costs nothing but one more
    // iteration. Never use it for a single attempt.
    inline friend bool try_acquire_pin_weak(element *elem) noexcept
    {
        size_t exp = 0;
        return elem->pin_.compare_exchange_weak(exp, 1, std::memory_order_acquire, std::memory_order_relaxed);
    }

    inline friend void release_pin(element* elem) noexcept
    {
        BOOST_ASSERT(elem->pin_.load(std::memory_order_relaxed) == 1);
        elem->pin_.store(0, std::memory_order_release);
    }

    struct set_hook_functor_type
    {
        using hook_type = set_hook_type;
        using hook_ptr = hook_type*;
        using const_hook_ptr = const hook_type*;
        using value_type = element;
        using pointer = value_type*;
        using const_pointer = const value_type*;

        inline static hook_ptr to_hook_ptr(value_type& value) noexcept { return &value.set_hook_; }
        inline static const_hook_ptr to_hook_ptr(const value_type& value) noexcept { return &value.set_hook_; }
        inline static pointer to_value_ptr(hook_ptr n) noexcept { return boost::intrusive::get_parent_from_member<element>(n, &element::set_hook_); }
        inline static const_pointer to_value_ptr(const_hook_ptr n) noexcept { return boost::intrusive::get_parent_from_member<element>(n, &element::set_hook_); }
    };

    struct list_hook_functor_type
    {
        using hook_type = list_hook_type;
        using hook_ptr = hook_type*;
        using const_hook_ptr = const hook_type*;
        using value_type = element;
        using pointer = value_type*;
        using const_pointer = const value_type*;

        static hook_ptr to_hook_ptr(value_type& value) { return &value.queue_hook_; }
        static const_hook_ptr to_hook_ptr(const value_type& value) { return &value.queue_hook_; }
        static pointer to_value_ptr(hook_ptr n) { return boost::intrusive::get_parent_from_member<element>(n, &element::queue_hook_); }
        static const_pointer to_value_ptr(const_hook_ptr n) { return boost::intrusive::get_parent_from_member<element>(n, &element::queue_hook_); }
    };

    using cache_elem_hasher_type = cache_elem_hasher<element, TraitsT>;
    using cache_elem_comparer_type = cache_elem_comparer<element, TraitsT>;

    using set_t = boost::intrusive::unordered_set<
          element
        , boost::intrusive::function_hook<set_hook_functor_type>
        , boost::intrusive::hash<cache_elem_hasher_type>
        , boost::intrusive::equal<cache_elem_comparer_type>
    >;

    using bucket_set_type = bucket_set<set_t>;

    using queue_t = boost::intrusive::list<
          element
        , boost::intrusive::function_hook<list_hook_functor_type>
        , boost::intrusive::constant_time_size<false>
    >;

    explicit hash_partition(size_t bucket_count)
        : buckets_(bucket_count)
        , set_(typename set_t::bucket_traits(buckets_.buckets(), buckets_.size()))
    {}

    hash_partition(hash_partition&& rhs) noexcept
        : buckets_(std::move(rhs.buckets_))
        , set_(std::move(rhs.set_))
    {}

    bool try_get(key_param_type key, element** result)
    {
        shared_lock_guard guard(set_mutex_);
        auto it = set_.find(key, cache_elem_hasher_type{}, cache_elem_comparer_type{});
        if (it != set_.end()) {
            *result = &*it;
            add_ref_fetch(*result);
            return true;
        }
        return false;
    }

    void insert(element& e) noexcept
    {
        lock_guard guard(set_mutex_);
        auto rpair = set_.insert(e);
        BOOST_ASSERT(rpair.second);
    }

    // Deliberately unlocked, unlike every other method here: the only caller is lru2q_cache::dispose(),
    // which runs from the derived class destructor when no other fiber can reach the cache. Taking
    // set_mutex_ would be pointless there and would also have to be held across a user callback.
    template <typename EraseFtorT>
    void erase(EraseFtorT const& ef)
    {
        while (!set_.empty()) {
            element* elem = &*set_.begin();
            set_.erase(*elem);
            try {
                ef(elem);
            } catch (...) {
                set_.insert(*elem);
                throw;
            }
        }
    }

    template <typename CheckFtorT>
    bool lock_check_erase(element& e, CheckFtorT const& ftor)
    {
        lock_guard guard(set_mutex_);
        if (ftor(e)) {
            BOOST_VERIFY(1 == set_.erase(e));
            return true;
        }
        return false;
    }

private:
    fibers::rw_mutex set_mutex_;
    bucket_set_type buckets_;
    set_t set_;
};

}

namespace sonia {

// An extension of the 2Q replacement algorithm (Johnson & Shasha) with three regions:
//
//   "in"  - FIFO for newcomers, sized by DerivedT::get_in_quota(). Re-accessing an element
//           while it is here changes nothing: this is the correlated reference filter, so a
//           burst of requests for the same key does not make it long lived.
//   "out" - elements pushed out of the "in" queue, sized by DerivedT::get_out_quota().
//           Here the classic 2Q rule applies: a repeated access promotes the element to "lru".
//   "lru" - the long lived LRU region, sized by DerivedT::get_lru_quota(). An element evicted
//           from here leaves the cache.
//
// The essential difference from the classic algorithm is that the "out" region is RESIDENT and
// not a ghost queue: it keeps values, not just keys, so a hit there costs no do_create_value()
// call. Setting in_quota to 0 yields the classic 2Q admission policy (promotion on the second
// reference), but not its memory profile - "out" still holds values.
//
// Notes on the behaviour that follow from the implementation:
//   * the whole state machine lives in release(), not in acquire(): an element changes its
//     region only when its last cached_value dies. Hence "hotness" is counted in releases, not
//     in acquires - an element that always has a live handle never gets promoted, and once it
//     is finally released it enters the "in" queue as a newcomer;
//   * a pinned element (refs != 0) belongs to no queue at all and therefore can not be evicted;
//   * the resident capacity is about in_quota + out_quota + lru_quota, the eviction condition in
//     put_in_queue() lets the "out" region borrow the yet unused part of the lru quota.
//
// DerivedT must provide: do_create_value(), do_dispose_value(), get_in_quota(), get_out_quota(),
// get_lru_quota(), and call dispose() from its destructor.
//
// do_create_value(key, element_type** ppelement, aux...) must store a fully constructed element
// whose bookkeeping fields are in their initial state: refs == 1 (the reference handed to the
// caller), pin_ == 0, state == not_in_queue and both intrusive hooks unlinked. Constructing the
// element normally gives that for free through the default member initializers, but an
// implementation that recycles memory from a pool has to reset those fields explicitly - reusing an
// element that still carries an old refs or state silently corrupts the region bookkeeping.
// Failing to produce a value must be reported by an exception, never by leaving *ppelement null.
//
// get_lru_quota() must be greater than 0: without the lru region a re-accessed "out" element has
// nowhere to be promoted to, and the cache degenerates into a plain FIFO. get_in_quota() may be 0.
template <typename DerivedT, typename TraitsT>
class lru2q_cache
{
    friend class lru2q_detail::cached_value<lru2q_cache>;
    DerivedT& derived() { return static_cast<DerivedT&>(*this); }
    
protected:
    using traits_type = TraitsT;
    using partition_type = lru2q_detail::hash_partition<traits_type>;
    using queue_t = typename partition_type::queue_t;

    using key_type = typename traits_type::key_type;
    using value_type = typename traits_type::value_type;
    using element_type = typename partition_type::element;
    using key_param_type = typename boost::call_traits<key_type>::param_type;

    using product = lru2q_detail::cached_value<lru2q_cache>;

public:
    lru2q_cache(
        size_t partition_count = 1,
        size_t buckets_per_partition = 17,
        size_t dcl_buckets = 17,
        size_t dcl_pool_reserve_size = 32);

    product get_product(key_type const& key)
    {
        element_type* element = nullptr;
        acquire(key, &element);
        return product(this, element);
    }

    // prototype for derived class to implement
    size_t get_lru_quota() const;
    size_t get_in_quota() const;
    size_t get_out_quota() const;

protected:
    // Must be called from the derived class destructor: it calls back into
    // DerivedT::do_dispose_value, so DerivedT must still be alive. Not public on purpose - calling
    // it while the cache is in use leaves every outstanding element dangling.
    // noexcept because of that contract: throwing out of a destructor terminates the process, so a
    // value that refuses to be disposed is abandoned here instead (see the definition).
    void dispose() noexcept;

    // Returns the element for the key, creating it through DerivedT::do_create_value() when it is
    // not cached yet, and reports where it came from. On return *ppelement is always a valid
    // element holding one reference for the caller - a failure to produce a value is reported by an
    // exception, never by a null. The reference must be given back with release().
    // Prefer get_product(), which ties that reference to a cached_value; this raw form is for
    // derived classes that need the provenance or the element itself.
    template <typename ... AuxParamsT>
    lru2q_detail::lru2q_provenance acquire(key_param_type key, element_type** ppelement, AuxParamsT&& ... aux_params);

    // Gives back one reference taken by acquire(). Callers must not decrement the counter
    // themselves - this method does it. Reaching zero references does not remove anything from the
    // cache, it only makes the element a candidate for eviction and advances its region state.
    // noexcept because ~cached_value() calls it and a destructor may not throw. Everything it can
    // run into - locking, do_dispose_value() failures - is either handled inside or, in the case of
    // a corrupted element state, unrecoverable and better turned into a loud terminate.
    void release(element_type* elem) noexcept;

    // Latch set the first time an element is pushed out of the cache, i.e. "the cache has reached
    // its capacity at least once". It is never cleared, so it does not mean "full right now" -
    // a derived class may expose it as a diagnostic property.
    bool is_filled() const noexcept { return filled_.load(std::memory_order_relaxed); }

    template <typename ... AuxParamsT>
    lru2q_detail::lru2q_provenance acquire0(key_param_type key, element_type** ppelement, AuxParamsT&& ... aux_params);
    
    inline partition_type& get_partition(key_param_type key) noexcept
    {
        size_t pidx = typename traits_type::hash_type{}(key) % partitions_.size();
        return partitions_[pidx];
    }

    element_type* put_in_queue(element_type*);
    element_type* remove_from_cache(element_type*);
    element_type* case_out_queue(element_type*);
    element_type* case_lru_queue(element_type*);

    std::vector<partition_type> partitions_;
    keyed_refcount_pool<key_type, fibers::mutex, typename traits_type::hash_type, typename traits_type::equal_type> dcl_mutexes_;
    size_t in_size_, out_size_;
    std::atomic<size_t> lru_size_;
    fibers::mutex fallback_queue_mtx_;
    std::atomic<size_t> fallback_queue_size_;
    queue_t fallback_queue_;

    // perhaps somewhere here will be a new cache line for frequently changing variables
    fibers::mutex queue_mtx_;
    queue_t queue_;
    typename queue_t::iterator next_out_it_;

    fibers::mutex lru_queue_mtx_;
    queue_t lru_queue_;

    std::atomic<bool> filled_;
};


template <typename DerivedT, typename TraitsT>
lru2q_cache<DerivedT, TraitsT>::lru2q_cache(size_t partition_count, size_t buckets_per_partition, size_t dcl_buckets, size_t dcl_pool_reserve_size)
    : dcl_mutexes_(dcl_buckets, dcl_pool_reserve_size)
    , in_size_{0}, out_size_{0}
    , lru_size_{0}
    , fallback_queue_size_{0}
    , filled_{false}
{
    partitions_.reserve(partition_count);
    while (partitions_.size() < partition_count) {
        partitions_.emplace_back(buckets_per_partition);
    }
    next_out_it_ = queue_.end(); // put_in_queue relies on end() to detect the 0 == in_quota case
}

template <typename DerivedT, typename TraitsT>
template <typename ... AuxParamsT>
lru2q_detail::lru2q_provenance lru2q_cache<DerivedT, TraitsT>::acquire(key_param_type key, element_type** ppelement, AuxParamsT&& ... aux_params)
{
    partition_type& partition = get_partition(key);
    if (partition.try_get(key, ppelement)) return lru2q_detail::lru2q_provenance::existing;
    return acquire0(key, ppelement, std::forward<AuxParamsT>(aux_params)...);
}

template <typename DerivedT, typename TraitsT>
template <typename ... AuxParamsT>
lru2q_detail::lru2q_provenance lru2q_cache<DerivedT, TraitsT>::acquire0(key_param_type key, element_type** ppelement, AuxParamsT&& ... aux_params)
{
    partition_type& partition = get_partition(key);
    auto * dcl_mutex = dcl_mutexes_.acquire(key);

    SCOPE_EXIT([dcl_mutex, this]{
        dcl_mutexes_.release(dcl_mutex);
    });
    {
        lock_guard dcl_guard(dcl_mutex->value); // it's expected that locked location can not be removed from cache (if allocated)
        if (partition.try_get(key, ppelement)) return lru2q_detail::lru2q_provenance::existing;

        // a cache miss takes place
        //
        // Nothing new is created while there are elements pending release: creating a value may
        // claim a scarce resource, and giving it out before the pending one is reclaimed is how a
        // process gets to exhaust that resource (or to meet the OOM killer). A repeated disposal
        // failure is therefore propagated to the caller as backpressure instead of being logged.
        //
        // A subtle invariant here: a drained element is no longer in its partition, so the dcl
        // mutex of ITS key is not held - only the one of the requested key. What keeps
        // do_dispose_value() of the old element from overlapping do_create_value() of a new
        // element with the same key is that fallback_queue_mtx_ is held ACROSS the disposal and
        // that fallback_queue_size_ is decremented only after the disposal succeeded, while
        // creation is gated on observing that counter at zero. Narrowing this lock (an obvious
        // looking optimization, since a user callback is called under a cache wide mutex) or
        // moving the decrement earlier reintroduces that race.
        for (;;) {
            if (0 == fallback_queue_size_.load()) {
                derived().do_create_value(key, ppelement, std::forward<AuxParamsT>(aux_params) ...);
                BOOST_ASSERT(*ppelement);
                partition.insert(**ppelement);
                return lru2q_detail::lru2q_provenance::created;
            }
            lock_guard guard(fallback_queue_mtx_);
            while (!fallback_queue_.empty()) {
                element_type * elem = &fallback_queue_.front();
                fallback_queue_.pop_front();
                try {
                    derived().do_dispose_value(elem); // can throw
                } catch (...) {
                    fallback_queue_.push_back(*elem);
                    throw;
                }
                --fallback_queue_size_;
            }
        }
    }
}

template <typename DerivedT, typename TraitsT>
void lru2q_cache<DerivedT, TraitsT>::release(element_type* elem) noexcept
{
    using namespace lru2q_detail;

    // This method owns the reference decrement: callers (cached_value in particular) must NOT
    // decrement before calling it, otherwise the counter underflows and the element never reaches
    // any queue. Note that dropping the last reference does not evict anything, it only unpins the
    // element and lets it take its place in the region state machine.
    while (elem)
    {
        size_t r = fetch_release(elem);

        if (r > 1) {
            return; // there are other instances
        }

        if (!r) {
            // Entered with a zero counter, i.e. an unbalanced release. Restore the count and bail
            // out instead of underflowing. No reachable interleaving is known to produce this: when
            // remove_from_cache() gives an element back ("somebody acquired it while I was erasing
            // it"), its check has added a reference, so this loop re-enters with at least 2 and
            // leaves through the r > 1 branch above. Kept as a guard, not as a working path.
            add_ref_fetch(elem);
            return;
        }

        // element doesn't change its queue position if requested during the time when a thread processing free_element for it
        if (!try_acquire_pin(elem)) {
            // processing at the moment, just skip
            return;
        }

        BOOST_ASSERT(elem->state <= queue_state::removing); // catches a corrupted state in debug builds

        // No default: on purpose. Every state below either returns or moves elem on, so the switch
        // is exhaustive, and leaving it that way makes the compiler point right here once a new
        // queue_state is added instead of quietly routing it into a catch all branch. A throwing
        // default would not work anyway - release() is noexcept because a destructor calls it.
        switch (elem->state)
        {
        case queue_state::not_in_queue: // just created, not in any queue, is going to be placed in "in" queue
            elem = put_in_queue(elem);
            break;

        case queue_state::removing:
            elem = remove_from_cache(elem);
            break;

        case queue_state::in_queue:
            release_pin(elem); // element has been handled, unpin
            return;

        case queue_state::out_queue:
            elem = case_out_queue(elem);
            break;

        case queue_state::lru_queue:
            elem = case_lru_queue(elem);
            break;
        }
    }
}

template <typename DerivedT, typename TraitsT>
lru2q_cache<DerivedT, TraitsT>::element_type* lru2q_cache<DerivedT, TraitsT>::put_in_queue(element_type* elem)
{
    using namespace lru2q_detail;

    size_t in_quota = derived().get_in_quota();
    size_t out_quota = derived().get_out_quota();
    size_t lru_quota = derived().get_lru_quota();

    // in_quota may be 0 (then nothing stays in the "in" region), but there must be somewhere to
    // promote a re-accessed "out" element to
    BOOST_ASSERT(lru_quota > 0);

    // queue_ holds both regions at once: [ front ... "out" ... next_out_it_ ... "in" ... back ].
    // next_out_it_ is the border, i.e. the oldest element still counted as "in", and it equals
    // queue_.end() while the "in" region has not filled up yet (in particular always when
    // in_quota is 0). in_size_ only grows: it is a fill up counter that saturates at in_quota,
    // after which the region keeps its size by moving the border instead.
    {
        unique_lock lock(queue_mtx_);
        queue_.push_back(*elem); // put in "in" queue

        if (in_size_ < in_quota) { // "in" queue has a free slot for the new element
            elem->state = queue_state::in_queue; // mark as placed in "in" queue
            release_pin(elem);
            ++in_size_;
            if (in_size_ == in_quota) { // it was the final slot
                next_out_it_ = queue_.begin(); // update reference on the front queue element
            }
            return nullptr;
        }

        if (next_out_it_ == queue_.end()) { // case 0 == in_quota: nothing ever stays in the "in" queue
            BOOST_ASSERT(0 == in_quota);
            // elem itself goes straight to the "out" queue and its pin is still ours,
            // so there is nothing to spin for here; next_out_it_ stays end()
            elem->state = queue_state::out_queue;
            release_pin(elem);
        } else {
            elem->state = queue_state::in_queue; // mark as placed in "in" queue
            release_pin(elem);

            // "in" queue is full
            //auto dlen = std::distance(next_out_it_, queue_.end());
            //BOOST_ASSERT((size_t)dlen == in_quota + 1);

            // *next_out_it_ is in "in" queue, its state was set before next_out_it_ could
            // reach it => the pin owner can only take the lock free "in queue" branch of
            // release(), so spinning under queue_mtx_ can not deadlock here
            auto & e = *next_out_it_;
            while (!try_acquire_pin_weak(&e)) {
                this_fiber::yield();
            }
            e.state = queue_state::out_queue;  // mark the front "in" queue element as an "out" queue element
            release_pin(&e);

            ++next_out_it_; // update reference on the front queue element of "in" queue
            BOOST_ASSERT(next_out_it_ != queue_.end());
        }

        ++out_size_;

        // This is the only place where elements leave the cache. The second condition lets the
        // "out" region borrow the part of the lru quota that the lru region has not claimed yet,
        // so the resident capacity is about in_quota + out_quota + lru_quota rather than a hard
        // limit per region.
        for (;;)
        {
            if (out_size_ <= out_quota || (out_size_ + lru_size_.load()) < (out_quota + lru_quota)) return nullptr; // use lru quata if lru_size allows

            elem = &queue_.front();

            if (!try_acquire_pin(elem)) {
                // processing at the moment, just skip
                return nullptr;
            }
            queue_.pop_front();
            --out_size_;

            lock.unlock();
            elem->state = queue_state::removing;
            elem = remove_from_cache(elem);
            if (elem) return elem;
            lock.lock();
        }
    }
}

template <typename DerivedT, typename TraitsT>
lru2q_cache<DerivedT, TraitsT>::element_type* lru2q_cache<DerivedT, TraitsT>::remove_from_cache(element_type* elem)
{
    filled_.store(true); // set flag

    partition_type & partition = get_partition(key(*elem));

    auto * dcl_mutex = dcl_mutexes_.acquire(key(*elem));

    SCOPE_EXIT([dcl_mutex, this]{
        dcl_mutexes_.release(dcl_mutex);
    });

    lock_guard dcl_guard(dcl_mutex->value);
    // The check runs under the partition write lock, so it races only against acquire() calls that
    // already found the element. Adding a reference and getting exactly 1 back means nobody else
    // holds it and the erase is safe; anything greater means somebody acquired it just now, and the
    // added reference is handed back to the caller through release()'s "r == 0" branch.
    if (partition.lock_check_erase(*elem, [](element_type &e) {
        return 1 == add_ref_fetch(&e);
    })) {

        try {
            derived().do_dispose_value(elem); // can throw
        } catch (...) {
            // The element is already out of the partition, so nothing can find it anymore.
            // Park it instead of failing this eviction: acquire0() retries the disposal on the
            // next cache miss. Pushing and incrementing the counter under the same mutex is what
            // makes "queue empty while the counter is not zero" unobservable to the drain loop -
            // see the invariant described there. The element stays pinned, which is harmless
            // since it belongs to no queue and to no partition by now.
            lock_guard guard(fallback_queue_mtx_);
            fallback_queue_.push_back(*elem);
            ++fallback_queue_size_;
        }
        return nullptr;
    }

    // add reference was called by try_erase
    // very rare case (catching up while 'out' queue releasing)
    release_pin(elem); // unpin
    return elem; // repeat disposing
}

template <typename DerivedT, typename TraitsT>
lru2q_cache<DerivedT, TraitsT>::element_type* lru2q_cache<DerivedT, TraitsT>::case_out_queue(element_type* elem)
{
    using namespace lru2q_detail;

    {
        lock_guard guard(queue_mtx_);
        queue_.erase(queue_.s_iterator_to(*elem));
        --out_size_;
    }

    element_type* elem2rm = nullptr;

    {
        lock_guard guard(lru_queue_mtx_);
        BOOST_ASSERT(derived().get_lru_quota() > 0); // a zero lru quota leaves promotion nowhere to go
        if (lru_size_.load() < derived().get_lru_quota()) {
            ++lru_size_;
            lru_queue_.push_back(*elem);
            elem->state = queue_state::lru_queue;
            release_pin(elem); // unpin
            return nullptr;
        }

        // try find elem to remove from the cache
        for (auto elem_it = lru_queue_.begin(); elem_it != lru_queue_.end(); ++elem_it)
        {
            if (!try_acquire_pin(&*elem_it)) {
                // processing at the moment, just postpone, will be replaced in lru_queue case.
                continue;
            }
            elem2rm = &*elem_it;
            lru_queue_.erase(elem_it);
            elem2rm->state = queue_state::removing;
            break;
        }

        if (!elem2rm) {
            // every lru element is being processed right now, so nothing was freed:
            // exceed the quota rather than leave elem pinned and out of every queue
            ++lru_size_;
        }

        lru_queue_.push_back(*elem);
        elem->state = queue_state::lru_queue;
        release_pin(elem); // unpin
    }

    return elem2rm ? remove_from_cache(elem2rm) : nullptr;
}

template <typename DerivedT, typename TraitsT>
lru2q_cache<DerivedT, TraitsT>::element_type* lru2q_cache<DerivedT, TraitsT>::case_lru_queue(element_type* elem)
{
    using namespace lru2q_detail;
    {
        lock_guard guard(lru_queue_mtx_);
        queue_state st = elem->state;
        BOOST_ASSERT(st == queue_state::lru_queue);
        
        lru_queue_.erase(lru_queue_.s_iterator_to(*elem));
        lru_queue_.push_back(*elem);
        release_pin(elem); // unpin
        return nullptr;
    }
}

template <typename DerivedT, typename TraitsT>
void lru2q_cache<DerivedT, TraitsT>::dispose() noexcept
{
    // no locking here: dispose() is expected to be called from the derived class
    // destructor, i.e. when no other fiber can touch the cache anymore

    // Unlike acquire0(), a disposal failure is swallowed rather than propagated. Propagating it
    // would leave a destructor through an exception and terminate the process, and the backpressure
    // that justifies rethrowing at runtime is meaningless here: the whole cache is going away, so
    // there is nothing left to protect from resource exhaustion. An element that refuses to be
    // disposed is abandoned, and the rest are still released.

    // elements that failed to be disposed earlier are already out of the partition
    // sets, so p.erase() below won't see them - finish them off first
    while (!fallback_queue_.empty()) {
        element_type* elem = &fallback_queue_.front();
        fallback_queue_.pop_front();
        --fallback_queue_size_;
        try {
            derived().do_dispose_value(elem); // can throw
        } catch (...) {
            // abandoned on purpose, see above
        }
    }

    for (partition_type& p : partitions_) {
        // the functor must not throw, otherwise hash_partition::erase() would propagate it
        p.erase([this](element_type* elem) noexcept {
            try {
                derived().do_dispose_value(elem);
            } catch (...) {
                // abandoned on purpose, see above
            }
        });
    }
    partitions_.clear();

    // the elements are gone; hooks are in normal_link mode, so clear() only
    // reinitializes the list headers and never dereferences the freed nodes
    queue_.clear();
    lru_queue_.clear();
    next_out_it_ = queue_.end();
    in_size_ = out_size_ = 0;
    lru_size_.store(0);
    filled_.store(false);
}

}
