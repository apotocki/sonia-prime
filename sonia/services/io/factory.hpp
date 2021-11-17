//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <atomic>

#include "sonia/shared_ptr.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/logger/loggable.hpp"

#include "tcp_socket.hpp"
#include "udp_socket.hpp"
#include "file.hpp"
#include "sockets.hpp"

#include "sonia/services/diagnostic.hpp"
#include "sonia/services/scheduler/scheduler.hpp"

namespace sonia::io {

class factory
    : public tcp_socket_factory_type
    , public tcp_server_socket_factory_type
    , public udp_socket_factory_type
    , public file_factory
    , public file_service
    , public enable_shared_from_this<factory>
    , public diagnostic
    , public virtual loggable
{
    static constexpr long qsz_min_value = (std::numeric_limits<long>::min)();

public:
    factory();
    ~factory() override;

    void open();
    void close() noexcept;
    virtual std::string name() const;

    std::string get_diagnostic_info() override;

    // tcp_socket_factory
    tcp_socket create_connected_tcp_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt) override;

    // tcp_server_socket_factory
    tcp_server_socket create_server_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt) override;

     // udp_socket_factory
    udp_socket create_udp_socket(sonia::sal::net_family_type) override;

    // file factory
    file open_file(cstring_view path, file_open_mode, file_access_mode, file_bufferring_mode) override;

    // file service
    size_t file_read(sonia::sal::file_handle_type, uint64_t fileoffset, array_view<char> dest) override;
    size_t file_write(sonia::sal::file_handle_type, uint64_t fileoffset, array_view<const char> src) override;

    class impl_base 
        : public tcp_socket_service_type
        , public tcp_server_socket_service_type
        , public udp_socket_service_type
    {
    protected:
        using tcp_handle_type = tcp_socket_service_type::tcp_handle_type;

    public:
        explicit impl_base(shared_ptr<factory> wr);
        
        impl_base(impl_base const&) = delete;
        impl_base& operator=(impl_base const&) = delete;

        virtual void thread_proc() = 0;

        void close() noexcept;
        virtual void park_threads() noexcept  = 0;

        void on_add_callback();
        void on_release_callback() noexcept;

        virtual tcp_server_socket do_create_tcp_server_socket(sonia::sal::socket_handle, sonia::sal::net_family_type dt) = 0;
        virtual tcp_socket do_create_tcp_socket(sonia::sal::socket_handle, sonia::sal::net_family_type dt) = 0;
        virtual udp_socket do_create_udp_socket(sonia::sal::socket_handle, sonia::sal::net_family_type dt) = 0;

        shared_ptr<factory> wrapper;
        std::atomic<long> qsz{0};

        scheduler& get_scheduler() const { return *wrapper->scheduler_; }
        logger::logger_ptr logger() const { return wrapper->logger(); }
    };
    
    class listener_task : public scheduler_task_adapter<factory::listener_task>
    {
        shared_ptr<factory::impl_base> impl_;

    public:
        listener_task(shared_ptr<factory::impl_base> pi)
            : impl_(std::move(pi))
        {}

        void run() noexcept override
        {
            impl_->thread_proc();
        }
    };

    friend class impl_base;

    size_t thread_count() const;

protected:
    shared_ptr<scheduler> scheduler_;
    size_t thread_count_{1};

private:
    shared_ptr<impl_base> get_dataptr();

    void close_handle(shared_handle<socket_handle_traits> * h);

    void initialize_impl();

    mutable fibers::mutex close_mtx_;
    fibers::condition_variable close_cond_;
    shared_ptr<impl_base> impl_holder_;
    weak_ptr<impl_base> impl_;

public:
    // diagnostic
    std::atomic<int> pending_reads_{ 0 }, pending_writes_{ 0 };
};

}
