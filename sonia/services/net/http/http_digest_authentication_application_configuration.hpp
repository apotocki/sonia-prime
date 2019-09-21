//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_HTTP_DIGEST_AUTHENTICATION_APPLICATION_CONFIGURATION_HPP
#define SONIA_SERVICES_HTTP_DIGEST_AUTHENTICATION_APPLICATION_CONFIGURATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <chrono>
#include <vector>
#include "sonia/string.hpp"
#include "http_route.hpp"

namespace sonia::services {

struct http_digest_authentication_application_configuration
{
    std::string auth_app;
    //std::string digest_opaque;
    std::chrono::seconds digest_nonce_timeout;
    std::chrono::seconds session_idle_lifetime;
    size_t max_sessions_per_user;
    std::vector<http_route> routes;

    std::string page404_message;
    shared_ptr<http::application> page404_application;
};

}

#endif // SONIA_SERVICES_HTTP_DIGEST_AUTHENTICATION_APPLICATION_CONFIGURATION_HPP
