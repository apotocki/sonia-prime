//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/services/builder.ipp"
#include "http_connector_builder.hpp"
#include "http_connector.hpp"

namespace sonia::services {

namespace sp = sonia::parameters;

http_connector_builder::http_connector_builder()
{
    set_log_attribute("Type", "builder");
    parameters_.bind()
        .variable("keep-alive-max-count", &http_connector_configuration::keep_alive_max_count, "max amount of \"keep alive\" connections").default_value(16)
        .variable("one-shot-max-count", &http_connector_configuration::not_keep_alive_max_count, "max amount of \"one shot\" connections").default_value(16)
        .variable("request-buffer-size", &http_connector_configuration::request_buffer_size, "request buffer size").default_value(65536)
        .variable("response-buffer-size", &http_connector_configuration::response_buffer_size, "response buffer size").default_value(65536)
        .variable("dos-message", &http_connector_configuration::dos_message, "the message returned in the case of dos event").default_value("max number of connections exceeded")
        .variable("404-message", &http_connector_configuration::page404_message, "the message returned in the case of 404 event").default_value("The page you requested was not found.")
        .variable("404-application", &http_connector_configuration::page404_application_name, "the application that handles 404 event")
        .array("routes", &http_connector_configuration::routes, "routes page404_application_name")
            .binder(sp::parameters_description<http_route>().bind()
                .variable("application", &http_route::application_name, "http application name").required()
                .variable("pathre", &http_route::pathre, "path regexp").required()
                    .binder([](json_value const& v)->boost::regex {
                        return boost::regex(to_string(v.get_string()));
                    })
            )
    ;
}

}
