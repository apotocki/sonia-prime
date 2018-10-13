//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "scheduler_builder.hpp"
#include "scheduler_service.hpp"

namespace sonia { namespace services {

scheduler_builder::scheduler_builder() {
    set_attribute("Type", "builder");
    parameters_.bind()
        .variable("threads", &scheduler_configuration::threads, "threads count").required()
        .variable("fibers", &scheduler_configuration::fibers, "fibers count per thread").default_value(0)
    ;
}

shared_ptr<service> scheduler_builder::build(json_object const& parameters) {
    scheduler_configuration cfg;
    parameters_.apply(parameters, &cfg);
    return make_shared<scheduler_service>(cfg);
}

}}
