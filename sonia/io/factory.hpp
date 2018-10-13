//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_FACTORY_HPP
#define SONIA_IO_FACTORY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <vector>
#include <atomic>

#include "sonia/shared_ptr.hpp"
#include "sonia/thread.hpp"
#include "sonia/logger/loggable.hpp"

#include "tcp_socket.hpp"
#include "tcp_acceptor.hpp"
#include "file.hpp"

namespace sonia { namespace io {

class factory
    : public tcp_socket_factory
    , public tcp_socket_service
    , public tcp_acceptor_factory
    , public tcp_acceptor_service
    , public file_factory
    , public file_service
    , public enable_shared_from_this<factory>
    , public virtual loggable
{
public:
    factory();
    virtual ~factory();

    void open(uint32_t thr_cnt);
    void close();
    virtual std::string name() const;

    // tcp_socket_factory
    tcp_socket create_tcp_socket(string_view address, uint16_t port, tcp_socket_type dt = tcp_socket_type::TCP) override;
    size_t tcp_socket_count(tcp_socket_type) const override;

    // tcp_socket_service
    void   tcp_socket_close(void * handle) override;
    size_t tcp_socket_read_some(void * handle, void * buff, size_t sz) override;
    void   tcp_socket_async_read_some(void * handle, void * buff, size_t sz, function<void(std::error_code const&, size_t)> const& ftor) override;
    size_t tcp_socket_write_some(void * handle, void const* buff, size_t sz) override;

    // tcp_acceptor_factory
    tcp_acceptor create_tcp_acceptor(string_view address, uint16_t port, tcp_socket_type dt = tcp_socket_type::TCP) override;

    //  tcp_acceptor_service
    void tcp_acceptor_async_accept_and_read_some(void * handle, void * buff, size_t sz, acceptor_functor const&) override;
    void tcp_acceptor_close(void * handle) override;

    // file factory
    file open_file(string_view path, file_open_mode, file_access_mode, file_bufferring_mode) override;

    // file service
    size_t file_read(void * handle, uint64_t fileoffset, array_view<char> dest) override;
    size_t file_write(void * handle, uint64_t fileoffset, array_view<const char> src) override;

    void thread_proc();

private:
    void * impl_data_;
};

}}

#endif // SONIA_IO_FACTORY_HPP
