//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sal.hpp"
#include "basic_scheduler.hpp"

#include <boost/fiber/all.hpp>

namespace sonia { namespace scheduler_detail {

void task_entry::release_ref(task_entry_pool_t * ppool) {
    if (1 == refs_.fetch_sub(1)) {
        ppool->delete_object(this);
    }
}

void task_entry::cancel() {
    bool exp = false;
    if (handled_.compare_exchange_strong(exp, true, std::memory_order::memory_order_relaxed)) {
        task_->cancel();
    }
}

void task_entry::run() {
    bool exp = false;
    if (handled_.compare_exchange_strong(exp, true, std::memory_order::memory_order_relaxed)) {
        task_->run();
    }
}

} // namespace sonia::scheduler_detail

basic_scheduler::basic_scheduler(uint32_t thr_cnt, uint32_t fb_cnt)
    : thr_cnt_(thr_cnt), fb_cnt_(fb_cnt), stopping_(true)
{

}

basic_scheduler::~basic_scheduler() {

}

std::string basic_scheduler::thread_name() const {
    return "scheduler thread";
}

void basic_scheduler::start()
{
    {
        auto lck = make_unique_lock(queue_mtx_);
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
        auto lck = make_unique_lock(queue_mtx_);
        if (stopping_) return;
        stopping_ = true;
        queue_cond_.notify_all();
    }

    for (auto & t : threads_) {
        t.join();
    }

    threads_.clear();
}



void basic_scheduler::thread_proc() {
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

void basic_scheduler::fiber_proc(fibers::mutex & mtx) {
    try {
        for (;;) {
        ///*
        try {
            {
                //LOG_TRACE(logger()) << "before acquire guard fiber " << this_fiber::get_id() << ", thread: " << this_thread::get_id();
                auto guard = make_lock_guard(mtx);
                //LOG_TRACE(logger()) << "acquire guard fiber " << this_fiber::get_id() << ", thread: " << this_thread::get_id();
                auto lck = make_unique_lock(queue_mtx_);
                //LOG_TRACE(logger()) << "got guard fiber " << this_fiber::get_id() << ", thread: " << this_thread::get_id();
                if (stopping_) {
                    //LOG_TRACE(logger()) << "return00 from fiber " << this_fiber::get_id() << ", thread: " << this_thread::get_id();
                    return;
                }
                queue_cond_.wait(lck, [this]() { return !queue_.empty() || stopping_; });
                if (stopping_) {
                    //LOG_TRACE(logger()) << "return0 from fiber " << this_fiber::get_id() << ", thread: " << this_thread::get_id();
                    return;
                }
            }
            function<void()> f = std::move(queue_.back());
            queue_.pop_back();
            f();
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

void basic_scheduler::post(scheduler_task_t && task) {

}

}
