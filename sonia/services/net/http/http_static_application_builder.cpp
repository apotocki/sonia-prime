//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/services/builder.ipp"
#include "http_static_application_builder.hpp"
#include "http_static_application.hpp"

namespace sonia::services {

namespace sp = sonia::parameters;

http_static_application_builder::http_static_application_builder()
{
    using fwd_pair_t = std::pair<boost::regex, std::string>;

    set_log_attribute("Type", "builder");
    parameters_.bind()
        .variable("www-path", &http_static_application_configuration::www_path, "web path to map").required()
        .variable("sys-path", &http_static_application_configuration::sys_path, "system path to map").required()
        .variable("mime-map-path", &http_static_application_configuration::mime_mapping_file_path, "path to mime-map file").required()
        .variable("404-application", &http_static_application_configuration::page404app, "wep app to handle 404 error")
        .array("forwards", &http_static_application_configuration::forward_mapping, "forward mapping")
            .binder(sp::parameters_description<fwd_pair_t>().bind()
                .variable("forward", &fwd_pair_t::second, "forward path").required()
                .variable("pathre", &fwd_pair_t::first, "path regexp to forward").required()
                    .binder([](json_value const& v)->boost::regex {
                        return boost::regex(to_string(v.get_string()));
                    })
            )
    ;
}

}
