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
#include "sonia/utility/linked_buffers.hpp"

namespace sonia { namespace services { namespace net {

class connector
{
public:
    virtual ~connector() noexcept {}

    using buff_ptr = single_linked_buffer_ptr<char>;

    virtual void connect(buff_ptr, size_t, sonia::io::tcp_socket) = 0;
    virtual void connect(buff_ptr, size_t, sonia::io::socket_address const&, sonia::io::udp_weak_socket) = 0;
};

}}}

#endif // SONIA_SERVICES_NET_CONNECTOR_HPP
