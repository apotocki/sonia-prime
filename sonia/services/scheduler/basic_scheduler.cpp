//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sal.hpp"
#include "sonia/utility/scope_exit.hpp"

#include "basic_scheduler.hpp"


#ifndef SONIA_TASK_POOL_INITIAL_SIZE
#   define SONIA_TASK_POOL_INITIAL_SIZE 1024
#endif

namespace sonia { namespace scheduler_detail {

bool basic_scheduler_task_handle_impl::cancel(uintptr_t cookie)
{
    auto * s = sched(cookie);
    if (entry_->is_priority_type()) {
        if (!s->unlink(*static_cast<priority_queue_entry*>(entry_))) return false;
    } else {
        if (!s->unlink(*entry_)) return false;
    }
    return true;
}

void basic_scheduler_task_handle_impl::reschedule(uintptr_t cookie, when_t when, time_duration_t td)
{
    auto * s = sched(cookie);
    time_duration_t now = s->relative_now();
    time_duration_t const* wtd = boost::get<time_duration_t>(&when);
    uint64_t dwhen = (wtd ? (*wtd + now) : now).count();

    priority_queue_entry * pe;
    if (entry_->is_priority_type()) {
        pe = static_cast<priority_queue_entry*>(entry_);
        if (!s->unlink(*pe)) {
            entry_->set_skipped();
            priority_queue_entry * new_pe = s->priority_queue_entry_pool_.new_object(dwhen, td.count(), entry_->task);
            entry_->release_ref(s);
            entry_ = pe = new_pe;
        } else {
            pe->time = dwhen;
            pe->rival = td.count();
        }
    } else {
        s->unlink(*entry_);
        entry_->set_skipped();
        pe = s->priority_queue_entry_pool_.new_object(dwhen, td.count(), entry_->task);
        entry_->release_ref(s);
        entry_ = pe;
    }

    pe->add_ref();
    if (!td.count() && (!when.which() || (wtd && !wtd->count()))) {
        s->push(*pe);
    } else if (wtd) {
        s->schedule_task(*pe, now);
    }
}

void basic_scheduler_task_handle_impl::destroy(uintptr_t cookie)
{
    entry_->release_ref(sched(cookie));
}

///////////////
void queue_entry::add_ref()
{
    ++flags_and_refs_;
}

void queue_entry::release_ref(basic_scheduler * s) noexcept
{
    if (1 == (refs_bits & flags_and_refs_.fetch_sub(1))) {
        s->free_entry(this);
    }
}

void queue_entry::set_skipped() noexcept
{
    flags_and_refs_.fetch_or(skip_bit);
}

void queue_entry::run(basic_scheduler * s)
{
    task->run();
    if (is_priority_type()) {
        auto * pe = static_cast<priority_queue_entry*>(this);
        if (pe->rival) {
            time_duration_t now = s->relative_now();
            pe->time = now.count() + pe->rival;
            s->schedule_task(*pe, now);
        }
    }
}

void queue_entry::on_cancel(basic_scheduler * s)
{
    task->on_cancel();
}

} // namespace sonia::scheduler_detail

using namespace ::sonia::scheduler_detail;

basic_scheduler::basic_scheduler(uint32_t thr_cnt, uint32_t fb_cnt)
    : thr_cnt_{thr_cnt}, fb_cnt_{fb_cnt}, stopping_{true}
    , queue_entry_pool_{SONIA_TASK_POOL_INITIAL_SIZE}
    , priority_queue_entry_pool_{SONIA_TASK_POOL_INITIAL_SIZE}
    , tp_steady_start_{std::chrono::steady_clock::now()}
{
    timer_ = services::timer{[this]{
        LOG_TRACE(logger()) << "on_priority_timer " << this_thread::get_id();
        on_priority_timer();
    }};
}

basic_scheduler::~basic_scheduler()
{
    timer_.disarm();
    timer_ = services::timer{};
}

std::string basic_scheduler::thread_name() const
{
    return "scheduler thread";
}

void basic_scheduler::start()
{
    {
        lock_guard lck(queue_mtx_);
        if (!stopping_) return;
        stopping_ = false;
    }

    threads_.reserve(thr_cnt_);
    if (thr_cnt_) {
        tbarrier_.emplace(thr_cnt_);
    }

    for (size_t t = 0; t < thr_cnt_; ++t) {
        threads_.emplace_back([this]() { on_new_thread(); thread_proc(); });
        sal::set_thread_name(threads_.back().get_id(), ("%1% #%2%"_fmt % thread_name() % t).str());
    }
}

void basic_scheduler::stop()
{
    // stop timer
    timer_.disarm();

    if (lock_guard lck(queue_mtx_); !stopping_) {
        stopping_ = true;
        queue_cond_.notify_all();
    } else return;

    // deal with priority queue
    for (lock_guard lck(priority_mtx_); !priority_set_.empty();)
    {
        auto it = priority_set_.begin();
        priority_queue_entry & pe = *it;
        priority_set_.erase(it);
        pe.on_cancel(this);
        pe.release_ref(this);
    }

    if (threads_.empty()) return;
    // fiber friendly thread join procedure
    unique_lock lk(close_mtx_);
    thread([this] {
        for (thread & t : threads_) {
            t.join();
        }
        {
            unique_lock lk(close_mtx_);
            threads_.clear();
            close_cond_.notify_one();
        }
    }).detach();
    close_cond_.wait(lk, [this] { return threads_.empty(); });
}

void basic_scheduler::thread_proc()
{
    //LOG_TRACE(logger()) << "start thread proc, thread: " << this_thread::get_id();
    //fibers::use_scheduling_algorithm<fiber_work_stealing_scheduler>(gh_, true);
    fibers::use_scheduling_algorithm<fiber_work_stealing_scheduler2>(gh_, true);
    {
        std::vector<fiber> fibers;
        for (size_t f = 0; f < fb_cnt_; ++f) {
            fibers.emplace_back([this]() { fiber_proc(); });
        }

        // DO NOT USE MAIN CONTEXT FOR THE REAL WORK (due to performance penalty during the scheduling routine for pinned contexts)
        // fiber_proc();
        
        for (fiber & f : fibers) {
            f.join();
            //LOG_TRACE(logger()) << "fiber finished " << this_fiber::get_id() << ", thread " << this_thread::get_id();
        }
        //LOG_TRACE(logger()) << "all fibers finished, thread " << this_thread::get_id();
        tbarrier_->wait();
        //LOG_TRACE(logger()) << "barrier finished " << ", thread " << ctid;
    }
}

void basic_scheduler::fiber_proc()
{
    for (;;)
    {
        try
        {
            bool stopping = false;
            bool has_more = false;

            queue_entry * pe;
            {
                //LOG_TRACE(logger()) << "before acquire guard fiber " << this_fiber::get_id() << ", thread: " << this_thread::get_id();
                unique_lock lck(queue_mtx_);
                //LOG_TRACE(logger()) << "got guard fiber " << this_fiber::get_id() << "," << sonia::fibers::context::active2() << ", thread: " << this_thread::get_id() << ", stopping: " << stopping_;
                queue_cond_.wait(lck, [this]() { return !queue_not_safe_empty() || stopping_; });
                if (stopping_ && queue_not_safe_empty()) {
                    //LOG_TRACE(logger()) << "return0 from fiber " << this_fiber::get_id() << ", thread: " << this_thread::get_id();
                    return;
                }
                stopping = stopping_;
                pe = queue_not_safe_pop_next();
                has_more = !queue_not_safe_empty();
            }
            
            if (has_more) queue_cond_.notify_one();
            SCOPE_EXIT([this, pe]{ pe->release_ref(this); });
            if (!pe->is_skipped()) {
                if (BOOST_LIKELY(!stopping)) {
                    pe->run(this); // pe ownership is moving to run();
                } else {
                    pe->on_cancel(this); // pe ownership is moving to on_cancel();
                }
            }
        } catch (...) {
            LOG_ERROR(logger()) << boost::current_exception_diagnostic_information();
        }
    }
}

//template <class ... ArgsT>
//task_handle_ptr basic_scheduler::do_post(bool wh, ArgsT && ... args)
//{
//    task_handle_ptr result;
//    task_entry * pe = task_pool_.new_object(std::forward<ArgsT>(args) ...);
//    if (wh) {
//        try {
//            result.reset(handle_pool_.new_object(shared_from_this(), pe), false);
//        } catch (...) {
//            release_task_ref(pe);
//            throw;
//        }
//    }
//    {
//        lock_guard guard(queue_mtx_);
//        tasks_.push_front(*pe);
//    }
//    queue_cond_.notify_one();
//    return std::move(result);
//}

void basic_scheduler::push(queue_entry & e, bool front) noexcept
{
    bool first;
    {
        lock_guard guard(queue_mtx_);
        if (e.is_linked()) return;
        first = entries_.empty();
        if (front) {
            entries_.push_front(e);
        } else {
            entries_.push_back(e);
        }
    }
    if (first) {
        queue_cond_.notify_one();
    }
}

scheduler_task_handle basic_scheduler::post(scheduler_task_t && t, when_t when, time_duration_t td)
{
    queue_entry * qe;
    time_duration_t const* optwtd = boost::get<time_duration_t>(&when);
    if (!td.count() && (!optwtd || (optwtd && !optwtd->count()))) {
        qe = queue_entry_pool_.new_object(std::move(t));
        qe->add_ref();
        push(*qe);
    } else {
        time_duration_t wtd = optwtd ? *optwtd : time_duration_t{ 0 };
        time_duration_t now = relative_now();
        time_duration_t dwhen = wtd + now;
        priority_queue_entry * pe = priority_queue_entry_pool_.new_object(dwhen.count(), td.count(), std::move(t));
        if (!optwtd || !optwtd->count()) { // when = now
            pe->add_ref();
            push(*pe);
        } else {
            schedule_task(*pe, now);
        }
        qe = pe;
    }
    return scheduler_task_handle{reinterpret_cast<uintptr_t>(this), in_place_type<basic_scheduler_task_handle_impl>, qe}; // no qe->add_ref() inside
}

bool basic_scheduler::queue_not_safe_empty() const
{
    return entries_.empty();
}

queue_entry * basic_scheduler::queue_not_safe_pop_next()
{
    queue_entry * pe = &entries_.back();
    entries_.pop_back();
    return pe;
}

bool basic_scheduler::unlink(queue_entry & e)
{
    if (lock_guard guard(queue_mtx_); e.is_linked()) {
        entries_.erase(entry_list_t::s_iterator_to(e));
        e.release_ref(this);
        return true;
    } else {
        return false;
    }
}

bool basic_scheduler::unlink(priority_queue_entry & e)
{
    if (unique_lock lck(priority_mtx_); e.set_hook_.is_linked()) {
        auto beg_it = priority_set_.begin();
        auto e_it = priority_set_t::s_iterator_to(e);
        bool is_b = beg_it == e_it;
        priority_set_.erase(e_it);
        e.release_ref(this);
        lck.unlock();
        if (is_b) {
            //LOG_INFO(logger()) << "unlink on_priority_timer call";
            on_priority_timer();
        }
        return true;
    } else {
        return unlink(static_cast<queue_entry&>(e));
    }
}

void basic_scheduler::schedule_task(priority_queue_entry & e, time_duration_t now)
{
    if (lock_guard guard(priority_mtx_); !e.set_hook_.is_linked()) {
        auto it = priority_set_.insert(e);
        e.add_ref();
        try {
            schedule_timer(it, now.count());
        } catch (...) {
            priority_set_.erase(it);
            e.release_ref(this);
            throw;
        }
    }
}

void basic_scheduler::schedule_timer(priority_set_t::iterator it, int64_t now)
{
    priority_queue_entry & e = *it;
    if (priority_lowest_ > e.time) {
        priority_lowest_ = e.time;
        int64_t resched_duration = priority_lowest_ - now;
        if (resched_duration < 1) { // (time_duration_t)1 resolution
            priority_set_.erase(it);
            push(e, false);
            priority_lowest_ = priority_max_val_;
        } else {
            time_duration_t td{resched_duration};
            LOG_TRACE(logger()) << "set timeout: " << td.count() << " ms";
            timer_.set(td);
        }
    }
}

void basic_scheduler::on_priority_timer()
{
    time_duration_t now = relative_now();
    lock_guard guard(priority_mtx_);
    priority_lowest_ = priority_max_val_;
    while (!priority_set_.empty()) {
        auto beg_it = priority_set_.begin();
        schedule_timer(beg_it, now.count());
        if (priority_lowest_ != priority_max_val_) break;
    }
    // if (priority_lowest_ == priority_max_val_) timer_.disarm();
}

basic_scheduler::time_duration_t basic_scheduler::relative_now() const
{
    return std::chrono::duration_cast<scheduler::time_duration_t>(std::chrono::steady_clock().now() - tp_steady_start_);
}

void basic_scheduler::free_entry(queue_entry * e) noexcept
{
    if (e->is_priority_type()) {
        priority_queue_entry_pool_.delete_object(static_cast<priority_queue_entry*>(e));
    } else {
        queue_entry_pool_.delete_object(e);
    }
}

}
