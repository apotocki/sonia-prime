//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_NET_SERVICE_HPP
#define SONIA_SERVICES_NET_SERVICE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <vector>

#include "sonia/services/service.hpp"
#include "sonia/services/io/udp_socket.hpp"
#include "sonia/services/io/sockets.hpp"
#include "sonia/services/scheduler/scheduler.hpp"

#include "net_service_configuration.hpp"
#include "connector.hpp"

namespace sonia::services {

class net_service 
    : public service
    , public enable_shared_from_this<net_service>
{
    using buff_ptr = single_linked_buffer_ptr<char>;

public:
    explicit net_service(net_service_configuration const& cfg);

    void open() override;
    void close() noexcept override;

    struct listener
    {
        virtual ~listener() {}
        virtual void close() = 0;
    };

private:
    net_service_configuration cfg_;
    shared_ptr<sonia::io::tcp_socket_factory_type> tcp_socket_factory_;
    shared_ptr<sonia::io::udp_socket_factory_type> udp_socket_factory_;
    shared_ptr<scheduler> scheduler_;
    std::vector<shared_ptr<listener>> listeners_;
};

}

#endif // SONIA_SERVICES_NET_SERVICE_HPP
