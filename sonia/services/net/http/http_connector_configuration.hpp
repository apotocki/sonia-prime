//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_HTTP_CONNECTOR_CONFIGURATION_HPP
#define SONIA_SERVICES_HTTP_CONNECTOR_CONFIGURATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <vector>
#include "sonia/optional.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/net/http/application.hpp"

#include "http_route.hpp"

namespace sonia::services {

struct http_connector_configuration
{
    size_t keep_alive_max_count;
    size_t not_keep_alive_max_count;
    size_t request_buffer_size;
    size_t response_buffer_size;
    // keep_alive_timeout;
    std::string dos_message;
    std::vector<http_route> routes;
    std::string page404_message;
    optional<std::string> page404_application_name;
    mutable shared_ptr<http::application> page404_application;
};

}

#endif // SONIA_SERVICES_HTTP_CONNECTOR_CONFIGURATION_HPP
