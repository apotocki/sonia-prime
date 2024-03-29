//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "http_static_application_builder.hpp"

namespace sonia::services {

namespace sp = sonia::parameters;

void http_static_application_builder::open()
{
    using fwd_pair_t = std::pair<boost::regex, std::string>;
    parameters_.bind()
        .variable("www-path", &http_static_application_configuration::www_path, "web path to map").required()
        .variable("sys-path", &http_static_application_configuration::sys_path, "system path to map").required()
        .variable("cache-control", &http_static_application_configuration::cache_contol_header, "cache_contol_header").default_value("max-age=3600, immutable")
        .variable("mime-map-path", &http_static_application_configuration::mime_mapping_file_path, "path to mime-map file").required()
        .variable("404-application", &http_static_application_configuration::page404app, "wep app to handle 404 error")
        .array("forwards", &http_static_application_configuration::forward_mapping, "forward mapping")
            .binder(sp::parameters_description<fwd_pair_t>().bind()
                .variable("forward", &fwd_pair_t::second, "forward path").required()
                .variable("pathre", &fwd_pair_t::first, "path regexp to forward").required()
                    .binder([](json_value const& v)->boost::regex {
                        return boost::regex(std::string(v.get_string()));
                    })
            )
    ;
}

}
