//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "timer.hpp"

#include "sonia/logger/logger.hpp"
#include "sonia/sys/linux/signals_api.hpp"

namespace sonia::linux {

timer::timer_descriptor::timer_descriptor(function<void()> const& h)
    : handler_entry{h}
{}

void timer::timer_descriptor::create(bool realtime)
{
    hid_ = register_handler(shared_from_this());
    sigevent sev;
#ifdef __ANDROID__
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = [](union sigval arg) {
        invoke_handler(arg.sival_ptr);
    };
    sev.sigev_notify_attributes = 0;
#else
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = get_user_signal();
#endif
    sev.sigev_value.sival_ptr = (void*)hid_;

    try {
        linux::timer_create(realtime ? CLOCK_REALTIME : CLOCK_MONOTONIC, &sev, &timerid_);
    } catch (...) {
        unregister_handler(hid_);
        hid_ = 0;
        throw;
    }
}

void timer::timer_descriptor::release()
{
    if (hid_) {
        GLOBAL_LOG_INFO() << "timer::timer_descriptor::release hid_ " << hid_;
        unregister_handler(hid_);
        linux::timer_delete(timerid_);
        hid_ = 0;
    }
    lock_guard guard(mtx);
    handler = std::nullptr_t{};
}

void timer::timer_descriptor::set(std::chrono::milliseconds ms)
{
    if (!hid_) return;

    int64_t ns = ms.count() * 1000000;
    
    itimerspec its;
    its.it_value.tv_sec = ns / 1000000000;
    its.it_value.tv_nsec = ns % 1000000000;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;

    linux::timer_settime(timerid_, 0, &its, NULL);
}

void timer::timer_descriptor::disarm()
{
    if (!hid_) return;

    itimerspec its;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    linux::timer_settime(timerid_, 0, &its, NULL);
}

timer::timer(function<void()> const& h, bool realtime)
{
    descr_ = make_shared<timer_descriptor>(h);
    descr_->create(realtime);
}

timer::~timer()
{
    if (descr_) {
        descr_->release();
    }
}

}
