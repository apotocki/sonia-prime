//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_BASIC_SCHEDULER_HPP
#define SONIA_BASIC_SCHEDULER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/fiber/all.hpp>

#include "sonia/thread.hpp"
#include "scheduler.hpp"

namespace sonia {

class basic_scheduler : public scheduler {
public:
    basic_scheduler(uint32_t thr_cnt, uint32_t fb_cnt);
    ~basic_scheduler() override;

    basic_scheduler(basic_scheduler const&) = delete;
    basic_scheduler(basic_scheduler &&) = delete;
    basic_scheduler& operator=(basic_scheduler const&) = delete;
    basic_scheduler& operator=(basic_scheduler &&) = delete;
    
    void start() override;
    void stop()  override;
    void post(scheduler_task_t &&) override;

private:
    virtual std::string thread_name() const;
    void thread_proc();
    void fiber_proc();

    uint32_t thr_cnt_, fb_cnt_;
    std::vector<thread> threads_;

    boost::fibers::mutex queue_mtx_;
};

}

#endif // SONIA_BASIC_SCHEDULER_HPP
