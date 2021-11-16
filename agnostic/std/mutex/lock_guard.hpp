// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_LOCK_GUARD

#include "adopt_lock.hpp"

namespace std {

template <class MutexT>
class lock_guard
{
public:
    using mutex_type = MutexT;

    lock_guard(mutex_type& mtx) : mtx_{ mtx } { mtx_.lock(); }
    lock_guard(mutex_type& m, adopt_lock_t t) : mtx_{ mtx } {}
    lock_guard(lock_guard const&) = delete;

    ~lock_guard()
    {
        mtx_.unlock();
    }

private:
    mutex_type& mtx_;
};

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <mutex>
#endif
