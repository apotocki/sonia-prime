//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_REGISTRY_SERVICE_CONFIGURATION_HPP
#define SONIA_REGISTRY_SERVICE_CONFIGURATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/filesystem/path.hpp>

#include "sonia/optional.hpp"

namespace sonia::services {

struct registry_service_configuration
{
    optional<boost::filesystem::path> path;
    optional<boost::filesystem::path> dir;
};

}

#endif // SONIA_REGISTRY_SERVICE_CONFIGURATION_HPP
