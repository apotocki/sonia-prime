//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_HTTP_DEFAULT_APPLICATION_CONFIGURATION_HPP
#define SONIA_SERVICES_HTTP_DEFAULT_APPLICATION_CONFIGURATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <string>
#include "sonia/optional.hpp"
#include "sonia/net/http/status_codes.hpp"

namespace sonia::services {

struct http_default_application_configuration
{
    http::status response_code;
    std::string title;
    optional<std::string> image;
    bool show_headers;
};

}

#endif // SONIA_SERVICES_HTTP_DEFAULT_APPLICATION_CONFIGURATION_HPP
