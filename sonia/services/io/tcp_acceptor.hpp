//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_TCP_ACCEPTOR_HPP
#define SONIA_IO_TCP_ACCEPTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "tcp_socket.hpp"

namespace sonia { namespace io {

class tcp_acceptor_service
{
public:
    virtual ~tcp_acceptor_service() {}
    virtual void tcp_acceptor_close(void * handle) noexcept = 0;
};

class tcp_acceptor
{
    friend class tcp_acceptor_access;

    tcp_acceptor(shared_ptr<tcp_acceptor_service> impl, void * handle) noexcept
        : impl_(std::move(impl)), handle_(handle)
    {}

public:
    tcp_acceptor() : handle_(nullptr) {}

    tcp_acceptor(tcp_acceptor const& rhs) = delete;
    tcp_acceptor(tcp_acceptor && rhs) noexcept : impl_(std::move(rhs.impl_)), handle_(rhs.handle_)
    {
        rhs.handle_ = nullptr;
    }

    ~tcp_acceptor()
    {
        if (handle_) impl_->tcp_acceptor_close(handle_);
    }

    tcp_acceptor & operator= (tcp_acceptor const& rhs) = delete;
    tcp_acceptor & operator= (tcp_acceptor && rhs) noexcept
    {
        if (handle_) {
            impl_->tcp_acceptor_close(handle_);
        }
        impl_ = std::move(rhs.impl_);
        handle_ = rhs.handle_;
        rhs.handle_ = nullptr;
        return *this;
    }

    void close() noexcept
    {
        if (handle_) {
            impl_->tcp_acceptor_close(handle_);
            handle_ = nullptr;
        }
    }

private:
    shared_ptr<tcp_acceptor_service> impl_;
    void * handle_;
};

class tcp_acceptor_access
{
public:
    static tcp_acceptor create_tcp_acceptor(shared_ptr<tcp_acceptor_service> impl, void * handle) noexcept
    {
        return tcp_acceptor(std::move(impl), handle);
    }
};

class tcp_acceptor_factory {
public:
    virtual ~tcp_acceptor_factory() {}

    // return an accepted socket and a size of read data in the provided buffer
    using connector_t = function<void(tcp_socket soc, size_t rsize)>;
    using connection_factory_t = function<void(array_view<char>, connector_t const&)>;

    virtual tcp_acceptor create_tcp_acceptor(cstring_view address, uint16_t port, tcp_socket_type dt, function<void(connection_factory_t const&)> const& handler) = 0;
};

}}

#endif // SONIA_IO_TCP_ACCEPTOR_HPP
