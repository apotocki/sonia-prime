//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sal.hpp"
#include "sonia/utility/scope_exit.hpp"

#include "basic_scheduler.hpp"

#include <boost/fiber/all.hpp>

#ifndef SONIA_TASK_POOL_INITIAL_SIZE
#   define SONIA_TASK_POOL_INITIAL_SIZE 1024
#endif
#ifndef SONIA_TASK_HANDLE_POOL_INITIAL_SIZE
#   define SONIA_TASK_HANDLE_POOL_INITIAL_SIZE 128
#endif

namespace sonia { namespace scheduler_detail {

void task_entry::release_ref(task_entry_pool_t & pool)
{
    if (1 == refs_.fetch_sub(1)) {
        pool.delete_object(this);
    }
}

bool task_entry::cancel()
{
    for (;;) {
        auto expval = refs_.load();
        if (0 == (expval & handled_flag_value)) return false; // already some way handled
        if (refs_.compare_exchange_strong(expval, expval & ~handled_flag_value)) break;
    }
    task_->on_cancel();
    return true;
}

void task_entry::run()
{
    for (;;) {
        auto expval = refs_.load();
        if (0 == (expval & handled_flag_value)) return; // already some way handled
        if (refs_.compare_exchange_strong(expval, expval & ~handled_flag_value)) break;
    }
    task_->run();
}

basic_task_handle::basic_task_handle(shared_ptr<basic_scheduler> s, task_entry * task)
    : scheduler_(std::move(s)), task_(task)
{
    task_->add_ref();
}

basic_task_handle::~basic_task_handle()
{
    scheduler_->release_task_ref(task_);
}

void basic_task_handle::release_ref()
{
    if (0 == --refs_) {
        scheduler_->free_task_handle(this);
    }
}

bool basic_task_handle::cancel()
{
    return scheduler_->unlink_and_cancel(task_);
}

} // namespace sonia::scheduler_detail

using namespace ::sonia::scheduler_detail;

basic_scheduler::basic_scheduler(uint32_t thr_cnt, uint32_t fb_cnt)
    : thr_cnt_(thr_cnt), fb_cnt_(fb_cnt), stopping_(true)
    , task_pool_(SONIA_TASK_POOL_INITIAL_SIZE)
    , handle_pool_(SONIA_TASK_HANDLE_POOL_INITIAL_SIZE)
{

}

basic_scheduler::~basic_scheduler()
{

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
    tbarrier_.emplace(thr_cnt_);

    for (size_t t = 0; t < thr_cnt_; ++t) {
        threads_.push_back(thread([this]() { thread_proc(); }));
        sal::set_thread_name(threads_.back().get_id(), ("%1% #%2%"_fmt % thread_name() % t).str());
    }
}

void basic_scheduler::stop()
{
    {
        lock_guard lck(queue_mtx_);
        if (stopping_) return;
        stopping_ = true;
        queue_cond_.notify_all();
    }

    for (auto & t : threads_) {
        t.join();
    }

    threads_.clear();
}



void basic_scheduler::thread_proc()
{
    fibers::use_scheduling_algorithm<fiber_work_stealing_scheduler>(gh_, true);
    /*
    fibers::context::active()->get_scheduler()->exthook = [](fibers::context::id fid, int op) {
        switch (op) {
        case 0:
            GLOBAL_LOG_TRACE() << "fiber pushed into remote_queue " << fid << ", thread " << this_thread::get_id();
            break;
        case 1:
            GLOBAL_LOG_TRACE() << "fiber popped from remote_queue " << fid << ", thread " << this_thread::get_id();
            break;
        default:
            GLOBAL_LOG_TRACE() << "fiber" << fid << ", thread " << this_thread::get_id() << " unknown op " << op;
        }
    };
    */

    {
        fibers::mutex mtx;
        std::vector<fiber> fibers;
        for (size_t f = 0; f < fb_cnt_; ++f) {
            fibers.push_back(boost::fibers::fiber([this, &mtx]() { fiber_proc(mtx); }));
        }

        fiber_proc(mtx);

        auto ctid = this_thread::get_id();

        for (fiber & f : fibers) {
            auto fid = f.get_id();
            f.join();
            //LOG_TRACE(logger()) << "fiber finished " << fid << ", thread " << ctid;
        }
        tbarrier_->wait();
        //LOG_TRACE(logger()) << "barrier finished " << ", thread " << ctid;
    }
}

void basic_scheduler::fiber_proc(fibers::mutex & mtx)
{
    try {
        for (;;) {
        ///*
        try {
            bool stopping = false;
            task_entry * pe;
            {
                //LOG_TRACE(logger()) << "before acquire guard fiber " << this_fiber::get_id() << ", thread: " << this_thread::get_id();
                //auto guard = make_lock_guard(mtx);
                //LOG_TRACE(logger()) << "acquire guard fiber " << this_fiber::get_id() << ", thread: " << this_thread::get_id();
                unique_lock lck(queue_mtx_);
                //LOG_TRACE(logger()) << "got guard fiber " << this_fiber::get_id() << ", thread: " << this_thread::get_id();
                if (stopping_ && queue_not_safe_empty()) {
                    //LOG_TRACE(logger()) << "return00 from fiber " << this_fiber::get_id() << ", thread: " << this_thread::get_id();
                    return;
                }
                queue_cond_.wait(lck, [this]() { return !queue_not_safe_empty() || stopping_; });
                if (stopping_ && queue_not_safe_empty()) {
                    //LOG_TRACE(logger()) << "return0 from fiber " << this_fiber::get_id() << ", thread: " << this_thread::get_id();
                    return;
                }
                stopping = stopping_;
                pe = queue_not_safe_pop_next();
            }
            SCOPE_EXIT([this, pe] { release_task_ref(pe); });
            if (BOOST_LIKELY(!stopping)) {
                pe->run();
            } else {
                pe->cancel();
            }
        } catch (...) {
            LOG_ERROR(logger()) << boost::current_exception_diagnostic_information();
        }
        //*/
        //this_thread::sleep(boost::chrono::milliseconds(100));
    }
    } catch (...) {
        LOG_ERROR(logger()) << boost::current_exception_diagnostic_information();
    }
    LOG_TRACE(logger()) << "return1 from fiber " << this_fiber::get_id() << ", thread: " << this_thread::get_id();
}

template <class ... ArgsT>
task_handle_ptr basic_scheduler::do_post(bool wh, ArgsT && ... args)
{
    task_handle_ptr result;
    task_entry * pe = task_pool_.new_object(std::forward<ArgsT>(args) ...);
    if (wh) {
        try {
            result.reset(handle_pool_.new_object(shared_from_this(), pe), false);
        } catch (...) {
            release_task_ref(pe);
            throw;
        }
    }
    {
        lock_guard guard(queue_mtx_);
        entries_.push_front(*pe);
    }
    queue_cond_.notify_one();
    return std::move(result);
}

task_handle_ptr basic_scheduler::post(function<void()> const& task, bool wh)
{
    return do_post(wh, in_place_type<function_call_scheduler_task<function<void()>>>, task);
}

task_handle_ptr basic_scheduler::post(scheduler_task_t && task, bool wh)
{
    return do_post(wh, std::move(task));
}

void basic_scheduler::release_task_ref(task_entry * te)
{
    te->release_ref(task_pool_);
}

void basic_scheduler::free_task_handle(basic_task_handle * th)
{
    handle_pool_.delete_object(th);
}

bool basic_scheduler::queue_not_safe_empty() const
{
    return entries_.empty();
}

task_entry * basic_scheduler::queue_not_safe_pop_next()
{
    task_entry * pe = &entries_.back();
    entries_.pop_back();
    return pe;
}

bool basic_scheduler::unlink_and_cancel(scheduler_detail::task_entry * te)
{
    if (lock_guard guard(queue_mtx_); BOOST_UNLIKELY(te->is_linked())) {
        entries_.erase(entry_list_t::s_iterator_to(*te));
    } else {
        return false;
    }
    return te->cancel();
}

}
