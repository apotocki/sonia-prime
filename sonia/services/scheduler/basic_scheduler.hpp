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
#include <boost/intrusive/list.hpp>

#include "sonia/concurrency.hpp"
#include "sonia/function.hpp"
#include "sonia/optional.hpp"
#include "sonia/logger/loggable.hpp"

#include "scheduler.hpp"
#include "fiber_work_stealing_scheduler.hpp"

#include "sonia/utility/object_pool.hpp"

namespace sonia {

class basic_scheduler;

namespace scheduler_detail {

class task_entry;
using task_entry_pool_t = object_pool<task_entry, sonia::spin_mutex>;

class task_entry 
    : public boost::intrusive::list_base_hook<>
{
public:
    template <typename T, class ... ArgsT>
    task_entry(in_place_type_t<T> ipt, ArgsT&& ... args)
        : task_(ipt, std::forward<ArgsT>(args) ...)
    {}

    explicit task_entry(scheduler_task_t && t)
        : task_(std::move(t))
    {}

    void add_ref() { ++refs_; }
    void release_ref(task_entry_pool_t & pool);
    bool cancel();
    void run();

private:
    scheduler_task_t task_;

    using ref_integral_type = unsigned long;
    static constexpr ref_integral_type handled_flag_value =
        ((ref_integral_type)1) << (sizeof(ref_integral_type) * CHAR_BIT - 1);

    std::atomic<ref_integral_type> refs_{handled_flag_value + 1}; // high bit is a 'handled' flag
};

class basic_task_handle : public scheduler_task_handle
{
public:
    bool cancel() override;

    void add_ref() override { ++refs_; }
    void release_ref() override;

    basic_task_handle(shared_ptr<basic_scheduler>, task_entry * task);
    basic_task_handle(basic_task_handle const&) = delete;
    basic_task_handle(basic_task_handle &&) = delete;
    basic_task_handle& operator=(basic_task_handle const&) = delete;
    basic_task_handle& operator=(basic_task_handle &&) = delete;

    ~basic_task_handle() override;

private:
    shared_ptr<basic_scheduler> scheduler_;
    task_entry * task_;
    std::atomic<long> refs_{1};
};

} // scheduler_detail

class basic_scheduler 
    : public scheduler
    , public virtual loggable
    , public enable_shared_from_this<basic_scheduler>
{
    friend class scheduler_detail::basic_task_handle;
    
    using handle_pool_t = object_pool<scheduler_detail::basic_task_handle, sonia::spin_mutex>;
    using entry_list_t = boost::intrusive::list<scheduler_detail::task_entry>;

public:
    basic_scheduler(uint32_t thr_cnt, uint32_t fb_cnt);
    ~basic_scheduler() override;

    basic_scheduler(basic_scheduler const&) = delete;
    basic_scheduler(basic_scheduler &&) = delete;
    basic_scheduler& operator=(basic_scheduler const&) = delete;
    basic_scheduler& operator=(basic_scheduler &&) = delete;
    
    void start();
    void stop();

    virtual void on_new_thread() {};

    void post(when_t when, scheduler_task_t &&) override;
    task_handle_ptr handled_post(when_t when, scheduler_task_t &&) override;
    task_handle_ptr post_and_repeat(time_duration_t interval, scheduler_task_t &&) override;

protected:


private:
    virtual std::string thread_name() const;
    void thread_proc();
    void fiber_proc(fibers::mutex & mtx);

    void release_task_ref(scheduler_detail::task_entry * te);
    void free_task_handle(scheduler_detail::basic_task_handle * th);

    template <class ... ArgsT>
    task_handle_ptr do_post(bool wh, ArgsT && ... args);

    bool unlink_and_cancel(scheduler_detail::task_entry *);

    bool queue_not_safe_empty() const;
    scheduler_detail::task_entry * queue_not_safe_pop_next();

    uint32_t thr_cnt_, fb_cnt_;
    std::vector<thread> threads_;
    fiber_work_stealing_scheduler2::group_host gh_;
    optional<fibers::barrier> tbarrier_;

    fibers::mutex queue_mtx_;
    fibers::condition_variable_any queue_cond_;
    entry_list_t entries_;

    bool stopping_;
    mutable fibers::mutex close_mtx_;
    fibers::condition_variable close_cond_;

    scheduler_detail::task_entry_pool_t task_pool_;
    handle_pool_t handle_pool_;

};

}

#endif // SONIA_BASIC_SCHEDULER_HPP
