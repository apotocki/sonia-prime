//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "transceiver_builder.hpp"
#include "transceiver_service.hpp"

namespace sonia { namespace services {

transceiver_builder::transceiver_builder()
{
    set_log_attribute("Type", "builder");
    parameters_.bind()
        .variable("socket-factory", &transceiver_configuration::socket_factory, "a socket factory name").required()
        //.variable("threads", &transceiver_configuration::threads, "threads count").required()
        //.variable("fibers", &transceiver_configuration::fibers, "fibers count per thread").default_value(0)
    ;
}

shared_ptr<service> transceiver_builder::build(json_object const& parameters)
{
    transceiver_configuration cfg;
    parameters_.apply(parameters, &cfg);
    return make_shared<transceiver_service>(cfg);
}

}}
