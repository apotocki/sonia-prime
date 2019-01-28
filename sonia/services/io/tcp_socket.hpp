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

class tcp_socket_service
{
public:
    virtual ~tcp_socket_service() {}

    virtual void   tcp_socket_close(void * handle) = 0;

    virtual size_t tcp_socket_read_some(void * handle, void * buff, size_t sz) = 0;
    virtual void   tcp_socket_async_read_some(void * handle, void * buff, size_t sz, function<void(std::error_code const&, size_t)> const& ftor) = 0;

    virtual size_t tcp_socket_write_some(void * handle, void const* buff, size_t sz) = 0;

};

class tcp_socket
{
    friend class tcp_socket_access;

    tcp_socket(shared_ptr<tcp_socket_service> impl, void * handle)
        : impl_(std::move(impl)), handle_(handle)
    {}

public:
    ~tcp_socket()
    {
        if (handle_) impl_->tcp_socket_close(handle_);
    }

    tcp_socket(tcp_socket const&) = delete;
    tcp_socket & operator= (tcp_socket const&) = delete;

    tcp_socket(tcp_socket && rhs)
        : impl_(std::move(rhs.impl_)), handle_(rhs.handle_)
    {
        rhs.handle_ = nullptr;
    }

    tcp_socket& operator= (tcp_socket && rhs)
    {
        if (handle_ && handle_ != rhs.handle_) impl_->tcp_socket_close(handle_);
        impl_ = std::move(rhs.impl_);
        handle_ = rhs.handle_;
        rhs.handle_ = nullptr;
        return *this;
    }

    // api
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

private:
    shared_ptr<tcp_socket_service> impl_;
    void * handle_;
};

class tcp_socket_access
{
public:
    static tcp_socket create_tcp_socket(shared_ptr<tcp_socket_service> impl, void * handle)
    {
        return tcp_socket(std::move(impl), handle);
    }
};

class tcp_socket_factory
{
public:
    virtual ~tcp_socket_factory() {}

    virtual tcp_socket create_tcp_socket(string_view address, uint16_t port = 0, tcp_socket_type dt = tcp_socket_type::TCP) = 0;
    virtual size_t tcp_socket_count(tcp_socket_type) const = 0;
};

}}

#endif // SONIA_IO_TCP_SOCKET_HPP
