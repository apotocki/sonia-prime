//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_SOCKETS_HPP
#define SONIA_IO_SOCKETS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"
#include "sonia/sal/net.hpp"
#include "sonia/utility/shared_handle.hpp"

namespace sonia { namespace io {

struct socket_handle_traits
{
    using type = sonia::sal::socket_type;
    static constexpr type not_initialized_v = sonia::sal::not_initialized_socket_v;

    struct base_type { virtual ~base_type() {} };

    template <typename ServiceT>
    static void close(ServiceT * s, type h) noexcept
    {
        if (not_initialized_v != h) {
            sonia::sal::close_socket(h);
        }
    }

    template <typename ServiceT, typename HandleT>
    static void free(ServiceT * s, HandleT h) noexcept
    {
        if (h) {
            s->free_handle(identity<ServiceT>(), h);
        }
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
    static bool close(ServiceT * s, HandleT * h) noexcept
    {
        auto * lh = h ? h->lock() : nullptr;
        if (lh) {
            socket_handle_traits::close(s, lh->handle);
            lh->handle = socket_handle_traits::not_initialized_v;
            h->release(s);
            return true;
        }
        return false;
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

template <class TraitsT> class tcp_socket_adapter;
template <class TraitsT> class tcp_socket_service;
template <class TraitsT> class tcp_socket_factory;

using tcp_socket = tcp_socket_adapter<socket_traits>;
using tcp_weak_socket = tcp_socket_adapter<socket_traits::weak_traits_type>;
using tcp_socket_service_type = tcp_socket_service<socket_traits>;
using tcp_socket_factory_type = tcp_socket_factory<socket_traits>;

template <class TraitsT> class udp_socket_adapter;
template <class TraitsT> class udp_socket_service;
template <class TraitsT> class udp_socket_factory;

using udp_socket = udp_socket_adapter<socket_traits>;
using udp_weak_socket = udp_socket_adapter<socket_traits::weak_traits_type>;
using udp_socket_service_type = udp_socket_service<socket_traits>;
using udp_socket_factory_type = udp_socket_factory<socket_traits>;

}}

#endif // SONIA_IO_SOCKETS_HPP
