//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_CONCURRENCY_BARRIER_HPP
#define SONIA_CONCURRENCY_BARRIER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <chrono>
#include <atomic>

#include "sonia/cstdint.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/function.hpp"

namespace sonia::concurrency {

template <class TraitsT>
class barrier
{
    using mutex_t = typename TraitsT::mutex_type;
    using condition_variable_t = typename TraitsT::condition_variable_type;

public:
    explicit barrier(uint32_t maxthreads = 1, std::chrono::milliseconds timeout = (std::numeric_limits<long long>::max)())
        : max_threads_(maxthreads)
        , barrier_timeout_millis_(timeout)
        , threshold_(0)
        , threads_(0)
        , main_threads_state_(0)
    {}

    barrier(barrier const&) = delete;
    barrier& operator=(barrier const&) = delete;

    void operator()(uint64_t generation, function<uint64_t()> const& mainfunc);

private:
    uint32_t max_threads_;
    std::chrono::milliseconds barrier_timeout_millis_;
    std::atomic<uint64_t> threshold_;
    std::atomic<uint32_t> threads_;
    std::atomic<uint32_t> main_threads_state_;
    std::chrono::steady_clock::time_point barrier_start_time_;
    mutex_t no_counted_threads_mtx_, main_mtx_, next_mtx_;
    condition_variable_t no_counted_threads_cv_;
    condition_variable_t main_thread_cv_;
    condition_variable_t next_thread_cv_;
};

template <class TraitsT>
void barrier<TraitsT>::operator()(uint64_t generation, function<uint64_t()> const& mainfunc)
{
    if (1 == max_threads_) return;
    uint32_t main_expected = 0;

    while (generation > threshold_.load())
    {
        if (main_threads_state_.compare_exchange_strong(main_expected, 1)) {
            // main thread
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            std::chrono::steady_clock::time_point end_time = start_time + barrier_timeout_millis_;

            // wait for thread counter reset
            {
                unique_lock lock(no_counted_threads_mtx_);
                while (threads_.load() > 0) {
                    no_counted_threads_cv_.wait(lock);
                }
            }
            
            // allow other threads to be collected and counted
            main_threads_state_.store(2);
            {
                unique_lock lock(next_mtx_);
                next_thread_cv_.notify_all();
            }

            // wait for condition
            {
                unique_lock lock(main_mtx_);
                while (threads_.load() < max_threads_ - 1 && start_time < end_time) {
                    main_thread_cv_.wait_until(lock, end_time);
                }
            }

            threshold_.store(mainfunc());
            
            // run collected threads
            main_threads_state_.store(0);
            unique_lock lock(next_mtx_);
            next_thread_cv_.notify_all();
        } else {
            unique_lock lock(next_mtx_);
            while (generation > threshold_.load() && 2 == main_threads_state_.load()) {
                if (max_threads_ == threads_.fetch_add(1)) {
                    unique_lock lock(main_mtx_);
                    main_thread_cv_.notify_one();
                }
                next_thread_cv_.wait(lock);
                if (1 == threads_.fetch_sub(1)) {
                    unique_lock lock(no_counted_threads_mtx_);
                    no_counted_threads_cv_.notify_one();
                }
            }

            while (generation > threshold_.load() && 1 == main_threads_state_.load()) {
                next_thread_cv_.wait(lock);
            }
        }
    }
}

}

#endif // SONIA_CONCURRENCY_BARRIER_HPP
