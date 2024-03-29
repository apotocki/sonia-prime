//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "http_digest_authentication_application_builder.hpp"

#include "sonia/services.hpp"

namespace sonia::services {

namespace sp = sonia::parameters;

void http_digest_authentication_application_builder::open()
{
    parameters_.bind()
        .rebinder("digest", parameters::parameters_description<http_digest_authentication_application_configuration>().bind()
            .variable("provider", &http_digest_authentication_application_configuration::digest_app, "digest provider application").required()
            .variable("realm", &http_digest_authentication_application_configuration::digest_realm, "digest realm parameter").default_value("sonia@authentication")
            .variable("nonce-timeout", &http_digest_authentication_application_configuration::digest_nonce_timeout, "the lifetime of a new nonce that is not used by client").default_value(std::chrono::seconds(30))
                .binder([](json_value const& v) { return std::chrono::seconds(v.get_int()); })
        )
        .variable("session-timeout", &http_digest_authentication_application_configuration::session_idle_lifetime, "the session idle lifetime in seconds").default_value(std::chrono::seconds(300))
                .binder([](json_value const& v) { return std::chrono::seconds(v.get_int()); })
        .variable("max-sessions-per-user", &http_digest_authentication_application_configuration::max_sessions_per_user, "max session count per user").default_value(10)
        .variable("404-message", &http_digest_authentication_application_configuration::page404_message, "the message returned if no route found").default_value("The page you requested was not found.")
        .variable("no-route-application", &http_digest_authentication_application_configuration::page404_application, "the application that runs if no route found")
            .binder([](json_value const& v) { return locate<http::application>(v.get_string()); })
        .array("routes", &http_digest_authentication_application_configuration::routes, "routes")
            .binder(sp::parameters_description<http_route>().bind()
                .variable("id", &http_route::id, "route id")
                .variable("application", &http_route::application_name, "http application name").required()
                .variable("pathre", &http_route::pathre, "path regexp").required()
                    .binder([](json_value const& v)->boost::regex {
                        return boost::regex(std::string(v.get_string()));
                    })
            )
    ;
}

}
