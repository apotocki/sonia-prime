//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_ECHO_CONNECTOR_HPP
#define SONIA_SERVICES_ECHO_CONNECTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/services/service.hpp"
#include "sonia/services/net/connector.hpp"

#include "echo_connector_configuration.hpp"

namespace sonia::services {

class echo_connector
    : public service
    , public net::tcp_connector
    , public net::udp_connector
{
public:
    explicit echo_connector(echo_connector_configuration const& cfg);

    void open() override;
    void close() noexcept override;

    void connect(sonia::io::tcp_socket) override;
    void connect(array_view<char> buff, size_t, sonia::sal::socket_address const&, sonia::io::udp_socket&) override;

private:
    echo_connector_configuration cfg_;
};

}

#endif // SONIA_SERVICES_ECHO_CONNECTOR_HPP
