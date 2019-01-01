//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SCHEDULER_HPP
#define SONIA_SCHEDULER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include "sonia/utility/automatic_polymorphic.hpp"

#ifndef SONIA_SCHEDULER_TASK_SZ
#   define SONIA_SCHEDULER_TASK_SZ 64
#endif

namespace sonia {

class scheduler_task : public polymorphic_movable {
public:
    virtual ~scheduler_task() {}
    virtual void cancel() {}
    virtual void run() = 0;
};

typedef automatic_polymorphic<scheduler_task, SONIA_SCHEDULER_TASK_SZ> scheduler_task_t;

template <typename FunctionT>
class function_call_scheduler_task : public scheduler_task
{
public:
    template <typename ArgT>
    explicit function_call_scheduler_task(ArgT && arg) 
        : func_(std::forward<ArgT>(arg))
    {}

    function_call_scheduler_task(function_call_scheduler_task const&) = delete;
    function_call_scheduler_task(function_call_scheduler_task &&) = default;
    function_call_scheduler_task & operator= (function_call_scheduler_task const&) = delete;
    function_call_scheduler_task & operator= (function_call_scheduler_task &&) = default;

    void run() override {
        func_();
    }

private:
    FunctionT func_;
};

template <typename FuncT>
scheduler_task_t make_scheduler_task(FuncT && func) {
    return scheduler_task_t(
        in_place_type_t<function_call_scheduler_task<remove_cvref_t<FuncT>>>(),
        std::forward<FuncT>(func)
    );
}

class scheduler {
public:
    virtual ~scheduler() {}

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void post(scheduler_task_t &&) = 0;

    template <typename FuncT>
    void post(FuncT && func) {
        post(make_scheduler_task(std::forward<FuncT>(func)));
    }
};

}

#endif // SONIA_SCHEDULER_HPP
