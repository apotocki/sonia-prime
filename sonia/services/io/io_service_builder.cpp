//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "io_service_builder.hpp"

namespace sonia::services {

namespace sp = sonia::parameters;

void io_service_builder::open()
{
    parameters_.bind()
        .variable("scheduler", &io_service_configuration::scheduler, "scheduler service name").required()
        .variable("threads", &io_service_configuration::threads, "listening for async events threads count").required()
    ;
}

}
