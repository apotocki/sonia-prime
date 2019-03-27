//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_SOCKETS_HPP
#define SONIA_IO_SOCKETS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/sal/net.hpp"
#include "sonia/utility/shared_handle.hpp"
#include "smart_handle_facade.hpp"

namespace sonia::io {

struct socket_handle_traits
{
    using type = sonia::sal::socket_handle;
    static constexpr type not_initialized_v = sonia::sal::not_initialized_socket_v;

    struct base_type { virtual ~base_type() = default; };

    template <typename ServiceT, typename HandleT>
    static void close(ServiceT * s, HandleT h) noexcept
    {
        s->release_handle(identity<ServiceT>(), h);
    }

    template <typename ServiceT, typename HandleT>
    static void free(ServiceT * s, HandleT h) noexcept
    {
       s->free_handle(identity<ServiceT>(), h);
    }
};

struct socket_traits;
struct weak_socket_traits;

struct socket_traits_base
{
    using strong_traits_type = socket_traits;
    using weak_traits_type = weak_socket_traits;
    using handle_type = shared_handle<socket_handle_traits> *;
    static constexpr handle_type not_initialized_v = nullptr;

    template <typename ServiceT, typename HandleT>
    static void close(ServiceT * s, HandleT * h) noexcept
    {
        s->close_handle(h);
    }
};

struct weak_socket_traits : socket_traits_base
{
    static void add_ref(handle_type h) noexcept { h->add_weakref(); }
    
    template <typename ServiceT>
    static void release(ServiceT * s, handle_type h) noexcept
    {
        if (h) h->release_weak(s);
    }
};

struct socket_traits : socket_traits_base
{
    static void add_ref(handle_type h) noexcept { if (h) h->add_ref(); }

    template <typename ServiceT>
    static void release(ServiceT * s, handle_type h) noexcept
    {
        if (h) h->release(s);
    }
};

template <class DerivedT, class TraitsT> class tcp_socket_adapter;
template <class TraitsT> class tcp_socket_service;
template <class TraitsT> class tcp_socket_factory;

using tcp_socket = smart_handle_facade<socket_traits, tcp_socket_adapter>;
using tcp_weak_socket = smart_handle_facade<socket_traits::weak_traits_type, tcp_socket_adapter>;
using tcp_socket_service_type = tcp_socket_service<socket_traits>;
using tcp_socket_factory_type = tcp_socket_factory<socket_traits>;

template <class DerivedT, class TraitsT> class udp_socket_adapter;
template <class TraitsT> class udp_socket_service;
template <class TraitsT> class udp_socket_factory;

using udp_socket = smart_handle_facade< socket_traits, udp_socket_adapter>;
using udp_weak_socket = smart_handle_facade<socket_traits::weak_traits_type, udp_socket_adapter>;
using udp_socket_service_type = udp_socket_service<socket_traits>;
using udp_socket_factory_type = udp_socket_factory<socket_traits>;

}

#endif // SONIA_IO_SOCKETS_HPP
