//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SCHEDULER_HPP
#define SONIA_SCHEDULER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include "sonia/function.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"

#ifndef SONIA_SCHEDULER_TASK_SZ
#   define SONIA_SCHEDULER_TASK_SZ (std::max)((size_t)64, sizeof(function<void()>) + sizeof(void*))
#endif

namespace sonia {

class scheduler_task : public polymorphic_movable
{
public:
    virtual void on_cancel() {}
    virtual void run() = 0;
};

using scheduler_task_t = automatic_polymorphic<scheduler_task, SONIA_SCHEDULER_TASK_SZ>;

class scheduler_task_handle
{
    virtual void add_ref() = 0;
    virtual void release_ref() = 0;

public:
    virtual ~scheduler_task_handle() {}

    virtual bool cancel() = 0; // returns true if cancelled

    friend void intrusive_ptr_add_ref(scheduler_task_handle * p)
    {
        p->add_ref();
    }

    friend void intrusive_ptr_release(scheduler_task_handle * p)
    {
        p->release_ref();
    }
};

using task_handle_ptr = boost::intrusive_ptr<scheduler_task_handle>;


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

    void run() override
    {
        func_();
    }

    polymorphic_movable* move(void* address, size_t sz) override
    {
        BOOST_ASSERT(sz >= sizeof(function_call_scheduler_task));
        new (address) function_call_scheduler_task(std::move(*this));
        return reinterpret_cast<function_call_scheduler_task*>(address);
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
    virtual ~scheduler() {}

    virtual task_handle_ptr post(scheduler_task_t &&, bool with_handle = true) = 0;
    virtual task_handle_ptr post(function<void()> const&, bool with_handle = true) = 0;
};

}

#endif // SONIA_SCHEDULER_HPP
