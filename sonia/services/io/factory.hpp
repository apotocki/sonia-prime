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
#include "sonia/concurrency.hpp"
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
    static constexpr long qsz_min_value = boost::integer_traits<long>::const_min;

public:
    factory();
    virtual ~factory() override;

    void open(uint32_t thr_cnt);
    void close();
    virtual std::string name() const;

    // tcp_socket_factory
    tcp_socket create_tcp_socket(cstring_view address, uint16_t port, tcp_socket_type dt = tcp_socket_type::TCP) override;
    size_t tcp_socket_count(tcp_socket_type) const override;

    // tcp_socket_service
    void   tcp_socket_close(intptr_t handle) override;
    size_t tcp_socket_read_some(intptr_t handle, void * buff, size_t sz) override;
    void   tcp_socket_async_read_some(intptr_t handle, void * buff, size_t sz, function<void(std::error_code const&, size_t)> const& ftor) override;
    size_t tcp_socket_write_some(intptr_t handle, void const* buff, size_t sz) override;

    // tcp_acceptor_factory
    tcp_acceptor create_tcp_acceptor(cstring_view address, uint16_t port, tcp_socket_type dt, function<void(tcp_acceptor_factory::connection_factory_t const&)> const& handler) override;

    //  tcp_acceptor_service
    void tcp_acceptor_close(void * handle) noexcept override;

    // file factory
    file open_file(cstring_view path, file_open_mode, file_access_mode, file_bufferring_mode) override;

    // file service
    size_t file_read(sonia::sal::file_handle_type, uint64_t fileoffset, array_view<char> dest) override;
    size_t file_write(sonia::sal::file_handle_type, uint64_t fileoffset, array_view<const char> src) override;

    class impl_base
    {
    public:
        virtual ~impl_base() {}

        explicit impl_base(shared_ptr<factory> wr) : wrapper(std::move(wr)) {}

        virtual void park_threads() noexcept  = 0;

        shared_ptr<factory> wrapper;
    };
    
    friend class impl_base;

    void on_add_callback();
    void on_release_callback() noexcept;
    void join_threads();
    size_t thread_count() const;
    std::atomic<long> qsz{0};

private:
    void thread_proc();
    void initialize_impl(uint32_t thr_cnt);

    mutable boost::fibers::mutex close_mtx_;
    boost::fibers::condition_variable close_cond_;
    std::vector<thread> threads_;
    std::unique_ptr<impl_base> impl_data_;
};

}}

#endif // SONIA_IO_FACTORY_HPP
