//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <atomic>
#include <mutex>

#include <boost/assert.hpp>
#include <boost/integer_traits.hpp>
#include "sonia/concurrency.hpp"

namespace sonia::fibers {

class rw_mutex
{
    static constexpr size_t log_half_rng = (sizeof(intptr_t) * CHAR_BIT - 1) / 2;
    static constexpr intptr_t state_pos_value = (intptr_t)1 << log_half_rng; // intptr_t::max / state_pos_value = ~ wr and state_pos_value r locks simultaneously
    std::atomic<intptr_t> state_{0};

    using mutex_t = mutex;
    mutex_t mtx_;

    spin_mutex smtx_;

    condition_variable_any rvar_;

public:
    rw_mutex() = default;
    rw_mutex(rw_mutex const&) = delete;
    rw_mutex& operator= (rw_mutex const&) = delete;

    void lock_shared()
    {
        for (;;) {
            if (state_.fetch_sub(1, std::memory_order_acquire) <= 0) {
                return;
            }

            intptr_t stateval = state_.fetch_add(1, std::memory_order_relaxed);
            
            if (stateval < 0) continue;

            BOOST_ASSERT(stateval != 0);

            // another fiber can be waiting on rvar at this point
            if ((stateval + 1) > 0 && 0 == ((stateval + 1) % state_pos_value)) {
                std::lock_guard guard(smtx_);
                rvar_.notify_one();
            }

            mtx_.lock();
            mtx_.unlock();
        }
    }

    void unlock_shared()
    {
        intptr_t stateval = 1 + state_.fetch_add(1, std::memory_order_release);
        if (stateval > 0 && 0 == (stateval % state_pos_value)) {
            std::lock_guard guard(smtx_);
            rvar_.notify_one();
        }
    }

    void promote()
    {
        intptr_t stateval = state_.fetch_add(state_pos_value + 1, std::memory_order_acquire);
        BOOST_ASSERT(0 != stateval);
        if (-1 == stateval) {
            mtx_.lock();
            return;
        } else if (stateval < 0) { // there is another shared lock at the moment
            mtx_.lock();
            std::unique_lock rnext_lock(smtx_);
            rvar_.wait(smtx_, [this]() { return 0 == state_.load(std::memory_order_relaxed) % state_pos_value; });
            return;
        }
        // there is a waiting write lock
        // so release shared lock and acquire lock
        stateval = state_.fetch_sub(state_pos_value, std::memory_order_relaxed) - state_pos_value;
        if (stateval > 0 && 0 == (stateval % state_pos_value)) {
            std::lock_guard guard(smtx_);
            rvar_.notify_one();
        }
        lock();
    }

    void lock()
    {
        for (;;) {
            intptr_t stateval = state_.fetch_add(state_pos_value, std::memory_order_acquire);
            if (!stateval) {
                mtx_.lock();
                return;
            } else if (stateval < 0) { // there is a shared lock at the moment
                mtx_.lock();
                std::unique_lock rnext_lock(smtx_);
                rvar_.wait(smtx_, [this]() { return 0 == state_.load(std::memory_order_relaxed) % state_pos_value; });
                return;
            }

            stateval = state_.fetch_sub(state_pos_value, std::memory_order_relaxed);

            if (stateval == state_pos_value) continue; // no locks at all, try again right away

            mtx_.lock();
            mtx_.unlock();
        }
    }

    void unlock()
    {
        state_.fetch_sub(state_pos_value, std::memory_order_release);
        mtx_.unlock();
    }

    void demote()
    {
        state_.fetch_sub(state_pos_value + 1, std::memory_order_acq_rel);
        mtx_.unlock();
    }
};

}
