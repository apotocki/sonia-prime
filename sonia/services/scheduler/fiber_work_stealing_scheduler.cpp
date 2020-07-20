//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include "sonia/fibers/type.hpp"
#include <boost/context/detail/prefetch.hpp>
#include <iostream>

#include "fiber_work_stealing_scheduler.hpp"

#include "sonia/logger/logger.hpp"

namespace sonia {

fibers::context * fiber_work_stealing_scheduler::group_host::steal(fiber_work_stealing_scheduler * exc)
{
    int count = (int)schedulers.size();
    lock_guard lock(mtx);
    do {
        size_t idx = ++victim_sched_idx % schedulers.size();
        fiber_work_stealing_scheduler * victim_sched = schedulers[idx].get();
        if (victim_sched != exc) {
            fibers::context * v = victim_sched->steal();
            if (v) return v;
        }
        --count;
    } while (count > 0);
    return nullptr;
}

void fiber_work_stealing_scheduler::group_host::remove(fiber_work_stealing_scheduler * s)
{
    lock_guard lock(mtx);
    for (auto it = schedulers.begin(), eit = schedulers.end(); it != eit; ++it) {
        if (it->get() == s) {
            schedulers.erase(it);
            return;
        }
    }
}

fiber_work_stealing_scheduler::fiber_work_stealing_scheduler(group_host & g, bool suspend)
    : group_(g), flag_(0), waiting_{0}, suspend_(suspend ? 1 : 0)
{
    lock_guard guard(group_.mtx);
    group_.schedulers.emplace_back(this);
}

fibers::context * fiber_work_stealing_scheduler::steal() noexcept
{
    return rqueue_.take_if([](fibers::context * c) { return !c->is_context(fibers::type::pinned_context); });
}

void fiber_work_stealing_scheduler::awakened(fibers::context* ctx) noexcept
{
    if (!ctx->is_context(fibers::type::pinned_context)) {
        //GLOBAL_LOG_TRACE() << "push fiber " << ctx->get_id() << ", thread: " << this_thread::get_id();
        ctx->detach();
        rqueue_.push(ctx);
        group_.notify_one();
    } else {
    //else {
    //  GLOBAL_LOG_TRACE() << "push pinned fiber " << ctx->get_id() << ", thread: " << this_thread::get_id();
    //}

        rqueue_.push(ctx);
    }
}

fibers::context * fiber_work_stealing_scheduler::pick_next() noexcept
{
    //GLOBAL_LOG_INFO() << "PICK_NEXT " << this_thread::get_id();
    fibers::context * victim = rqueue_.pop();
    if (victim) {
        boost::context::detail::prefetch_range(victim, sizeof(fibers::context));
        if (!victim->is_context(fibers::type::pinned_context)) {
            fibers::context::active()->attach(victim);
        }
    } else {
        //GLOBAL_LOG_INFO() << "STEALING NEXT " << this_thread::get_id();
        victim = group_.steal(this);
        if (victim) {
            boost::context::detail::prefetch_range(victim, sizeof(fibers::context));
            BOOST_ASSERT(!victim->is_context(fibers::type::pinned_context));
            fibers::context::active()->attach(victim);
        }
    }
    if (victim && !victim->is_context(fibers::type::pinned_context)) {
        //GLOBAL_LOG_TRACE() << "pick fiber " << victim->get_id();
    } else if (victim) {
        //GLOBAL_LOG_TRACE() << "pick pinned fiber " << victim->get_id();
    }
    return victim;
}

void fiber_work_stealing_scheduler::suspend_until(std::chrono::steady_clock::time_point const& tp) noexcept
{
    if ( suspend_) {
        if ((std::chrono::steady_clock::time_point::max)() == tp) {
            unique_lock lk(mtx_);
            ++group_.sleeping_cnt;
            waiting_ = 1;
            cnd_.wait( lk, [this](){ return !!flag_; });
            //GLOBAL_LOG_INFO() << "UNLOCK " << this_thread::get_id();
            waiting_ = flag_ = 0;
            --group_.sleeping_cnt;
        } else {
            unique_lock lk(mtx_);
            ++group_.sleeping_cnt;
            waiting_ = 1;
            cnd_.wait_until(lk, tp, [this](){ return !!flag_; });
            waiting_ = flag_ = 0;
            --group_.sleeping_cnt;
        }
    }
}

void fiber_work_stealing_scheduler::notify() noexcept
{
    try {

    if (BOOST_UNLIKELY(!suspend_)) return;

    mtx_.lock();
    flag_ = 1;
    if (waiting_) {
        mtx_.unlock();
        cnd_.notify_all();
    } else {
        mtx_.unlock();
        group_.notify_one();
    }
    

    } catch (...) {
        auto str = boost::current_exception_diagnostic_information();
        std::cout << str;
    }
}

bool fiber_work_stealing_scheduler::try_notify() noexcept
{
    try {
        if (unique_lock lck(mtx_); waiting_) {
            flag_ = 1;
        } else {
            return false;
        }
        cnd_.notify_all();
        return true;
    } catch (...) {
        auto str = boost::current_exception_diagnostic_information();
        std::cout << str;
        return false;
    }
}

void fiber_work_stealing_scheduler::group_host::notify_one()
{
    if (!sleeping_cnt.load()) return;
    lock_guard guard(mtx);
    for (auto & sched : schedulers) {
        if (sched->try_notify()) return;
    }
}
//////////////////////////////////
fiber_work_stealing_scheduler2::fiber_work_stealing_scheduler2(group_host & g, bool suspend)
    : group_(g), flag_(0), suspend_(suspend ? 1 : 0)
{
    lock_guard guard(group_.mtx);
    group_.schedulers.emplace_back(this);
}

void fiber_work_stealing_scheduler2::awakened(fibers::context* ctx) noexcept
{
    if (!ctx->is_context(fibers::type::pinned_context)) {
        ctx->detach();
        group_.rqueue_.push(ctx);
    } else {
        rqueue_.push(ctx);
    }
}

fibers::context * fiber_work_stealing_scheduler2::pick_next() noexcept
{
    fibers::context * victim = rqueue_.pop();
    if (victim) {
        boost::context::detail::prefetch_range(victim, sizeof(fibers::context));
        BOOST_ASSERT(victim->is_context(fibers::type::pinned_context));
    } else {
        victim = group_.rqueue_.pop();
        if (victim) {
            boost::context::detail::prefetch_range(victim, sizeof(fibers::context));
            BOOST_ASSERT(!victim->is_context(fibers::type::pinned_context));
            fibers::context::active()->attach(victim);
        }
    }
    return victim;
}

void fiber_work_stealing_scheduler2::suspend_until(std::chrono::steady_clock::time_point const& tp) noexcept
{
    if ( suspend_) {
        if ((std::chrono::steady_clock::time_point::max)() == tp) {
            unique_lock lk(mtx_);
            cnd_.wait( lk, [this](){ return !!flag_; });
            flag_ = 0;
        } else {
            unique_lock lk(mtx_);
            cnd_.wait_until(lk, tp, [this](){ return !!flag_; });
            flag_ = 0;
        }
    }
}

void fiber_work_stealing_scheduler2::notify() noexcept
{
    if (suspend_) {
        mtx_.lock();
        flag_ = 1;
        mtx_.unlock();
        cnd_.notify_all();
    }
}

}
