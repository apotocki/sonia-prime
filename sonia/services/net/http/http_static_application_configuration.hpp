//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_HTTP_STATIC_APPLICATION_CONFIGURATION_HPP
#define SONIA_SERVICES_HTTP_STATIC_APPLICATION_CONFIGURATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <string>
#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/regex.hpp>

#include "sonia/optional.hpp"

namespace sonia::services {

struct http_static_application_configuration
{
    std::string www_path;
    std::string sys_path;
    std::string mime_mapping_file_path;
    optional<std::string> page404app;

	using forward_mapping_type = std::vector<std::pair<boost::regex, std::string>>;

	forward_mapping_type forward_mapping;
};

}

#endif // SONIA_SERVICES_HTTP_STATIC_APPLICATION_CONFIGURATION_HPP
