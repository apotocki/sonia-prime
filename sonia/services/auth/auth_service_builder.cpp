//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "auth_service_builder.hpp"

namespace sonia::services {

void auth_service_builder::open()
{
    namespace sp = sonia::parameters;
    using user_tpl_t = std::pair<std::string, std::string>;

    parameters_.bind()
        .variable("path", &auth_service_configuration::path, "path used to store the auth service state")
            .binder([](json_value const& v) { return fs::path(v.get_u8string()); })
        .variable("dir", &auth_service_configuration::dir, "dir used to store the auth service state")
            .binder([](json_value const& v) { return fs::path(v.get_u8string()); })
        .variable("authenticator", &auth_service_configuration::authenticator_app, "authenticator application").required()
        .array("users", &auth_service_configuration::bootstrap_users, "bootstrap users")
            .binder(sp::parameters_description<user_tpl_t>().bind()
                .variable("login", &user_tpl_t::first, "user login")
                .variable("password", &user_tpl_t::second, "user password")
            )
    ;
}

}
