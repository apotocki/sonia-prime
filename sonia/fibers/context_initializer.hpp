//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "sonia/prime_config.hpp"
#include "sonia/fibers/context.hpp"

namespace sonia::fibers {

// schwarz counter
struct context_initializer {
    struct context_descriptor {
        context * active_;
        context * main_;
    };
    static thread_local context_descriptor context_descriptor_;

    //static thread_local context *   active_;
    //static thread_local std::size_t counter_;

    static BOOST_NOINLINE SONIA_PRIME_API context_descriptor* get_descriptor() noexcept;

    static context** get_pactive() noexcept {
        return &get_descriptor()->active_;
    }

    //static BOOST_NOINLINE context** get_pactive() noexcept {
    //    return &active_;
    //}

    context_initializer() = default;

    //void init() {
    //    //if ( 0 == counter_++) {
    //        // main fiber context of this thread
    //        context * main_ctx = new main_context{};
    //        // scheduler of this thread
    //        auto sched = new scheduler{};
    //        // attach main context to scheduler
    //        sched->attach_main_context( main_ctx);
    //        // create and attach dispatcher context to scheduler
    //        sched->attach_dispatcher_context( make_dispatcher_context() );
    //        // make main context to active context
    //        *get_pactive() = main_ctx;
    //    //}
    //}

    ~context_initializer();
};

}
