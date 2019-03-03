//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_TCP_SOCKET_HPP
#define SONIA_IO_TCP_SOCKET_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <system_error>

#include "sonia/array_view.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/function.hpp"
#include "sonia/string.hpp"
#include "sonia/cstdint.hpp"

namespace sonia { namespace io {

enum class tcp_socket_type
{
    TCP,
    SSL
};

template <class TraitsT>
class tcp_socket_adapter;

template <class TraitsT>
using tcp_connector_t = function<void(tcp_socket_adapter<TraitsT>, size_t rsize)>;

template <class TraitsT>
using tcp_connection_handler_t = function<void(array_view<char>, tcp_connector_t<TraitsT> const&)>;

template <typename TraitsT>
class tcp_socket_service
{
protected:
    using tcp_handle_type = typename TraitsT::handle_type;
    using tcp_connection_handler_type = tcp_connection_handler_t<TraitsT>;

public:
    virtual ~tcp_socket_service() {}

    virtual void   tcp_socket_bind(tcp_handle_type, cstring_view address, uint16_t port) = 0;
    virtual void   tcp_socket_listen(tcp_handle_type, function<void(tcp_connection_handler_type const&)> const& handler) = 0;
    virtual size_t tcp_socket_read_some(tcp_handle_type, void * buff, size_t sz) = 0;
    virtual void   tcp_socket_async_read_some(tcp_handle_type, void * buff, size_t sz, function<void(std::error_code const&, size_t)> const& ftor) = 0;
    virtual size_t tcp_socket_write_some(tcp_handle_type, void const* buff, size_t sz) = 0;
    virtual void   tcp_socket_on_close(tcp_handle_type) = 0;
    virtual void   free_handle(identity<tcp_socket_service>, tcp_handle_type) = 0;
};

template <class TraitsT>
class tcp_socket_adapter
{
    friend class tcp_socket_access;

    using socket_handle = typename TraitsT::handle_type;
    using service_type = tcp_socket_service<TraitsT>;

    tcp_socket_adapter(shared_ptr<service_type> impl, socket_handle handle)
        : impl_(std::move(impl)), handle_(handle)
    {
        TraitsT::add_ref(handle_);
    }

public:
    tcp_socket_adapter() : handle_(TraitsT::not_initialized_v) {}

    tcp_socket_adapter(tcp_socket_adapter const& rhs)
        : tcp_socket_adapter(rhs.impl_, rhs.handle_)
    {
        TraitsT::add_ref(handle_);
    }

    tcp_socket_adapter(tcp_socket_adapter && rhs)
        : impl_(std::move(rhs.impl_)), handle_(rhs.handle_)
    {
        rhs.handle_ = TraitsT::not_initialized_v;
    }

    ~tcp_socket_adapter()
    {
        TraitsT::release(impl_.get(), handle_);
    }

    tcp_socket_adapter& operator= (tcp_socket_adapter const& rhs)
    {
        if (BOOST_LIKELY(rhs.handle_ != handle_)) {
            TraitsT::release(impl_.get(), handle_);
            handle_ = rhs.handle_;
            impl_ = rhs.impl_;
            TraitsT::add_ref(handle_);
        }
        return *this;
    }

    tcp_socket_adapter& operator= (tcp_socket_adapter && rhs)
    {
        if (BOOST_LIKELY(rhs.handle_ != handle_)) {
            TraitsT::release(impl_.get(), handle_);
            handle_ = rhs.handle_;
            impl_ = std::move(rhs.impl_);
            rhs.handle_ = TraitsT::not_initialized_v;
        }
        return *this;
    }

    // api
    void bind(cstring_view address, uint16_t port)
    {
        impl_->tcp_socket_bind(handle_, address, port);
    }

    void listen(function<void(tcp_connection_handler_t<TraitsT> const&)> const& handler)
    {
        impl_->tcp_socket_listen(handle_, handler);
    }

    template <typename T>
    size_t read_some(array_view<T> buff)
    {
        return impl_->tcp_socket_read_some(handle_, buff.begin(), buff.size() * sizeof(T));
    }

    template <typename T>
    size_t read_some(T * buff, size_t sz)
    {
        return impl_->tcp_socket_read_some(handle_, buff, sz * sizeof(T));
    }

    template <typename T>
    void async_read_some(array_view<T> buff, function<void(std::error_code const&, size_t)> const& ftor)
    {
        return impl_->tcp_socket_async_read_some(handle_, buff.begin(), buff.size() * sizeof(T), ftor);
    }

    template <typename T>
    size_t write_some(array_view<const T> buff)
    {
        return impl_->tcp_socket_write_some(handle_, buff.begin(), buff.size() * sizeof(T));
    }

    template <typename T>
    size_t write_some(const T * buff, size_t sz)
    {
        return impl_->tcp_socket_write_some(handle_, buff, sz * sizeof(T));
    }

    void close() noexcept
    {
        if (TraitsT::close(impl_.get(), handle_)) {
            impl_->tcp_socket_on_close(handle_);
        }
    }

private:
    shared_ptr<service_type> impl_;
    socket_handle handle_;
};

class tcp_socket_access
{
public:
    template <typename TraitsT>
    static tcp_socket_adapter<TraitsT> create_tcp_socket(shared_ptr<tcp_socket_service<typename TraitsT::strong_traits_type>> impl, typename TraitsT::handle_type handle)
    {
        return tcp_socket_adapter<TraitsT>(std::move(impl), handle);
    }
};

template <class TraitsT>
class tcp_socket_factory
{
public:
    virtual ~tcp_socket_factory() {}
    virtual tcp_socket_adapter<TraitsT> create_tcp_socket(tcp_socket_type dt = tcp_socket_type::TCP) = 0;
    virtual tcp_socket_adapter<TraitsT> create_tcp_socket(cstring_view address, uint16_t port = 0, tcp_socket_type dt = tcp_socket_type::TCP) = 0;
};

}}

#endif // SONIA_IO_TCP_SOCKET_HPP
