//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "bookkeeper_service_builder.hpp"

namespace sonia::services {

void bookkeeper_service_builder::open()
{
    parameters_.bind()
        .variable("path", &bookkeeper_service_configuration::path, "path used to store the bookkeeper state")
            .binder([](json_value const& v) { return fs::path(v.get_u8string()); })
        .variable("dir", &bookkeeper_service_configuration::dir, "dir used to store the bookkeeper state")
            .binder([](json_value const& v) { return fs::path(v.get_u8string()); })
    ;
}

}
