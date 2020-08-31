//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "io_cache_service_builder.hpp"

namespace sonia::services {

void io_cache_service_builder::open()
{
    parameters_.bind()
        .variable("tcp-socket-factory", &io_cache_service_configuration::tcp_socket_factory, "tcp socket factory name")
        //.variable("udp-socket-factory", &io_cache_service_configuration::udp_socket_factory, "udp socket factory name")
        .variable("max-connection-count", &io_cache_service_configuration::connection_count, "max connection count").default_value(16)
        .variable("per-route-max-connection-count", &io_cache_service_configuration::per_route_connection_count, "max connection count per unique route").default_value(4)
    ;
}

}
