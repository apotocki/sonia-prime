//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "io_service.hpp"
#include "sonia/services.hpp"

namespace sonia::services {

io_service::io_service(io_service_configuration cfg)
    : cfg_(cfg)
{
    set_log_attribute("Type", "io");
    locate(cfg.scheduler, scheduler_);
    thread_count_ = cfg_.threads;
}

void io_service::open()
{
    sonia::io::factory::open();
}

void io_service::close() noexcept
{
    sonia::io::factory::close();
}

io_ssl_service::io_ssl_service(io_ssl_service_configuration cfg)
    : cfg_(std::move(cfg))
{
    set_log_attribute("Type", "io-ssl");
    if (cfg_.tcp_server_socket_factory) {
        locate(*cfg_.tcp_server_socket_factory, base_server_socket_factory_);
    }
    if (cfg_.tcp_socket_factory) {
        locate(*cfg_.tcp_server_socket_factory, base_socket_factory_);
    }
}

void io_ssl_service::open()
{
    io::ssl_factory::open(cfg_);
}

void io_ssl_service::close() noexcept
{

}

}
