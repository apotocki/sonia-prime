//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <atomic>

#include <boost/thread/barrier.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/set.hpp>

#include "sonia/concurrency.hpp"
#include "sonia/function.hpp"
#include "sonia/optional.hpp"
#include "sonia/logger/loggable.hpp"

#include "scheduler.hpp"
#include "fiber_work_stealing_scheduler.hpp"

#include "sonia/utility/object_pool.hpp"
#include "sonia/services/timer.hpp"

namespace sonia {

class basic_scheduler;

namespace scheduler_detail {

class queue_entry;
class priority_queue_entry;

class basic_scheduler_task_handle_impl : public scheduler_task_handle_impl
{
public:
    explicit basic_scheduler_task_handle_impl(queue_entry * e) : entry_{e} {}

    bool cancel(uintptr_t cookie) override;
    void reschedule(uintptr_t cookie, when_t when, time_duration_t repeat_interval) override;

    void destroy(uintptr_t cookie) override;

    polymorphic_movable* move(void* address, size_t sz) override final
    {
        BOOST_ASSERT(sz >= sizeof(basic_scheduler_task_handle_impl));
        new (address) basic_scheduler_task_handle_impl(entry_);
        return reinterpret_cast<basic_scheduler_task_handle_impl*>(address);
    }

private:
    static basic_scheduler * sched(uintptr_t cookie) { return reinterpret_cast<basic_scheduler *>(cookie); }
    queue_entry * entry_;
};

class queue_entry : public boost::intrusive::list_base_hook<>
{
public:
    using flags_and_refs_t = uint16_t;
    static const flags_and_refs_t refs_bits = (flags_and_refs_t)0xff;
    static const flags_and_refs_t type_bit = (flags_and_refs_t)0x100;
    static const flags_and_refs_t skip_bit = (flags_and_refs_t)0x200;

    template <class TaskT>
    explicit queue_entry(TaskT && t, bool type = false) noexcept
        : task{std::forward<TaskT>(t)}
    {
        if (type) {
            flags_and_refs_.fetch_or(type_bit);
        }
    }

    queue_entry(queue_entry const&) = delete;
    queue_entry& operator=(queue_entry const&) = delete;

    void run(basic_scheduler *);
    void on_cancel(basic_scheduler *);

    //void reset() noexcept;

    bool is_priority_type() const noexcept { return !!(flags_and_refs_.load() & type_bit); }
    bool is_skipped() const noexcept { return !!(flags_and_refs_.load() & skip_bit); }

    void add_ref();
    void release_ref(basic_scheduler *) noexcept;

    void set_skipped() noexcept;

    scheduler_task_t task;

protected:
    std::atomic<flags_and_refs_t> flags_and_refs_{1}; //0-7 bits: refs; 8 bit: type; 9 bit: skipped
};

class priority_queue_entry : public queue_entry
{
public:
    int64_t time;
    int64_t rival;
    boost::intrusive::set_member_hook<> set_hook_;

    template <class TaskT>
    priority_queue_entry(int64_t tm, int64_t ri, TaskT && t)
        : queue_entry{std::forward<TaskT>(t), true}, time{tm}, rival{ri}
    {}

    friend bool operator< (priority_queue_entry const& l, priority_queue_entry const& r) { return l.time < r.time; }
    friend bool operator> (priority_queue_entry const& l, priority_queue_entry const& r) { return l.time > r.time; }
    friend bool operator== (priority_queue_entry const& l, priority_queue_entry const& r) { return l.time == r.time; }
};

using queue_entry_pool_t = object_pool<queue_entry, sonia::spin_mutex>;
using priority_queue_entry_pool_t = object_pool<priority_queue_entry, sonia::spin_mutex>;

using entry_list_t = boost::intrusive::list<queue_entry>;

using priority_set_t = boost::intrusive::multiset<
    priority_queue_entry,
    boost::intrusive::member_hook<
        priority_queue_entry, boost::intrusive::set_member_hook<>, &priority_queue_entry::set_hook_
    >>;

} // scheduler_detail

class basic_scheduler 
    : public scheduler
    , public virtual loggable
    , public enable_shared_from_this<basic_scheduler>
{
    friend class scheduler_detail::queue_entry;
    friend class scheduler_detail::priority_queue_entry;
    friend class scheduler_detail::basic_scheduler_task_handle_impl;

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

    scheduler_task_handle post(scheduler_task_t &&, when_t, time_duration_t) override;
   
    void push(scheduler_detail::queue_entry & t, bool front = true) noexcept;

private:
    virtual std::string thread_name() const;
    void thread_proc();
    void fiber_proc();

    bool unlink(scheduler_detail::queue_entry &);
    bool unlink(scheduler_detail::priority_queue_entry &);

    bool queue_not_safe_empty() const;
    scheduler_detail::queue_entry * queue_not_safe_pop_next();
    
    void free_entry(scheduler_detail::queue_entry * e) noexcept;

    uint32_t thr_cnt_, fb_cnt_;
    std::vector<thread> threads_;
    
    //fiber_work_stealing_scheduler::group_host gh_;
    fiber_work_stealing_scheduler2::group_host gh_;
    optional<fibers::barrier> tbarrier_;

    fibers::mutex queue_mtx_;
    fibers::condition_variable_any queue_cond_;
    scheduler_detail::entry_list_t entries_;

    bool stopping_;
    mutable fibers::mutex close_mtx_;
    fibers::condition_variable close_cond_;

    scheduler_detail::queue_entry_pool_t queue_entry_pool_;
    scheduler_detail::priority_queue_entry_pool_t priority_queue_entry_pool_;

    time_duration_t relative_now() const;
    mutable fibers::mutex priority_mtx_;
    scheduler_detail::priority_set_t priority_set_;
    std::chrono::time_point<std::chrono::steady_clock> tp_steady_start_;
    static const int64_t priority_max_val_ = (std::numeric_limits<int64_t>::max)();
    int64_t priority_lowest_{priority_max_val_};

    void schedule_task(scheduler_detail::priority_queue_entry &, time_duration_t now);
    void schedule_timer(scheduler_detail::priority_set_t::iterator it, int64_t now); // unsafe in relation to priority_set_
    void on_priority_timer();
    services::timer timer_;
};

}
