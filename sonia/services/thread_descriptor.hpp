//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_THREAD_DESCRIPTOR_HPP
#define SONIA_SERVICES_THREAD_DESCRIPTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/prime_config.hpp"

#include "host_impl.hpp"

namespace sonia { namespace services {

class thread_descriptor {
public:
    virtual ~thread_descriptor() {}
    shared_ptr<host_impl> host;

    explicit thread_descriptor(shared_ptr<host_impl> h) : host{std::move(h)} {}

    thread_descriptor() = delete;
    thread_descriptor(thread_descriptor const&) = delete;
    thread_descriptor& operator=(thread_descriptor const&) = delete;

    SONIA_PRIME_API static void reset();
    SONIA_PRIME_API void set();
};

}}

#endif // SONIA_SERVICES_THREAD_DESCRIPTOR_HPP
