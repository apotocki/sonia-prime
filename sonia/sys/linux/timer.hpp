//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SYS_LINUX_TIMER_HPP
#define SONIA_SYS_LINUX_TIMER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <chrono>
#include "sonia/function.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/sys/linux/signals.hpp"
#include "sonia/sys/linux/signals_api.hpp"

namespace sonia::linux {

class timer
{
    class timer_descriptor 
        : public handler_entry
        , public enable_shared_from_this<timer_descriptor>
    {
    public:
        explicit timer_descriptor(function<void()> const&);

        void set(std::chrono::milliseconds);
        void disarm();

        void create(bool realtime);
        void release();

    private:
        timer_t timerid_;
        intptr_t hid_{0};
    };

public:
    explicit timer(function<void()> const& h, bool realtime = true);
    ~timer();

    timer(timer const&) = delete;
    timer(timer &&) = default;
    timer& operator=(timer const&) = delete;
    timer& operator=(timer && rhs)
    {
        if (descr_ != rhs.descr_) {
            if (descr_) descr_->release();
            descr_ = std::move(rhs.descr_);
        }
        return *this;
    }

    void set(std::chrono::milliseconds ms) { descr_->set(ms); }
    void disarm() { descr_->disarm(); }

private:
    shared_ptr<timer_descriptor> descr_;
};

}

#endif // SONIA_SYS_LINUX_TIMER_HPP
