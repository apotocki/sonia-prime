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

#include <boost/fiber/all.hpp>

namespace sonia {

using boost::thread;
using std::mutex;
using std::unique_lock;
using std::condition_variable;

using boost::lock_guard;
using boost::make_lock_guard;
using boost::fibers::fiber;

namespace fibers = boost::fibers;
namespace this_thread = boost::this_thread;
namespace this_fiber = boost::this_fiber;

struct dummy_mutex_t {
    void lock() {}
    void unlock() {}
    void lock_shared() {}
    void unlock_shared() {}
};

constexpr dummy_mutex_t dummy_mutex = {};

template <typename MutexT>
unique_lock<MutexT> make_unique_lock(MutexT & m) {
    return unique_lock<MutexT>(m);
}

}

#endif // SONIA_THREAD_HPP
