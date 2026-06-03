//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <limits>
#include <type_traits>

#include "sonia/shared_ptr.hpp"
#include "sonia/variant.hpp"
#include "sonia/span.hpp"
#include "sonia/small_vector.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/utility/invocation/invocation.hpp"

namespace sonia::invocation {

// event_emitter — a thread-safe multicast handler list.
//
// Two subscription modes:
//   subscribe(shared_ptr / callable*)  — returns size_t id, use unsubscribe(id) to remove.
//   subscribe(weak_ptr)                — returns void; the slot is reclaimed automatically
//                                        when the weak_ptr expires during emit(). Because no
//                                        id is issued, unsubscribe() cannot be mistakenly
//                                        called with a stale id that has been reused.
//
// Usage:
//   event_emitter emitter;
//   size_t id = emitter.subscribe(shared_handler);
//   emitter.subscribe(weak_handler);             // fire-and-forget, no id
//   emitter.emit(args);
//   emitter.unsubscribe(id);
//
// Notes:
//   - emit() snapshots the handler list before invoking, so subscribe/
//     unsubscribe may be safely called from within a handler.

class event_emitter
{
public:
    // handler_t covers only the two ownership modes that return an id.
    using handler_t = variant<
        shared_ptr<callable>,
        callable*
    >;

private:
    // Internal storage: handler_t alternatives + weak_ptr (auto-reclaimed) +
    // size_t as free-slot sentinel (next free index, or npos).
    using slot_t = variant<
        shared_ptr<callable>,
        callable*,
        weak_ptr<callable>,
        size_t        // free-slot sentinel: value = index of next free slot
    >;

    static constexpr size_t npos = (std::numeric_limits<size_t>::max)();

    small_vector<slot_t, 2> slots_;
    size_t free_head_ = npos;   // head of the free-slot linked list
    mutable fibers::mutex mtx_;

    size_t do_subscribe(slot_t slot)
    {
        lock_guard guard(mtx_);
        if (free_head_ == npos) {
            slots_.push_back(std::move(slot));
            return slots_.size() - 1;
        }
        size_t idx = free_head_;
        free_head_ = get<size_t>(slots_[idx]);
        slots_[idx] = std::move(slot);
        return idx;
    }

public:
    event_emitter() = default;
    event_emitter(event_emitter const&) = delete;
    event_emitter& operator=(event_emitter const&) = delete;

    // Register a shared_ptr or raw-pointer handler. Returns id for unsubscribe().
    size_t subscribe(handler_t h)
    {
        slot_t slot = visit([](auto&& arg) -> slot_t {
            return std::forward<decltype(arg)>(arg);
        }, h);
        return do_subscribe(std::move(slot));
    }

    // Helper: subscribe from a shared_ptr to any callable-derived type.
    template <std::derived_from<callable> T>
    size_t subscribe(shared_ptr<T> h)
    {
        return do_subscribe(slot_t{ static_pointer_cast<callable>(std::move(h)) });
    }

    // Register a weak_ptr handler. Returns void — slot is auto-reclaimed on expiry.
    // unsubscribe() must NOT be called for weak_ptr subscriptions.
    void subscribe(weak_ptr<callable> h)
    {
        do_subscribe(slot_t{ std::move(h) });
    }

    template <std::derived_from<callable> T>
    void subscribe(weak_ptr<T> h)
    {
        do_subscribe(slot_t{ static_pointer_cast<callable>(std::move(h)) });
    }

    // Remove a handler registered via the id-returning subscribe() overloads.
    void unsubscribe(size_t id)
    {
        lock_guard guard(mtx_);
        if (id >= slots_.size()) return;
        slots_[id] = slot_t{ free_head_ };
        free_head_ = id;
    }

    // Invoke all live handlers with the given arguments.
    // ErrorHandler:     optional void(size_t id, smart_blob const&)
    // ExceptionHandler: optional void(size_t id, std::exception const&)
    template <typename ErrorHandlerT = nullptr_t, typename ExceptionHandlerT = nullptr_t>
    void emit(span<const blob_result> args,
              ErrorHandlerT on_error = nullptr,
              ExceptionHandlerT on_exception = nullptr)
    {
        // Snapshot under the lock so handlers may call subscribe/unsubscribe.
        small_vector<std::pair<size_t, slot_t>, 8> snapshot;
        {
            lock_guard guard(mtx_);
            snapshot.reserve(slots_.size());
            for (size_t i = 0; i < slots_.size(); ++i) {
                if (!holds_alternative<size_t>(slots_[i])) {
                    snapshot.emplace_back(i, slots_[i]);
                }
            }
        }

        for (auto& [idx, slot] : snapshot) {
            try {
                smart_blob res = visit(match(
                    [&args](shared_ptr<callable> const& f) -> smart_blob {
                        return f->invoke(args);
                    },
                    [&args](callable* f) -> smart_blob {
                        return f->invoke(args);
                    },
                    [&](weak_ptr<callable> const& wf) -> smart_blob {
                        if (auto f = wf.lock()) {
                            return f->invoke(args);
                        }
                        // expired — reclaim the slot (safe: no id was ever issued)
                        lock_guard guard(mtx_);
                        if (idx < slots_.size() && holds_alternative<weak_ptr<callable>>(slots_[idx])) {
                            slots_[idx] = slot_t{ free_head_ };
                            free_head_ = idx;
                        }
                        return {};
                    },
                    [](size_t) -> smart_blob { return {}; }
                ), slot);

                if (res.is_error()) {
                    if constexpr (!std::is_same_v<ErrorHandlerT, nullptr_t>) {
                        on_error(idx, res);
                    }
                }
            } catch (std::exception const& ex) {
                if constexpr (!std::is_same_v<ExceptionHandlerT, nullptr_t>) {
                    on_exception(idx, ex);
                }
            }
        }
    }

    template <typename ErrorHandlerT = nullptr_t, typename ExceptionHandlerT = nullptr_t>
    void emit(std::initializer_list<blob_result> args,
        ErrorHandlerT on_error = nullptr,
        ExceptionHandlerT on_exception = nullptr)
    {
        emit(span<const blob_result>{ args.begin(), args.size() }, on_error, on_exception);
    }
};

} // namespace sonia::invocation
