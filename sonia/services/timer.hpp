//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_TIMER_HPP
#define SONIA_SERVICES_TIMER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/prime_config.hpp"

#include <chrono>
#include "sonia/function.hpp"

#ifdef BOOST_WINDOWS
#   include "sonia/sys/windows/thread_pool.hpp"
#elif defined (__linux__)
#   include "sonia/sys/linux/timer.hpp"
#elif defined (__APPLE__)
#   include "sonia/sys/macos/timer.hpp"
#endif

namespace sonia::services {

#ifdef BOOST_WINDOWS
class timer
{
public:
    timer() = default;
    SONIA_PRIME_API explicit timer(function<void()> const&);
    ~timer() = default;

    timer(timer const&) = delete;
    timer(timer && rhs) = default;
    timer& operator=(timer const&) = delete;
    timer& operator=(timer && rhs) = default;

    void set(std::chrono::milliseconds ms) { impl_->set(ms); }
    void disarm() { impl_->disarm(); }

private:
    std::unique_ptr<windows::threadpool_timer> impl_;
};
#elif defined (__linux__)
using timer = linux::timer;
#elif defined (__APPLE__)
using timer = macos::timer;
#endif

}

#endif // SONIA_SERVICES_TIMER_HPP
