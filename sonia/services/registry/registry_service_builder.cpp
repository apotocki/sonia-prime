//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/services/builder.ipp"
#include "registry_service_builder.hpp"
#include "registry_service.hpp"

namespace sonia::services {

registry_service_builder::registry_service_builder()
{
    set_log_attribute("Type", "builder");
    parameters_.bind()
        .variable("path", &registry_service_configuration::path, "path used to store the registry state")
            .binder([](json_value const& v) { return boost::filesystem::path(to_string(v.get_string())); })
        .variable("dir", &registry_service_configuration::dir, "dir used to store registry state")
            .binder([](json_value const& v) { return boost::filesystem::path(to_string(v.get_string())); })
    ;
}

}
