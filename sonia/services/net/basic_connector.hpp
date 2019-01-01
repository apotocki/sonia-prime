//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_NET_BASIC_CONNECTOR_HPP
#define SONIA_SERVICES_NET_BASIC_CONNECTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "connector.hpp"

namespace sonia { namespace services { namespace net {

class basic_connector : public connector {
public:
    class connection {
        sonia::io::tcp_socket soc;
    };

    void connect(buff_ptr, size_t, sonia::io::tcp_socket) override;

};

}}}
#endif // SONIA_SERVICES_NET_BASIC_CONNECTOR_HPP
