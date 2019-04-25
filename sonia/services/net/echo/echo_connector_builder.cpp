//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/services/builder.ipp"
#include "echo_connector_builder.hpp"

#include "echo_connector.hpp"

namespace sonia::services {

namespace sp = sonia::parameters;

echo_connector_builder::echo_connector_builder()
{
    set_log_attribute("Type", "builder");
    parameters_.bind()
        // .variable("param", &echo_connector_configuration::param, "param").required()
    ;
}

}
