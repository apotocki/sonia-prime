//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FIBER_MUTEX_HPP
#define SONIA_FIBER_MUTEX_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/thread.hpp"
#include "sonia/logger/logger.hpp"

namespace sonia {

class fiber_mutex {
private:
    friend class fibers::condition_variable;

    typedef fibers::context::wait_queue_t   wait_queue_type;

    spinlock wait_queue_splk_{};
    wait_queue_type wait_queue_{};
    fibers::context * owner_{ nullptr };

public:
    fiber_mutex() = default;

    ~fiber_mutex() {
        BOOST_ASSERT( nullptr == owner_);
        BOOST_ASSERT( wait_queue_.empty() );
    }

    fiber_mutex(fiber_mutex const&) = delete;
    fiber_mutex & operator=(fiber_mutex const&) = delete;

    void lock() {
        while ( true) {
            fibers::context * active_ctx = fibers::context::active();
            // store this fiber in order to be notified later
            fibers::detail::spinlock_lock lk{ wait_queue_splk_ };
            if ( BOOST_UNLIKELY( active_ctx == owner_) ) {
                throw fibers::lock_error{
                    std::make_error_code( std::errc::resource_deadlock_would_occur),
                    "boost fiber: a deadlock is detected" };
            } else if ( nullptr == owner_) {
                owner_ = active_ctx;
                GLOBAL_LOG_TRACE() << "lock acquired by " << owner_->get_id() << ", thread: " << this_thread::get_id();
                return;
            }
            BOOST_ASSERT( ! active_ctx->wait_is_linked() );
            active_ctx->wait_link( wait_queue_);
            // suspend this fiber
            GLOBAL_LOG_TRACE() << "suspended by lock " << active_ctx->get_id() << ", thread: " << this_thread::get_id();
            active_ctx->suspend( lk);
            GLOBAL_LOG_TRACE() << "awakened by lock " << active_ctx->get_id() << ", thread: " << this_thread::get_id();
            BOOST_ASSERT( ! active_ctx->wait_is_linked() );
        }
    }

    bool try_lock() {
        fibers::context * active_ctx = fibers::context::active();
        fibers::detail::spinlock_lock lk{ wait_queue_splk_ };
        if ( BOOST_UNLIKELY( active_ctx == owner_) ) {
            throw fibers::lock_error{
                std::make_error_code( std::errc::resource_deadlock_would_occur),
                "boost fiber: a deadlock is detected" };
        } else if ( nullptr == owner_) {
            owner_ = active_ctx;
        }
        lk.unlock();
        // let other fiber release the lock
        active_ctx->yield();
        return active_ctx == owner_;
    }

    void unlock() {
        fibers::context * active_ctx = fibers::context::active();
        fibers::detail::spinlock_lock lk{ wait_queue_splk_ };
        if ( BOOST_UNLIKELY( active_ctx != owner_) ) {
            throw fibers::lock_error{
                std::make_error_code( std::errc::operation_not_permitted),
                "boost fiber: no  privilege to perform the operation" };
        }
        GLOBAL_LOG_TRACE() << "release lock by fiber " << active_ctx->get_id() << ", thread: " << this_thread::get_id();
        owner_ = nullptr;
        if ( ! wait_queue_.empty() ) {
            fibers::context * ctx = & wait_queue_.front();
            wait_queue_.pop_front();
            bool remote = active_ctx->get_scheduler() != ctx->get_scheduler();
            GLOBAL_LOG_TRACE() << "lock wakes up fiber " << ctx->get_id() << (remote? " REMOTE" : " LOCAL") <<  ", thread: " << this_thread::get_id();
            active_ctx->schedule(ctx);
        }
    }
};

}

#endif // SONIA_FIBER_MUTEX_HPP
