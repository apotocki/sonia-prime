//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <chrono>
#include <dispatch/dispatch.h>

#include "sonia/function.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/concurrency.hpp"

namespace sonia::macos {

void run_queue();
void stop_queue();

class timer_descriptor 
    : public enable_shared_from_this<timer_descriptor>
{
public:
    explicit timer_descriptor(function<void()> const& h);

    ~timer_descriptor();

    void set(std::chrono::milliseconds ms);
    void disarm();

    void create(bool realtime);
    void release();

private:
    dispatch_source_t timer_;
    function<void()> handler_;
    spin_mutex mtx_;
    bool disarmed_{true};
    bool skip_{true};
};

}
