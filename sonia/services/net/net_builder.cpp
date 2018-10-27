//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "net_builder.hpp"
#include "net_service.hpp"

namespace sonia { namespace services {

namespace sp = sonia::parameters;

net_builder::net_builder() {
    set_attribute("Type", "builder");
    parameters_.bind()
        .variable("acceptor-factory", &net_configuration::acceptor_factory, "acceptor factory name").required()
        .variable("scheduler", &net_configuration::scheduler, "scheduler service name").required()
        .array("listeners", &net_configuration::listeners, "listeners set definition")
            .binder(sp::parameters_description<net::listener_configuration>().bind()
                .variable("connector", &net::listener_configuration::connector, "connector service name").required()
                .variable("address", &net::listener_configuration::address, "address string").required()
                .variable("port", &net::listener_configuration::port, "port").required()
                .variable("buffer-size", &net::listener_configuration::buffer_size, "initial buffer size").default_value(1024)
                .variable("type", &net::listener_configuration::type, "socket type").default_value(net::listener_type::TCP)
                    .binder([](json_value const& v) ->net::listener_type {
                        string_view sv = v.get_string();
                        if (sv == "tcp") return net::listener_type::TCP;
                        if (sv == "ssl") return net::listener_type::SSL;
                        if (sv == "udp") return net::listener_type::UDP;
                        throw exception("unknown string value '%1%' for the net listener type"_fmt % sv);
                    })
            )
    ;
}

shared_ptr<service> net_builder::build(json_object const& parameters) {
    net_configuration cfg;
    parameters_.apply(parameters, &cfg);
    return make_shared<net_service>(cfg);
}

}}
