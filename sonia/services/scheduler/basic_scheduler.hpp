//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_BASIC_SCHEDULER_HPP
#define SONIA_BASIC_SCHEDULER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <atomic>

#include <boost/thread/barrier.hpp>

#include "sonia/concurrency.hpp"
#include "sonia/function.hpp"
#include "sonia/optional.hpp"
#include "sonia/logger/loggable.hpp"

#include "scheduler.hpp"
#include "fiber_work_stealing_scheduler.hpp"

#include "sonia/utility/object_pool.hpp"

namespace sonia { namespace scheduler_detail {

class task_entry;
using task_entry_pool_t = object_pool<task_entry, sonia::spin_mutex>;

class task_entry {
public:
    template <typename T, class ... ArgsT>
    task_entry(in_place_type_t<T> ipt, ArgsT&& ... args)
        : task_(ipt, std::forward<ArgsT>(args) ...)
    {}

    void add_ref() { ++refs_; }
    void release_ref(task_entry_pool_t * ppool);
    void cancel();
    void run();

private:
    scheduler_task_t task_;
    std::atomic<long> refs_{1};
    std::atomic<bool> handled_{false};
};

}

class basic_scheduler 
    : public scheduler
    , public virtual loggable
{
public:
    basic_scheduler(uint32_t thr_cnt, uint32_t fb_cnt);
    ~basic_scheduler() override;

    basic_scheduler(basic_scheduler const&) = delete;
    basic_scheduler(basic_scheduler &&) = delete;
    basic_scheduler& operator=(basic_scheduler const&) = delete;
    basic_scheduler& operator=(basic_scheduler &&) = delete;
    
    void start() override;
    void stop() override;
    void post(scheduler_task_t &&) override;

protected:


private:
    virtual std::string thread_name() const;
    void thread_proc();
    void fiber_proc(fibers::mutex & mtx);

    uint32_t thr_cnt_, fb_cnt_;
    std::vector<thread> threads_;
    fiber_work_stealing_scheduler::group_host gh_;
    optional<fibers::barrier> tbarrier_;

    fibers::mutex queue_mtx_;
    fibers::condition_variable_any queue_cond_;
    bool stopping_;
    std::deque<function<void()>> queue_;

    //scheduler_detail::task_entry_pool_t task_pool_;
};

}

#endif // SONIA_BASIC_SCHEDULER_HPP
