//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_THREAD_HPP
#define SONIA_THREAD_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

namespace sonia {

using boost::thread;
using boost::mutex;
using boost::lock_guard;
using boost::unique_lock;
using boost::condition_variable;

namespace this_thread = boost::this_thread;

struct dummy_mutex_t {
    void lock() {}
    void unlock() {}
    void lock_shared() {}
    void unlock_shared() {}
};

constexpr dummy_mutex_t dummy_mutex = {};

}

#endif // SONIA_THREAD_HPP
