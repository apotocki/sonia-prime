//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_RW_FIBER_MUTEX_EXT_HPP
#define SONIA_RW_FIBER_MUTEX_EXT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <atomic>
#include <mutex>

#include <boost/assert.hpp>
#include <boost/integer_traits.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/fiber/condition_variable.hpp>

namespace sonia {

class rw_fiber_mutex_ext {
    static const size_t log_half_rng = (sizeof(intptr_t) * CHAR_BIT - 1) / 2;
    static const intptr_t state_pos_value = (intptr_t)1 << log_half_rng; // intptr_t::max / state_pos_value = ~ wr and state_pos_value r locks simultaneously
    std::atomic<intptr_t> state_{0};

    typedef boost::fibers::mutex mutex_t;
    mutex_t mtx_;

    typedef boost::fibers::detail::spinlock spinlock_t;
    spinlock_t smtx_;

    boost::fibers::condition_variable_any rvar_;

    //wait_queue_splk_{};
    //typedef boost::fibers::context::wait_queue_t wait_queue_type;
    //wait_queue_type read_wait_queue_{};
    //wait_queue_type write_wait_queue_{};

public:
    rw_fiber_mutex_ext() = default;
    rw_fiber_mutex_ext(rw_fiber_mutex_ext const&) = delete;
    rw_fiber_mutex_ext& operator= (rw_fiber_mutex_ext const&) = delete;

    void lock_shared() {
        for (;;) {
            if (state_.fetch_sub(1, std::memory_order_acquire) <= 0) {
                return;
            }

            intptr_t stateval = state_.fetch_add(1, std::memory_order_relaxed);

            if (stateval < 0) continue;

            BOOST_ASSERT(stateval != 0);

            mtx_.lock();
            mtx_.unlock();
        }
    }

    void unlock_shared() {
        intptr_t stateval = 1 + state_.fetch_add(1, std::memory_order_release);
        if (stateval > 0 && 0 == (stateval % state_pos_value)) {
            std::lock_guard<spinlock_t> guard(smtx_);
            rvar_.notify_one();
        }
    }

    void promote() {
        intptr_t stateval = state_.fetch_add(state_pos_value + 1, std::memory_order_acquire);
        BOOST_ASSERT(!stateval);
        if (-1 == stateval) {
            mtx_.lock();
            return;
        } else if (stateval < 0) { // there is an another shared lock at the moemnt
            mtx_.lock();
            std::unique_lock<spinlock_t> rnext_lock(smtx_);
            rvar_.wait(smtx_, [this]() { return 0 == state_.load(std::memory_order_relaxed) % state_pos_value; });
            return;
        }
        // there is a waiting write lock

        //
        intptr_t stateval = state_.fetch_sub(state_pos_value, std::memory_order_relaxed) - state_pos_value;
        if (stateval > 0 && 0 == (stateval % state_pos_value)) {
            std::lock_guard<spinlock_t> guard(smtx_);
            rvar_.notify_one();
        }
        lock();
    }

    void lock() {
        for (;;) {
            intptr_t stateval = state_.fetch_add(state_pos_value, std::memory_order_acquire);
            if (!stateval) {
                mtx_.lock();
                return;
            } else if (stateval < 0) { // there is a shared lock at the moemnt
                mtx_.lock();
                std::unique_lock<spinlock_t> rnext_lock(smtx_);
                rvar_.wait(smtx_, [this]() { return 0 == state_.load(std::memory_order_relaxed) % state_pos_value; });
                return;
            }

            intptr_t stateval = state_.fetch_sub(state_pos_value, std::memory_order_relaxed);

            if (stateval == state_pos_value) continue; // no locks at all, try again right away

            mtx_.lock();
            mtx_.unlock();
        }
    }

    void unlock() {
        state_.fetch_sub(state_pos_value, std::memory_order_release);
        mtx_.unlock();
    }

private:
    //void do_suspend(wait_queue_type & queue) {
    //    using namespace boost::fibers;
    //    context * active_ctx = context::active();
    //    detail::spinlock_lock lk{ wait_queue_splk_ };
    //    BOOST_ASSERT( ! active_ctx->wait_is_linked() );
    //    active_ctx->wait_link(queue);
    //    active_ctx->suspend(lk);
    //    BOOST_ASSERT(!active_ctx->wait_is_linked());
    //}
};

}


#endif // SONIA_RW_FIBER_MUTEX_EXT_HPP
