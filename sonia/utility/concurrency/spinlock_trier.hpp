//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <atomic>
#include <algorithm>
#include <random>

#include "sonia/fibers/detail/config.hpp"
#include "sonia/fibers/detail/cpu_relax.hpp"
#include "sonia/fibers/detail/spinlock_status.hpp"

namespace sonia {

class spinlock_trier
{
    std::atomic<fibers::detail::spinlock_status> state_{fibers::detail::spinlock_status::unlocked};
    size_t attempts_;

public:
    explicit spinlock_trier(size_t attempts = 32) : attempts_(attempts) {}
    spinlock_trier(spinlock_trier const&) = delete;
    spinlock_trier & operator=(spinlock_trier const&) = delete;

    bool try_lock() noexcept
    {
#if defined(BOOST_FIBERS_SPIN_SINGLE_CORE)
        return single_try_lock();
#else
        static thread_local std::minstd_rand generator{ std::random_device{}() };
        std::size_t collisions = 0 ;
        for (;;) {
            size_t retries = 0;
            while (fibers::detail::spinlock_status::locked == state_.load(std::memory_order_relaxed)) {
                if (attempts_ > retries) {
                    ++retries;
                    cpu_relax();
                } else {
                    return false;
                }
            }
#endif
            if (fibers::detail::spinlock_status::locked == state_.exchange(fibers::detail::spinlock_status::locked, std::memory_order_acquire)) {
                std::uniform_int_distribution<size_t > distribution{0, static_cast<size_t>(1) << (std::min)(collisions, 8)};
                const size_t z = distribution(generator);
                ++collisions;
                for (std::size_t i = 0; i < z; ++i) {
                    // -> reduces the power consumed by the CPU
                    // -> prevent pipeline stalls
                    cpu_relax();
                }
            } else {
                return true;
            }
        }
    }

    bool single_try_lock() noexcept
    {
        return fibers::detail::spinlock_status::unlocked == state_.exchange(fibers::detail::spinlock_status::locked, std::memory_order_acquire);
    }

    void unlock() noexcept
    {
        state_.store(fibers::detail::spinlock_status::unlocked, std::memory_order_release);
    }
};

}
