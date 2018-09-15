//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sal.hpp"
#include "basic_scheduler.hpp"

#include <boost/fiber/all.hpp>

namespace sonia {

basic_scheduler::basic_scheduler(uint32_t thr_cnt, uint32_t fb_cnt)
    : thr_cnt_(thr_cnt), fb_cnt_(fb_cnt)
{
    start();
}

basic_scheduler::~basic_scheduler() {

}

std::string basic_scheduler::thread_name() const {
    return "scheduler thread";
}

void basic_scheduler::start() {
    if (threads_.empty()) return; // already started
    threads_.reserve(thr_cnt_);
    for (size_t t = 0; t < thr_cnt_; ++t) {
        threads_.push_back(thread([this]() { thread_proc(); }));
        sal::set_thread_name(threads_.back().get_id(), ("%1% #%2%"_fmt % thread_name() % t).str());
    }
}

void basic_scheduler::stop() {

}

void basic_scheduler::thread_proc() {
    boost::fibers::use_scheduling_algorithm<boost::fibers::algo::work_stealing>(thr_cnt_);

    std::vector<boost::fibers::fiber> fibers;
    for (size_t f = 0; f < fb_cnt_; ++f) {
        fibers.push_back(boost::fibers::fiber([this]() { fiber_proc(); }));
    }

    fiber_proc();
}

void basic_scheduler::fiber_proc() {
    unique_lock<boost::fibers::mutex> lck(queue_mtx_);

}

void basic_scheduler::post(scheduler_task_t && task) {

}

}