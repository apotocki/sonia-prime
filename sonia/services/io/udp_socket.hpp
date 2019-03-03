//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_UDP_SOCKET_HPP
#define SONIA_IO_UDP_SOCKET_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <system_error>

#include "sonia/array_view.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/function.hpp"
#include "sonia/string.hpp"
#include "sonia/cstdint.hpp"

#include "socket_address.hpp"

namespace sonia { namespace io {

template <class TraitsT>
class udp_socket_adapter;

template <class TraitsT>
using udp_connector_t = function<void(size_t rsize, socket_address const&, udp_socket_adapter<typename TraitsT::weak_traits_type>)>;

template <class TraitsT>
using udp_connection_handler_t = function<void(array_view<char>, udp_connector_t<TraitsT> const&)>;

template <typename TraitsT>
class udp_socket_service
{
protected:
    using udp_handle_type = typename TraitsT::handle_type;
    using udp_connection_handler_type = udp_connection_handler_t<TraitsT>;
    using udp_connector_type = udp_connector_t<TraitsT>;

public:
    virtual ~udp_socket_service() {}
    virtual void   udp_socket_bind(udp_handle_type, cstring_view address, uint16_t port) = 0;
    virtual void   udp_socket_listen(udp_handle_type, function<void(udp_connection_handler_type const&)> const& handler) = 0;
    virtual size_t udp_socket_read_some(udp_handle_type, void * buff, size_t sz) = 0;
    virtual size_t udp_socket_write_some(udp_handle_type, socket_address const&, void const* buff, size_t sz) = 0;
    virtual size_t udp_socket_write_some(udp_handle_type, cstring_view address, uint16_t port, void const* buff, size_t sz) = 0;
    virtual void   udp_socket_on_close(udp_handle_type) = 0;
    virtual void   free_handle(identity<udp_socket_service>, udp_handle_type) = 0;
};

template <class TraitsT>
class udp_socket_adapter
{
    friend class udp_socket_access;

    using socket_handle = typename TraitsT::handle_type;
    using service_type = udp_socket_service<typename TraitsT::strong_traits_type>;

    udp_socket_adapter(shared_ptr<service_type> impl, socket_handle handle, bool addref = true)
        : impl_(std::move(impl)), handle_(handle)
    {
        if (addref) TraitsT::add_ref(handle_);
    }

public:
    udp_socket_adapter() : handle_(TraitsT::not_initialized_v) {}

    udp_socket_adapter(udp_socket_adapter const& rhs)
        : udp_socket_adapter(rhs.impl_, rhs.handle_)
    {
        TraitsT::add_ref(handle_);
    }

    udp_socket_adapter(udp_socket_adapter && rhs)
        : udp_socket_adapter(std::move(rhs.impl_), rhs.handle_, false)
    {
        rhs.handle_ = TraitsT::not_initialized_v;
    }

    ~udp_socket_adapter()
    {
        TraitsT::release(impl_.get(), handle_);
    }

    udp_socket_adapter& operator= (udp_socket_adapter const& rhs)
    {
        if (BOOST_LIKELY(rhs.handle_ != handle_)) {
            TraitsT::release(impl_.get(), handle_);
            handle_ = rhs.handle_;
            impl_ = rhs.impl_;
            TraitsT::add_ref(handle_);
        }
        return *this;
    }

    udp_socket_adapter& operator= (udp_socket_adapter && rhs)
    {
        if (BOOST_LIKELY(rhs.handle_ != handle_)) {
            TraitsT::release(impl_.get(), handle_);
            handle_ = rhs.handle_;
            impl_ = std::move(rhs.impl_);
            rhs.handle_ = TraitsT::not_initialized_v;
        }
        return *this;
    }

    void bind(cstring_view address, uint16_t port)
    {
        impl_->udp_socket_bind(handle_, address, port);
    }

    void listen(function<void(udp_connection_handler_t<TraitsT> const&)> const& handler)
    {
        impl_->udp_socket_listen(handle_, handler);
    }

    template <typename T>
    size_t read_some(array_view<T> buff)
    {
        return impl_->udp_socket_read_some(handle_, buff.begin(), buff.size() * sizeof(T));
    }

    template <typename T>
    size_t read_some(T * buff, size_t sz)
    {
        return impl_->udp_socket_read_some(handle_, buff, sz * sizeof(T));
    }

    template <typename T>
    size_t write_some(socket_address const& address, array_view<T> buff)
    {
        return impl_->udp_socket_write_some(handle_, address, buff.begin(), buff.size() * sizeof(T));
    }

    template <typename T>
    size_t write_some(cstring_view address, uint16_t port, array_view<T> buff)
    {
        return impl_->udp_socket_write_some(handle_, address, port, buff.begin(), buff.size() * sizeof(T));
    }

    template <typename T>
    size_t write_some(cstring_view address, uint16_t port, const T * buff, size_t sz)
    {
        return impl_->udp_socket_write_some(handle_, address, port, buff, sz * sizeof(T));
    }

    void close() noexcept
    {
        if (TraitsT::close(impl_.get(), handle_)) {
            impl_->udp_socket_on_close(handle_);
        }
    }

private:
    shared_ptr<service_type> impl_;
    socket_handle handle_;
};

class udp_socket_access
{
public:
    template <typename TraitsT>
    static udp_socket_adapter<TraitsT> create_udp_socket(shared_ptr<udp_socket_service<typename TraitsT::strong_traits_type>> impl, typename TraitsT::handle_type handle)
    {
        return udp_socket_adapter<TraitsT>(std::move(impl), handle);
    }
};

template <class TraitsT>
class udp_socket_factory
{
public:
    virtual ~udp_socket_factory() {}

    virtual udp_socket_adapter<TraitsT> create_udp_socket() = 0;
};

}}

#endif // SONIA_IO_UDP_SOCKET_HPP
