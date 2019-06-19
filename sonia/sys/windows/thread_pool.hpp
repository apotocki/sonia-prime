//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <chrono>

//#include "sonia/prime_config.hpp"
#include "sonia/function.hpp"
#include "sonia/shared_ptr.hpp"

#include "thread_pool_api.hpp"

namespace sonia::windows {

//using user_handler_type = function<void()>;

//SONIA_PRIME_API void setup_thread_pool(size_t min_thread_count = 1, size_t max_thread_count = 0); // max_thread_count = 0 means max_thread_count = the number of cores
//SONIA_PRIME_API void close_thread_pool();

class threadpool;

class threadpool_timer
{
public:
    void set(std::chrono::milliseconds);
    void set(std::chrono::milliseconds, function<void()> const&);
    void disarm();

    explicit threadpool_timer(shared_ptr<threadpool> pool);
    threadpool_timer(shared_ptr<threadpool> pool, function<void()> const&);
    ~threadpool_timer();

private:
    static void proc(PTP_CALLBACK_INSTANCE, PVOID, PTP_TIMER);

    shared_ptr<threadpool> pool_;

    PTP_TIMER pti_;
    function<void()> func_;
    bool is_set_{false};
};

class threadpool : public enable_shared_from_this<threadpool>
{
    friend class threadpool_timer;

public:
    explicit threadpool(size_t min_thread_count = 1, size_t max_thread_count = 0);
    
    threadpool(threadpool const&) = delete;
    threadpool(threadpool &&) = delete;
    threadpool& operator= (threadpool const&) = delete;
    threadpool& operator= (threadpool &&) = delete;

    ~threadpool();

    void set_cancel_pending_callbacks_on_exit(bool val) { cancel_pendings_ = val; }

    std::unique_ptr<threadpool_timer> create_timer();
    std::unique_ptr<threadpool_timer> create_timer(function<void()> const&);

private:
    PTP_POOL tpool_{nullptr};
    PTP_CLEANUP_GROUP tpool_cleanup_group_{nullptr};
    TP_CALLBACK_ENVIRON tpool_callback_env_;
    bool cancel_pendings_{false};
};

}

