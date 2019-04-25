//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/services/builder.ipp"
#include "http_default_application_builder.hpp"
#include "http_default_application.hpp"

namespace sonia::services {

http_default_application_builder::http_default_application_builder()
{
    set_log_attribute("Type", "builder");
    parameters_.bind()
        .variable("response-code", &http_default_application_configuration::response_code, "response code").default_value(http::status::OK)
            .binder([](json_value const& v)->http::status {
                if (v.type() != json_value_type::number) {
                    throw exception("unknown response code value '%1%'"_fmt % v);
                }
                return (http::status)v.get_int();
            })
        .variable("title", &http_default_application_configuration::title, "response page title").default_value("default page")
        .variable("image", &http_default_application_configuration::image, "response page image")
        .variable("show-headers", &http_default_application_configuration::show_headers, "response page image").default_value(true)
    ;
}

}
