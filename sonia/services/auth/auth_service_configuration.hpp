//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_AUTH_SERVICE_CONFIGURATION_HPP
#define SONIA_AUTH_SERVICE_CONFIGURATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <vector>
#include <boost/filesystem/path.hpp>

#include "sonia/optional.hpp"
#include "sonia/string.hpp"

namespace sonia::services {

struct auth_service_configuration
{
    optional<boost::filesystem::path> path;
    optional<boost::filesystem::path> dir;

    std::string authenticator_app;
    std::vector<std::pair<std::string, std::string>> bootstrap_users;
};

}

#endif // SONIA_AUTH_SERVICE_CONFIGURATION_HPP
