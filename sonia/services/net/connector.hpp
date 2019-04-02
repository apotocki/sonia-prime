//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_NET_CONNECTOR_HPP
#define SONIA_SERVICES_NET_CONNECTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/services/io/tcp_socket.hpp"
#include "sonia/services/io/udp_socket.hpp"
#include "sonia/services/io/sockets.hpp"

namespace sonia::services::net {

class tcp_connector
{
public:
    virtual ~tcp_connector() = default;

    virtual void connect(array_view<char> buff, size_t rsz, sonia::io::tcp_socket) = 0;
};

class udp_connector
{
public:
    virtual ~udp_connector() = default;

    virtual void connect(array_view<char> buff, size_t rsz, sonia::sal::socket_address const&, sonia::io::udp_socket&) = 0;
};

}

#endif // SONIA_SERVICES_NET_CONNECTOR_HPP
