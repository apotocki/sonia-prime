//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <atomic>

#include "sonia/services/service.hpp"
#include "sonia/services/io/udp_socket.hpp"
#include "sonia/services/io/sockets.hpp"
#include "sonia/services/scheduler/scheduler.hpp"

#include "net_service_configuration.hpp"

namespace sonia::services {

class net_service : public service
{
public:
    virtual void reload() = 0;

    net_service_configuration configuration;
};

class net_service_impl
    : public net_service
    , public enable_shared_from_this<net_service_impl>
{
public:
    explicit net_service_impl(net_service_configuration const& cfg);

    void open() override;
    void close() noexcept override;
    void reload() override;

    struct listener
    {
        virtual ~listener() = default;
        virtual void close() = 0;

        std::atomic<size_t> workers_count{1};
        size_t workers_max{1};
        size_t buffer_size{0};

        std::atomic<bool> closed{false};
    };

private:
    shared_ptr<sonia::io::tcp_server_socket_factory_type> tcp_server_socket_factory_;
    shared_ptr<sonia::io::udp_socket_factory_type> udp_socket_factory_;
    shared_ptr<scheduler> scheduler_;
    std::vector<shared_ptr<listener>> listeners_;
};

}
