//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_THREAD_MANAGER_HPP
#define SONIA_SERVICES_THREAD_MANAGER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <atomic>
#include <boost/thread/barrier.hpp>

#include "sonia/cstdint.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/optional.hpp"
#include "sonia/string.hpp"
#include "sonia/logger/loggable.hpp"

#include "fiber_work_stealing_scheduler.hpp"

namespace sonia {

template <class DerivedT>
class thread_manager : public virtual loggable {
public:
    thread_manager(uint32_t thr_cnt, uint32_t fb_cnt);

    thread_manager(thread_manager const&) = delete;
    thread_manager(thread_manager &&) = delete;
    thread_manager& operator=(thread_manager const&) = delete;
    thread_manager& operator=(thread_manager &&) = delete;
    
    ~thread_manager() override;

    void start();
    void stop();
    //void add_fibers(size_t cnt)
    //void add_threads(size_t cnt)

    // derived's function prototypes
    std::string thread_name() const { return "scheduler thread"; }
    bool queue_empty() const { return true; }
    void handle_queue(unique_lock<fibers::mutex> & lck) {}
    //void on_fibers_suspend();

protected:
    DerivedT const& derived() const noexcept { return static_cast<DerivedT const&>(*this); }
    DerivedT & derived() noexcept { return static_cast<DerivedT &>(*this); }

    void thread_proc() noexcept;
    void fiber_proc() noexcept;

private:
    uint32_t thr_cnt_, fb_cnt_;
    std::vector<thread> threads_;
    optional<fibers::barrier> tbarrier_;
    fiber_work_stealing_scheduler::group_host gh_;
    fibers::mutex ctl_mtx_;

    fibers::mutex queue_mtx_;
    fibers::condition_variable_any queue_cond_;
    std::atomic<bool> stopping_{true};
};

}

#endif // SONIA_SERVICES_THREAD_MANAGER_HPP