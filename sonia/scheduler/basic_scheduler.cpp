//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sal.hpp"
#include "basic_scheduler.hpp"

#include <boost/fiber/all.hpp>

namespace sonia {

basic_scheduler::basic_scheduler(uint32_t thr_cnt, uint32_t fb_cnt)
    : thr_cnt_(thr_cnt), fb_cnt_(fb_cnt), stopping_(true)
{

}

basic_scheduler::~basic_scheduler() {

}

std::string basic_scheduler::thread_name() const {
    return "scheduler thread";
}

void basic_scheduler::start() {
    bool exp = true;
    if (stopping_.compare_exchange_strong(exp, false)) {
        threads_.reserve(thr_cnt_);
        //fibers::use_scheduling_algorithm<fibers::algo::work_stealing>(thr_cnt_);
        for (size_t t = 0; t < thr_cnt_; ++t) {
            threads_.push_back(thread([this]() { thread_proc(); }));
            sal::set_thread_name(threads_.back().get_id(), ("%1% #%2%"_fmt % thread_name() % t).str());
        }
    }
}

void basic_scheduler::stop() {
    bool exp = false;
    if (stopping_.compare_exchange_strong(exp, true)) {
        for (auto & t : threads_) {
            t.interrupt();
        }

        queue_cond_.notify_all();

        for (auto & t : threads_) {
            t.join();
        }

        threads_.clear();
    }
}

void basic_scheduler::thread_proc() {
    fibers::use_scheduling_algorithm<fibers::algo::work_stealing>(thr_cnt_);
    //fibers::use_scheduling_algorithm<fibers::algo::shared_work>();// thr_cnt_);

    fibers::mutex mtx;
    std::vector<fiber> fibers;
    fibers.reserve(fb_cnt_ + 1);
    for (size_t f = 0; f < fb_cnt_; ++f) {
        fibers.push_back(boost::fibers::fiber([this, &mtx]() { fiber_proc(mtx); }));
    }

    fiber_proc(mtx);

    for (fiber & f : fibers) {
        f.join();
    }
}

void basic_scheduler::fiber_proc(fibers::mutex & mtx) {
    try {
    while (!stopping_.load()) {
        ///*
        try {
            {
                auto guard = make_lock_guard(mtx);
                auto lck = make_unique_lock(queue_mtx_);
                queue_cond_.wait(lck, [this]() { return !queue_.empty() || stopping_.load(); });
                if (stopping_.load()) return;
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
}

void basic_scheduler::post(scheduler_task_t && task) {

}

}
