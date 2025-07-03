//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <atomic>
#include "sonia/concurrency.hpp"

namespace sonia::concurrency {

struct once_flag
{
    // 0 - uninitialized, 1 - running, 2 - done
    std::atomic<long> value_{ 0 };
    fibers::mutex mtx_;
    fibers::condition_variable cv_;
};

template< class Callable, class... Args>
void call_once(once_flag& flag, Callable&& f, Args&&... args)
    noexcept( noexcept(std::invoke(std::forward<Callable>(f), std::forward<Args>(args)...)) )
{
    if (flag.value_.load(std::memory_order_acquire) == 2) return;
    if (flag.value_.exchange(1, std::memory_order_acquire) == 0) {
        f(std::forward<Args>(args)...);
        flag.value_.store(2, std::memory_order_release);
        unique_lock lock{ flag.mtx_ };
        cv_.notify_all();
    } else {
        unique_lock lock{ flag.mtx_ };
        cv_.wait(lock, [&flag] { return flag.value_.load(std::memory_order_acquire) == 2; });
    }
}

}
