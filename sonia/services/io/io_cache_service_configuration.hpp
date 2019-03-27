//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_IO_CACHE_SERVICE_CONFIGURATION_HPP
#define SONIA_SERVICES_IO_CACHE_SERVICE_CONFIGURATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"

namespace sonia::services {

class io_cache_service_configuration
{
public:
    std::string tcp_socket_factory;
    uint16_t connection_count;
    uint16_t per_route_connection_count;
};

}

#endif // SONIA_SERVICES_IO_CACHE_SERVICE_CONFIGURATION_HPP
