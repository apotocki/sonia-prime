//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>

#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"
#include "sonia/sal/net.hpp"

namespace sonia::services::net {

enum class listener_type
{
    TCP = 0,
    SSL,
    UDP
};

struct listener_configuration
{
    std::string connector;
    sonia::sal::net_family_type family;
    std::string address;
    uint16_t port;
    listener_type type;
    size_t buffer_size;
    size_t workers_count;
};

}

namespace sonia::services {

struct net_service_configuration
{
    std::string udp_socket_factory;
    std::string tcp_server_socket_factory;
    std::string scheduler;
    std::vector<net::listener_configuration> listeners;
};

}
