//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FIBER_WORK_STEALING_SCHEDULER_HPP
#define SONIA_FIBER_WORK_STEALING_SCHEDULER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <vector>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include "sonia/utility/simple_queue.hpp"

#include "sonia/concurrency.hpp"
#include "sonia/utility/concurrency/debug_mutex.hpp"

namespace sonia {

class fiber_work_stealing_scheduler : public fibers::algo::algorithm
{
public:
    class group_host
    {
        friend class fiber_work_stealing_scheduler;

        std::vector<boost::intrusive_ptr<fiber_work_stealing_scheduler>> schedulers;
        threads::debug_mutex mtx;
        size_t victim_sched_idx = 0;
        std::atomic<long> sleeping_cnt{0};
    public:
        fibers::context * steal(fiber_work_stealing_scheduler * exc);
        void remove(fiber_work_stealing_scheduler *);

        void notify_one();
    };

    explicit fiber_work_stealing_scheduler(group_host & g, bool suspend = true);

    fiber_work_stealing_scheduler(fiber_work_stealing_scheduler const&) = delete;
    fiber_work_stealing_scheduler(fiber_work_stealing_scheduler &&) = delete;
    fiber_work_stealing_scheduler& operator=(fiber_work_stealing_scheduler const&) = delete;
    fiber_work_stealing_scheduler& operator=(fiber_work_stealing_scheduler &&) = delete;

    void awakened(fibers::context*) noexcept override final;

    fibers::context * pick_next() noexcept override final;

    bool has_ready_fibers() const noexcept override final
    {
        return !rqueue_.empty();
    }

    void suspend_until(std::chrono::steady_clock::time_point const&) noexcept override final;

    void notify() noexcept override final;

    fibers::context * steal() noexcept;

    bool try_notify() noexcept;

private:
    group_host & group_;
    threads::mutex mtx_;
    threads::condition_variable cnd_;
    simple_queue<fibers::context*, spin_mutex> rqueue_{1024};
    uint8_t flag_ : 1;
    uint8_t waiting_ : 1;
    uint8_t suspend_ : 1;
};

class fiber_work_stealing_scheduler2 : public fibers::algo::algorithm
{
public:
    struct group_host
    {
        spin_mutex mtx;
        std::vector<boost::intrusive_ptr<fiber_work_stealing_scheduler2>> schedulers;
        simple_queue<fibers::context*, spin_mutex> rqueue_{1024};
    };

    explicit fiber_work_stealing_scheduler2(group_host & g, bool suspend = true);

    fiber_work_stealing_scheduler2(fiber_work_stealing_scheduler2 const&) = delete;
    fiber_work_stealing_scheduler2(fiber_work_stealing_scheduler2 &&) = delete;
    fiber_work_stealing_scheduler2& operator=(fiber_work_stealing_scheduler const&) = delete;
    fiber_work_stealing_scheduler2& operator=(fiber_work_stealing_scheduler2 &&) = delete;

    void awakened(fibers::context*) noexcept override final;

    fibers::context * pick_next() noexcept override final;

    bool has_ready_fibers() const noexcept override final
    {
        return !rqueue_.empty() || !group_.rqueue_.empty();
    }

    void suspend_until(std::chrono::steady_clock::time_point const&) noexcept override final;

    void notify() noexcept override final;

private:
    group_host & group_;
    threads::mutex mtx_;
    threads::condition_variable cnd_;
    simple_queue<fibers::context*, spin_mutex> rqueue_{2};
    uint8_t flag_ : 1;
    uint8_t suspend_ : 1;
};

}

#endif // SONIA_FIBER_WORK_STEALING_SCHEDULER_HPP
