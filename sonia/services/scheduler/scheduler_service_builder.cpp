//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "scheduler_service_builder.hpp"

namespace sonia::services {

void scheduler_service_builder::open()
{
    parameters_.bind()
        .variable("threads", &scheduler_service_configuration::threads, "threads count").required()
        .variable("fibers", &scheduler_service_configuration::fibers, "fibers count per thread").default_value(0)
    ;
}

}
