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

class tcp_acceptor_service {
public:
    virtual ~tcp_acceptor_service() {}

    typedef function<void(void *, size_t)> renew_functor;
    typedef function<void(std::error_code const&, size_t, tcp_socket soc, renew_functor const&)> acceptor_functor;

    virtual void tcp_acceptor_close(void * handle) = 0;
    virtual void tcp_acceptor_async_accept_and_read_some(void * handle, void * buff, size_t sz, acceptor_functor const&) = 0;
};

class tcp_acceptor
{
    friend class tcp_acceptor_access;

    tcp_acceptor(shared_ptr<tcp_acceptor_service> impl, void * handle)
        : impl_(std::move(impl)), handle_(handle)
    {}

public:
    ~tcp_acceptor() {
        if (handle_) impl_->tcp_acceptor_close(handle_);
    }

    typedef function<void(void *, size_t)> renew_functor;
    typedef function<void(std::error_code const&, size_t, tcp_socket soc, renew_functor const&)> acceptor_functor;

    void close() {
        if (handle_) {
            impl_->tcp_acceptor_close(handle_);
            handle_ = nullptr;
        }
    }

    void async_accept(acceptor_functor const& func) {
        return impl_->tcp_acceptor_async_accept_and_read_some(handle_, nullptr, 0, func);
    }

    template <typename T>
    void async_accept_and_read_some(array_view<T> buff, acceptor_functor const& func) {
        return impl_->tcp_acceptor_async_accept_and_read_some(handle_, buff.begin(), buff.size() * sizeof(T), func);
    }

private:
    shared_ptr<tcp_acceptor_service> impl_;
    void * handle_;
};

class tcp_acceptor_access {
public:
    static tcp_acceptor create_tcp_acceptor(shared_ptr<tcp_acceptor_service> impl, void * handle) {
        return tcp_acceptor(std::move(impl), handle);
    }
};

class tcp_acceptor_factory {
public:
    virtual ~tcp_acceptor_factory() {}

    virtual tcp_acceptor create_tcp_acceptor(string_view address, uint16_t port, tcp_socket_type dt = tcp_socket_type::TCP) = 0;
    // acceptor_functor const& aftor,
};

}}

#endif // SONIA_IO_TCP_ACCEPTOR_HPP
