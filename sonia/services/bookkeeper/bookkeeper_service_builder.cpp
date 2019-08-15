//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/services/builder.ipp"
#include "bookkeeper_service_builder.hpp"
#include "bookkeeper_service.hpp"

namespace sonia::services {

bookkeeper_service_builder::bookkeeper_service_builder()
{
    set_log_attribute("Type", "builder");
    parameters_.bind()
        .variable("path", &bookkeeper_service_configuration::path, "path used to store the bookkeeper state")
            .binder([](json_value const& v) { return boost::filesystem::path(to_string(v.get_string())); })
        .variable("dir", &bookkeeper_service_configuration::dir, "dir used to store the bookkeeper state")
            .binder([](json_value const& v) { return boost::filesystem::path(to_string(v.get_string())); })
    ;
}

}
