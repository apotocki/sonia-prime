//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/shared_lock_guard.hpp>

#if 1
#include "sonia/fibers/future.hpp"
#include "sonia/fibers/fiber.hpp"
#include "sonia/fibers/barrier.hpp"
namespace sonia {
    using fibers::fiber;
    using spin_mutex = fibers::detail::spinlock;
}
#else
#include <boost/fiber/future.hpp>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/barrier.hpp>
namespace sonia {
    using boost::fibers::fiber;
    using spin_mutex = boost::fibers::detail::spinlock;
    namespace fibers {
        using namespace boost::fibers;
    }
    namespace this_fiber = boost::this_fiber;
}

#endif

namespace sonia {

using boost::thread;

using std::unique_lock;
using boost::lock_guard;
using boost::shared_lock_guard;

namespace threads {
    using std::mutex;
    using std::condition_variable;
}

namespace this_thread {
    using namespace boost::this_thread;
}

struct dummy_mutex_t
{
    void lock() {}
    void unlock() {}
    void lock_shared() {}
    void unlock_shared() {}
};

constexpr dummy_mutex_t dummy_mutex = {};

enum class rw_type
{
    shared = 0,
    exclusive
};

template <typename RWMutexT>
class rw_lock_guard
{
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

    void promote()
    {
        BOOST_ASSERT(rw_type::shared == rwt_);
        mtx_.promote();
        rwt_ = rw_type::exclusive;
    }

    void demote()
    {
        BOOST_ASSERT(rw_type::exclusive == rwt_);
        mtx_.demote();
        rwt_ = rw_type::shared;
    }

    ~rw_lock_guard()
    {
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


// traits
//struct fiber_traits
//{
//    using mutex_type = fibers::mutex;
//    using condition_variable_type = fibers::condition_variable;
//};

}
