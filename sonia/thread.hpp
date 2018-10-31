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
#include <boost/thread/shared_lock_guard.hpp>

#include <boost/fiber/detail/spinlock.hpp>
#include <boost/fiber/all.hpp>

namespace sonia {

using boost::thread;
using std::mutex;
using std::unique_lock;
using std::condition_variable;

using boost::lock_guard;
using boost::shared_lock_guard;
using boost::make_lock_guard;
using boost::fibers::fiber;

using boost::fibers::detail::spinlock;

namespace fibers {
    using namespace boost::fibers;
}

namespace this_fiber = boost::this_fiber;

namespace this_thread {
    using namespace boost::this_thread;
}

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

template <typename MutexT>
shared_lock_guard<MutexT> make_shared_lock_guard(MutexT & m) {
    return shared_lock_guard<MutexT>(m);
}

enum class rw_type {
    shared = 0,
    exclusive
};

template <typename RWMutexT>
class rw_lock_guard {
public:
    explicit rw_lock_guard(RWMutexT & mtx, rw_type rwt)
        : mtx_(mtx), rwt_(rwt)
    {
        if (rw_type::shared == rwt) {
            mtx_.lock_shared();
        } else {
            mtx_.lock();
        }
    }

    rw_lock_guard(rw_lock_guard const&) = delete;
    rw_lock_guard(rw_lock_guard &&) = default;
    rw_lock_guard& operator=(rw_lock_guard const&) = delete;
    rw_lock_guard& operator=(rw_lock_guard &&) = default;

    void promote() {
        BOOST_ASSERT(rw_type::shared == rwt_);
        mtx_.promote();
        rwt_ = rw_type::exclusive;
    }

    void demote() {
        BOOST_ASSERT(rw_type::exclusive == rwt_);
        mtx_.demote();
        rwt_ = rw_type::shared;
    }

    ~rw_lock_guard() {
        if (rw_type::shared == rwt_) {
            mtx_.unlock_shared();
        } else {
            mtx_.unlock();
        }
    }

private:
    RWMutexT & mtx_;
    rw_type rwt_;
};

template <typename MutexT>
rw_lock_guard<MutexT> make_rw_lock_guard(MutexT & m, rw_type rwt) {
    return rw_lock_guard<MutexT>(m, rwt);
}

}

#endif // SONIA_THREAD_HPP
