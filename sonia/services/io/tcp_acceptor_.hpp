//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_TCP_ACCEPTOR_HPP
#define SONIA_IO_TCP_ACCEPTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include "tcp_socket.hpp"

namespace sonia { namespace io {

template <class TraitsT>
using tcp_connector_t = function<void(smart_handle_facade<TraitsT, tcp_socket_adapter>, size_t rsize)>;

template <class TraitsT>
using tcp_acceptor_handler_t = function<void(array_view<char>, tcp_connector_t<TraitsT> const&)>;

template <typename TraitsT>
class tcp_acceptor_service
{
protected:
    using acceptor_handle_type = typename TraitsT::handle_type;

public:
    virtual ~tcp_acceptor_service() {}

    //virtual std::pair<smart_handle_facade<TraitsT, tcp_socket_adapter>, size_t> tcp_acceptor_accept(acceptor_handle_type, char*, size_t) = 0;
    //virtual size_t tcp_acceptor_waiting_count(acceptor_handle_type) = 0;
    virtual void tcp_acceptor_on_close(acceptor_handle_type) noexcept = 0;
    virtual void free_handle(identity<tcp_acceptor_service>, acceptor_handle_type) noexcept = 0;
};

template <class DerivedT, class TraitsT>
class tcp_acceptor_adapter
{
public:
    using handle_type = typename TraitsT::handle_type;
    using service_type = tcp_acceptor_service<TraitsT>;
    //using socket_handle = smart_handle_facade<TraitsT, tcp_socket_adapter>;
    //
    //std::pair<socket_handle, size_t> accept(char * buff, size_t sz)
    //{
    //    return impl().tcp_acceptor_accept(handle(), buff, sz);
    //}

    size_t waiting_count() const
    {
        return impl().tcp_acceptor_waiting_count(handle());
    }

    void close() noexcept
    {
        if (TraitsT::close(&impl(), handle())) {
            impl().tcp_acceptor_on_close(handle());
        }
    }

private:
    service_type & impl() { return *static_cast<DerivedT*>(this)->impl_; }
    handle_type handle() const { return static_cast<DerivedT const*>(this)->handle_; }
};

class tcp_acceptor_access : smart_handle_access<tcp_acceptor_adapter>
{
public:
    template <typename TraitsT>
    static smart_handle_facade<TraitsT, tcp_acceptor_adapter> create_tcp_acceptor(
        shared_ptr<tcp_acceptor_service<TraitsT>> impl,
        typename TraitsT::handle_type handle) noexcept
    {
        return smart_handle_access<tcp_acceptor_adapter>::create<TraitsT>(std::move(impl), handle);
    }
};

template <class TraitsT>
class tcp_acceptor_factory
{
public:
    using tcp_acceptor_handler_type = tcp_acceptor_handler_t<TraitsT>;
    
    virtual ~tcp_acceptor_factory() {}
    //virtual smart_handle_facade<TraitsT, tcp_acceptor_adapter> create_tcp_acceptor(cstring_view address, uint16_t port, tcp_socket_type dt) = 0;
    virtual smart_handle_facade<TraitsT, tcp_acceptor_adapter> create_tcp_acceptor(cstring_view address, uint16_t port, tcp_socket_type dt, function<void(tcp_acceptor_handler_type const&)> const& handler) = 0;
};

}}

#endif // SONIA_IO_TCP_ACCEPTOR_HPP
