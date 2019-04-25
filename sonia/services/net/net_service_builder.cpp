//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/services/builder.ipp"
#include "net_service_builder.hpp"
#include "net_service.hpp"

namespace sonia::services {

namespace sp = sonia::parameters;

net_service_builder::net_service_builder()
{
    set_log_attribute("Type", "builder");
    parameters_.bind()
        .variable("tcp-socket-factory", &net_service_configuration::tcp_socket_factory, "tcp socket factory name")
        .variable("udp-socket-factory", &net_service_configuration::udp_socket_factory, "udp socket factory name")
        .variable("scheduler", &net_service_configuration::scheduler, "scheduler service name").required()
        .array("listeners", &net_service_configuration::listeners, "listeners set definition")
            .binder(sp::parameters_description<net::listener_configuration>().bind()
                .variable("connector", &net::listener_configuration::connector, "connector service name").required()
                .variable("address", &net::listener_configuration::address, "address string").required()
                .variable("port", &net::listener_configuration::port, "port").required()
                .variable("buffer-size", &net::listener_configuration::buffer_size, "initial buffer size").default_value(1024)
                .variable("workers", &net::listener_configuration::workers_count, "workers count per listener").default_value(16)
                .variable("family", &net::listener_configuration::family, "socket type").default_value(sonia::sal::net_family_type::INET)
                    .binder([](json_value const& v)->sonia::sal::net_family_type {
                        string_view sv = v.get_string();
                        if (sv == "INET") return sonia::sal::net_family_type::INET;
                        if (sv == "INET6") return sonia::sal::net_family_type::INET6;
                        throw exception("unknown string value '%1%' for the net family type"_fmt % sv);
                    })
                .variable("type", &net::listener_configuration::type, "socket type").default_value(net::listener_type::TCP)
                    .binder([](json_value const& v)->net::listener_type {
                        string_view sv = v.get_string();
                        if (sv == "tcp") return net::listener_type::TCP;
                        if (sv == "ssl") return net::listener_type::SSL;
                        if (sv == "udp") return net::listener_type::UDP;
                        throw exception("unknown string value '%1%' for the net listener type"_fmt % sv);
                    })
            )
    ;
}

}
