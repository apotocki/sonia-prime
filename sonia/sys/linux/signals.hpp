//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILTIY_POSIX_SIGNALS_HPP
#define SONIA_UTILTIY_POSIX_SIGNALS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/prime_config.hpp"
#include "sonia/function.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/concurrency.hpp"

namespace sonia::linux {

using user_handler_type = function<void()>;

class handler_entry
{
public:
    user_handler_type handler;
    mutex mtx;

    explicit handler_entry(user_handler_type const& f) : handler(f) {}
    virtual ~handler_entry() = default;
};


SONIA_PRIME_API int  get_user_signal();
SONIA_PRIME_API void set_user_signal(int);
SONIA_PRIME_API void set_interruption_handler(user_handler_type const&);
SONIA_PRIME_API void raise_user_signal(user_handler_type const&);

SONIA_PRIME_API void run_watchers(int count = 1);
SONIA_PRIME_API void stop_watchers() noexcept;

SONIA_PRIME_API intptr_t register_handler(shared_ptr<handler_entry>);
SONIA_PRIME_API void unregister_handler(intptr_t);

//SONIA_PRIME_API void raise_user_signal();

//static void* signal_thread_func(void *p);

}

#endif // SONIA_UTILTIY_POSIX_SIGNALS_HPP