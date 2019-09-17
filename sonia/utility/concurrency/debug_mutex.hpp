//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_CONCURRENCY_DEBUG_MUTEX_HPP
#define SONIA_CONCURRENCY_DEBUG_MUTEX_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/concurrency.hpp"
#include "sonia/optional.hpp"
#include "sonia/exceptions.hpp"

namespace sonia::threads {

struct debug_mutex : public mutex
{
    spin_mutex smtx;
    optional<thread::id> owned_tid;

    void lock()
    {
        thread::id cur_tid = this_thread::get_id();
        {
            lock_guard guard(smtx);
            if (owned_tid && *owned_tid == cur_tid) {
                THROW_INTERNAL_ERROR("double lock by the same thread");
            }
        }
        mutex::lock();
        {
            lock_guard guard(smtx);
            owned_tid = cur_tid;
        }
    }

    void unlock()
    {
        thread::id cur_tid = this_thread::get_id();
        {
            lock_guard guard(smtx);
            if (!owned_tid) {
                THROW_INTERNAL_ERROR("an attempt to unlock an unlocked mutex");
            } else if (*owned_tid != cur_tid) {
                THROW_INTERNAL_ERROR("unlock: wrong thread");
            }
            mutex::unlock();
            owned_tid.reset();
        }
    }

    void lock_shared() { lock(); }
    void unlock_shared() { unlock(); }
};


}
#endif // SONIA_CONCURRENCY_DEBUG_MUTEX_HPP
