//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <chrono>

#include "sonia/function.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/sys/macos/dispatch.hpp"

namespace sonia::macos {

class timer
{
public:
    timer() = default;

    explicit timer(function<void()> const& h, bool realtime = true)
    	: descr_(make_shared<timer_descriptor>(h))
    {
    	descr_->create(realtime);
    }

    ~timer() noexcept { if (descr_) descr_->release(); }

    timer(timer const&) = delete;
    timer(timer && rhs) = default;

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
