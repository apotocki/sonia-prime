//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "echo_connector_builder.hpp"

namespace sonia::services {

namespace sp = sonia::parameters;

void echo_connector_builder::open()
{
    parameters_.bind()
        // .variable("param", &echo_connector_configuration::param, "param").required()
    ;
}

}
