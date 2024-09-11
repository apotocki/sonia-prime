//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>
#include <chrono>

#include <boost/variant.hpp>

#include "sonia/function.hpp"
#include "sonia/logger/logger.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"

#ifndef SONIA_SCHEDULER_TASK_SZ
#   define SONIA_SCHEDULER_TASK_SZ (std::max)((size_t)64, sizeof(function<void()>) + 2 * sizeof(void*))
#endif

namespace sonia::scheduler_detail {

using time_point_t = std::chrono::time_point<std::chrono::system_clock>;
using time_duration_t = std::chrono::milliseconds;
using when_t = boost::variant<null_t, time_duration_t, time_point_t>;

class scheduler_task_handle_impl : public polymorphic_movable 
{
public:
    virtual bool cancel(uintptr_t cookie) = 0; // returns true if cancelled
    virtual void reschedule(uintptr_t cookie, when_t when, time_duration_t repeat_interval) = 0;

    virtual void destroy(uintptr_t cookie) = 0;
};

}

namespace sonia {

class NOVTABLE scheduler_task : public polymorphic_clonable, public polymorphic_movable
{
public:
    virtual void on_cancel() noexcept {}
    virtual void run() noexcept = 0;
};

template <typename DerivedT>
class scheduler_task_adapter : public scheduler_task
{
public:
    void* move(void* address, size_t sz) override
    {
        BOOST_ASSERT(sz >= sizeof(DerivedT));
        return new (address) DerivedT(std::move(static_cast<DerivedT&>(*this)));
    }

    void* clone(void* address, size_t sz) const override
    {
        BOOST_ASSERT(sz >= sizeof(DerivedT));
        return new (address) DerivedT(static_cast<DerivedT const&>(*this));
    }
};

using scheduler_task_t = automatic_polymorphic<scheduler_task, SONIA_SCHEDULER_TASK_SZ>;

class scheduler_task_handle
{
    automatic_polymorphic<scheduler_detail::scheduler_task_handle_impl, 2 * sizeof(void*)> impl_;
    uintptr_t cookie_;

public:
    using time_duration_t = scheduler_detail::time_duration_t;
    using when_t = scheduler_detail::when_t;

    scheduler_task_handle() : cookie_{0} {}

    template <typename T, class ... ArgsT>
    scheduler_task_handle(uintptr_t cookie, in_place_type_t<T> ipt, ArgsT&& ... args)
        : impl_{ipt, std::forward<ArgsT>(args) ...}, cookie_{cookie}
    {}

    ~scheduler_task_handle()
    {
        if (cookie_) impl_->destroy(cookie_);
    }

    scheduler_task_handle(scheduler_task_handle const&) = delete;
    scheduler_task_handle(scheduler_task_handle && rhs) : impl_{std::move(rhs.impl_)}, cookie_{rhs.cookie_}
    {
        rhs.cookie_ = 0;
    }
    scheduler_task_handle& operator= (scheduler_task_handle const&) = delete;
    scheduler_task_handle& operator= (scheduler_task_handle && rhs)
    {
        if (this != &rhs) {
            if (cookie_) impl_->destroy(cookie_);
            cookie_ = 0;
            impl_ = std::move(rhs.impl_);
            cookie_ = rhs.cookie_;
            rhs.cookie_ = 0;
        }
        return *this;
    }

    bool cancel() { if (cookie_) return impl_->cancel(cookie_); else return true; } // returns true if cancelled
    void reschedule(when_t when = null_t{}, time_duration_t repeat_interval = time_duration_t{0}) { if (cookie_) impl_->reschedule(cookie_, when, repeat_interval); }
};

template <typename FunctionT>
class function_call_scheduler_task : public scheduler_task
{
public:
    template <typename ArgT>
    requires(!is_same_v<function_call_scheduler_task, remove_cvref_t<ArgT>>)
    explicit function_call_scheduler_task(ArgT && arg) 
        : func_{std::forward<ArgT>(arg)}
    {}

    function_call_scheduler_task(function_call_scheduler_task const&) = default;
    function_call_scheduler_task(function_call_scheduler_task &&) = default;
    function_call_scheduler_task & operator= (function_call_scheduler_task const&) = default;
    function_call_scheduler_task & operator= (function_call_scheduler_task &&) = default;

    void run() noexcept override
    {
        try {
            func_();
        } catch (...) {
            GLOBAL_LOG_ERROR() << boost::current_exception_diagnostic_information();
        }
    }

    void* move(void* address, size_t sz) override
    {
        BOOST_ASSERT(sz >= sizeof(function_call_scheduler_task));
        return new (address) function_call_scheduler_task(std::move(*this));
    }

    void* clone(void* address, size_t sz) const override
    {
        BOOST_ASSERT(sz >= sizeof(function_call_scheduler_task));
        return new (address) function_call_scheduler_task(*this);
    }

private:
    FunctionT func_;
};

template <typename FuncT>
scheduler_task_t make_scheduler_task(FuncT && func)
{
    return scheduler_task_t(
        in_place_type_t<function_call_scheduler_task<remove_cvref_t<FuncT>>>(),
        std::forward<FuncT>(func)
    );
}

class scheduler
{
public:
    virtual ~scheduler() = default;

    using time_duration_t = scheduler_detail::time_duration_t;
    using when_t = scheduler_detail::when_t;

    virtual scheduler_task_handle post(scheduler_task_t &&, when_t when = null_t{}, time_duration_t repeat_interval = time_duration_t{0}) = 0;

    scheduler_task_handle post(function<void()> const& f, when_t when = null_t{}, time_duration_t repeat_interval = time_duration_t{0})
    {
        return post(make_scheduler_task(f), when, repeat_interval);
    }
};

}
