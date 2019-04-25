//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/services/builder.ipp"
#include "transceiver_builder.hpp"
#include "transceiver_service.hpp"

namespace sonia::services {

transceiver_service_builder::transceiver_service_builder()
{
    set_log_attribute("Type", "builder");
    parameters_.bind()
        .variable("socket-factory", &transceiver_service_configuration::socket_factory, "a socket factory name").required()
        //.variable("threads", &transceiver_configuration::threads, "threads count").required()
        //.variable("fibers", &transceiver_configuration::fibers, "fibers count per thread").default_value(0)
    ;
}

}
