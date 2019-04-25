//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_IO_SERVICE_CONFIGURATION_HPP
#define SONIA_SERVICES_IO_SERVICE_CONFIGURATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/cstdint.hpp"
#include "sonia/optional.hpp"
#include "io_ssl_configuration.hpp"

namespace sonia::services {

struct io_service_configuration
{
    uint32_t threads;
    optional<io::ssl_configuration> ssl_configuration;
};

}

#endif // SONIA_SERVICES_IO_SERVICE_CONFIGURATION_HPP
