//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/services/builder.ipp"
#include "scheduler_service_builder.hpp"
#include "scheduler_service.hpp"

namespace sonia::services {

scheduler_service_builder::scheduler_service_builder()
{
    set_log_attribute("Type", "builder");
    parameters_.bind()
        .variable("threads", &scheduler_service_configuration::threads, "threads count").required()
        .variable("fibers", &scheduler_service_configuration::fibers, "fibers count per thread").default_value(0)
    ;
}

}
