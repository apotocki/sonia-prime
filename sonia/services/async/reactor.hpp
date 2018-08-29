//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_ASYNC_REACTOR_HPP
#define SONIA_SERVICES_ASYNC_REACTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/thread.hpp"

namespace sonia { namespace services { namespace async {

class reactor final {
public:
    explicit reactor(uint32_t thr_cnt);

    ~reactor();

private:
    void thread_proc();
    std::vector<thread> threads_;

    void * handle_;
};

}}}

#endif // SONIA_SERVICES_ASYNC_REACTOR_HPP
