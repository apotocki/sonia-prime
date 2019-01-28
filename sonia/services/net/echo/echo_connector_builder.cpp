//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "echo_connector_builder.hpp"
#include "echo_connector.hpp"

namespace sonia { namespace services {

namespace sp = sonia::parameters;

echo_connector_builder::echo_connector_builder()
{
    set_log_attribute("Type", "builder");
    parameters_.bind()
        // .variable("param", &echo_connector_configuration::param, "param").required()
    ;
}

shared_ptr<service> echo_connector_builder::build(json_object const& parameters)
{
    echo_connector_configuration cfg;
    parameters_.apply(parameters, &cfg);
    return make_shared<echo_connector>(cfg);
}

}}
