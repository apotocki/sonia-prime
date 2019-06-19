//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "main_api.hpp"
#include "thread_pool.hpp"

#include "boost/thread/thread.hpp"



#include "sonia/utility/scope_exit.hpp"
#include "sonia/logger/logger.hpp"

namespace sonia::windows {

threadpool::threadpool(size_t min_thread_count, size_t max_thread_count)
{
    if (!max_thread_count) {
        max_thread_count = boost::thread::hardware_concurrency();
    }

    bool init_completed = false;
    SCOPE_EXIT([this, &init_completed]() {
        if (!init_completed) {
            if (tpool_cleanup_group_) close_threadpool_cleanup_group(tpool_cleanup_group_);
            if (tpool_) close_threadpool(tpool_);
            destroy_threadpool_environment(&tpool_callback_env_);
        }
    });

    initialize_threadpool_environment(&tpool_callback_env_);

    tpool_ = create_threadpool();
    set_threadpool_thread_minimum(tpool_, min_thread_count);
    set_threadpool_thread_maximum(tpool_, max_thread_count);

    set_threadpool_callback_pool(&tpool_callback_env_, tpool_);

    tpool_cleanup_group_ = create_threadpool_cleanup_group();
    set_threadpool_callback_cleanup_group(&tpool_callback_env_, tpool_cleanup_group_, nullptr);

    init_completed = true;
}

threadpool::~threadpool()
{
    close_threadpool_cleanup_group_members(tpool_cleanup_group_, cancel_pendings_, NULL);
    close_threadpool_cleanup_group(tpool_cleanup_group_);
    close_threadpool(tpool_);
    destroy_threadpool_environment(&tpool_callback_env_);
}

std::unique_ptr<threadpool_timer> threadpool::create_timer()
{
    return std::make_unique<threadpool_timer>(shared_from_this());
}

std::unique_ptr<threadpool_timer> threadpool::create_timer(function<void()> const& f)
{
    return std::make_unique<threadpool_timer>(shared_from_this(), f);
}

threadpool_timer::threadpool_timer(shared_ptr<threadpool> pool)
    : pool_{std::move(pool)}
{
    pti_ = create_threadpool_timer(&threadpool_timer::proc, this, &pool_->tpool_callback_env_);
}

threadpool_timer::threadpool_timer(shared_ptr<threadpool> pool, function<void()> const& f)
    : pool_{std::move(pool)}, func_{f}
{
    pti_ = create_threadpool_timer(&threadpool_timer::proc, this, &pool_->tpool_callback_env_);
}


threadpool_timer::~threadpool_timer()
{
    set_threadpool_timer(pti_, NULL, 0, 0);
    wait_for_threadpool_timer_callbacks(pti_, true);
    close_threadpool_timer(pti_);
}

void threadpool_timer::proc(PTP_CALLBACK_INSTANCE, PVOID param, PTP_TIMER)
{
    threadpool_timer * ptpt = reinterpret_cast<threadpool_timer*>(param);
    try {
        ptpt->func_();
    } catch (...) {
        GLOBAL_LOG_ERROR() << "error during the timer proc: " << boost::current_exception_diagnostic_information();
    }
}

void threadpool_timer::set(std::chrono::milliseconds ms)
{
    ULARGE_INTEGER ultime;
    ultime.QuadPart = (LONGLONG) -(ms.count() * 10000);

    FILETIME ftime;
    ftime.dwHighDateTime = ultime.HighPart;
    ftime.dwLowDateTime  = ultime.LowPart;
    set_threadpool_timer(pti_, &ftime, 0, 0);

    is_set_ = true;
}

void threadpool_timer::set(std::chrono::milliseconds ms, function<void()> const& func)
{
    if (is_set_) {
        set_threadpool_timer(pti_, NULL, 0, 0);
        wait_for_threadpool_timer_callbacks(pti_, true);
    }
    func_ = func;
    set(ms);
}

void threadpool_timer::disarm()
{
    set_threadpool_timer(pti_, NULL, 0, 0);
    wait_for_threadpool_timer_callbacks(pti_, true);
    is_set_ = false;
}

}
