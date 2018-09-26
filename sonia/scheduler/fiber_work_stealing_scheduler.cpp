//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include "boost/fiber/type.hpp"
#include <boost/context/detail/prefetch.hpp>

#include "fiber_work_stealing_scheduler.hpp"

//#include "sonia/logger/logger.hpp"

namespace sonia {

fibers::context * fiber_work_stealing_scheduler::group_host::steal(fiber_work_stealing_scheduler * exc) {
    int count = (int)schedulers.size();
    auto lock = make_lock_guard(mtx);
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

void fiber_work_stealing_scheduler::group_host::remove(fiber_work_stealing_scheduler * s) {
    auto lock = make_lock_guard(mtx);
    for (auto it = schedulers.begin(), eit = schedulers.end(); it != eit; ++it) {
        if (it->get() == s) {
            schedulers.erase(it);
            return;
        }
    }
}

fiber_work_stealing_scheduler::fiber_work_stealing_scheduler(group_host & g, bool suspend)
    : group_(g), flag_(0), suspend_(suspend ? 1 : 0)
{
    auto guard = make_lock_guard(group_.mtx);
    group_.schedulers.push_back(this);
}

fiber_work_stealing_scheduler::~fiber_work_stealing_scheduler() {

}

fibers::context * fiber_work_stealing_scheduler::steal() noexcept
{
    return rqueue_.take_if([](fibers::context * c) { return !c->is_context(fibers::type::pinned_context); });
}

void fiber_work_stealing_scheduler::awakened(fibers::context* ctx) noexcept {
    if (!ctx->is_context(fibers::type::pinned_context)) {
        //GLOBAL_LOG_TRACE() << "push fiber " << ctx->get_id() << ", thread: " << this_thread::get_id();
        ctx->detach();
    }
    //else {
    //  GLOBAL_LOG_TRACE() << "push pinned fiber " << ctx->get_id() << ", thread: " << this_thread::get_id();
    //}

    rqueue_.push(ctx);
}

fibers::context * fiber_work_stealing_scheduler::pick_next() noexcept {
    fibers::context * victim = rqueue_.pop();
    if (victim) {
        boost::context::detail::prefetch_range(victim, sizeof(fibers::context));
        if (!victim->is_context(fibers::type::pinned_context)) {
            fibers::context::active()->attach(victim);
        }
    } else {
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

void fiber_work_stealing_scheduler::suspend_until(std::chrono::steady_clock::time_point const& tp) noexcept {
    if ( suspend_) {
        if ((std::chrono::steady_clock::time_point::max)() == tp) {
            auto lk = make_unique_lock(mtx_);
            cnd_.wait( lk, [this](){ return !!flag_; });
            flag_ = 0;
        } else {
            auto lk = make_unique_lock(mtx_);
            cnd_.wait_until(lk, tp, [this](){ return !!flag_; });
            flag_ = 0;
        }
    }
}

void fiber_work_stealing_scheduler::notify() noexcept {
    if (suspend_) {
        auto lk = make_unique_lock(mtx_);
        flag_ = 1;
        lk.unlock();
        cnd_.notify_all();
    }
}

}
