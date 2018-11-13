//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_THREAD_MANAGER_IPP
#define SONIA_SERVICES_THREAD_MANAGER_IPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/exception/diagnostic_information.hpp>

#include "thread_manager.hpp"
#include "sonia/sal.hpp"

namespace sonia {

template <class DerivedT>
thread_manager<DerivedT>::thread_manager(uint32_t thr_cnt, uint32_t fb_cnt)
    : thr_cnt_(thr_cnt), fb_cnt_(fb_cnt)
{}

template <class DerivedT>
thread_manager<DerivedT>::~thread_manager()
{}

template <class DerivedT>
void thread_manager<DerivedT>::start()
{
    auto ctl_guard = make_lock_guard(ctl_mtx_);
    bool exp = true;
    if (!stopping_.compare_exchange_strong(exp, false, std::memory_order::memory_order_relaxed) {
        return;
    }

    threads_.reserve(thr_cnt_);
    tbarrier_.emplace(thr_cnt_);

    for (size_t t = 0; t < thr_cnt_; ++t) {
        threads_.push_back(thread([this]() { thread_proc(); }));
        sal::set_thread_name(threads_.back().get_id(), ("%1% #%2%"_fmt % derived().thread_name() % t).str());
    }
}

template <class DerivedT>
void thread_manager<DerivedT>::stop()
{
    auto ctl_guard = make_lock_guard(ctl_mtx_);
    bool exp = false;
    {
        auto guard = make_lock_guard(queue_mtx_);
        if (!stopping_.compare_exchange_strong(exp, true, std::memory_order::memory_order_relaxed) {
            return;
        }
    }

    queue_cond_.notify_all();

    for (auto & t : threads_) {
        t.join();
    }

    threads_.clear();
}

template <class DerivedT>
void thread_manager<DerivedT>::thread_proc() noexcept
{
    try {
        fibers::use_scheduling_algorithm<fiber_work_stealing_scheduler>(gh_, true);

        std::vector<fiber> fibers;
        fibers.reserve(fb_cnt_);
        for (size_t f = 0; f < fb_cnt_; ++f) {
            fibers.push_back(boost::fibers::fiber([this]() { fiber_proc(); }));
        }

        fiber_proc();

        for (fiber & f : fibers) {
            f.join();
        }
        tbarrier_->wait();
    } catch (...) {
        LOG_ERROR(logger()) << boost::current_exception_diagnostic_information();
    }
}

template <class DerivedT>
void thread_manager<DerivedT>::fiber_proc() noexcept
{
    for (;;)
    {
        try {
            auto lck = make_unique_lock(queue_mtx_);
            queue_cond_.wait(lck, [this]() { return derived().queue_empty() || stopping_.load(std::memory_order::memory_order_relaxed); });
            if (stopping_.load(std::memory_order::memory_order_relaxed)) {
                return;
            }
            derived().handle_queue(lck);
        } catch (...) {
            LOG_ERROR(logger()) << boost::current_exception_diagnostic_information();
        }
    }
}

}

#endif // SONIA_SERVICES_THREAD_MANAGER_IPP
