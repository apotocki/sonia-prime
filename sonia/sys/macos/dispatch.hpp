//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILTIY_MACOS_DISPATCH_HPP
#define SONIA_UTILTIY_MACOS_DISPATCH_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <chrono>
#include <dispatch/dispatch.h>

#include "sonia/function.hpp"
#include "sonia/shared_ptr.hpp"

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
    std::atomic<uint32_t> disarmed_cnt_{0};
};

}

#endif // SONIA_UTILTIY_MACOS_DISPATCH_HPP
