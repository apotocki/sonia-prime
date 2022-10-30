//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVIVES_TRANSCEIVER_SERVICE_HPP
#define SONIA_SERVIVES_TRANSCEIVER_SERVICE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <list>
#include "sonia/concurrency.hpp"

#include "sonia/services/service.hpp"
#include "sonia/services/transceiver.hpp"
#include "sonia/services/net/connector.hpp"

#include "transceiver_configuration.hpp"

namespace sonia::services {

class transceiver_service
    : public transceiver
    , public service
    , public net::tcp_connector
    , public net::udp_connector
{
public:
    explicit transceiver_service(transceiver_service_configuration const&);

    void close() noexcept override;

    // transceiver api
    void transmit_and_receive(string_view dest, serializable_proxy_object & obj) override;
    //void async_send(string_view dest, command<void> &)  override;
    //void invoke_raw(string_view dest, abstract_command_base & cmd, void * p_opt_future) override;

    void close_connections() noexcept override;

    void connect(sonia::io::tcp_socket) override;
    void connect(array_view<char> buff, size_t, sonia::sal::socket_address const&, sonia::io::udp_socket&) override;

private:
    transceiver_service_configuration cfg_;
    shared_ptr<sonia::io::tcp_socket_factory_type> soc_factory_;

    fibers::mutex mtx;
    std::list<sonia::io::tcp_socket> using_set_;
    bool closed_{false};
};

}

#endif // SONIA_SERVIVES_TRANSCEIVER_SERVICE_HPP
