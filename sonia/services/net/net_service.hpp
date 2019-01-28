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
#include "sonia/services/io/tcp_acceptor.hpp"
#include "sonia/services/scheduler/scheduler.hpp"

#include "sonia/utility/linked_buffers.hpp"

#include "net_service_configuration.hpp"
#include "connector.hpp"

namespace sonia { namespace services {

class net_service 
    : public service
    , public enable_shared_from_this<net_service>
{
    using buff_ptr = single_linked_buffer_ptr<char>;

public:
    explicit net_service(net_service_configuration const& cfg);

    void open() override;
    void close() noexcept override;

private:
    struct listener : public loggable
    {
        shared_ptr<net::connector> cn;
        sonia::io::tcp_acceptor acceptor;
        buff_ptr buff;
        bool closing;
        explicit listener(logger::logger_ptr lptr) : loggable(lptr), closing(false) {}
        //listener(shared_ptr<net::connector> ncn) : cn(std::move(ncn)) {}

        void acceptor_proc(std::error_code const&, size_t, sonia::io::tcp_socket soc, sonia::io::tcp_acceptor::renew_functor const&, shared_ptr<scheduler>);

        void close()
        {
            closing = true;
            acceptor.close();
        }
    };

    net_service_configuration cfg_;
    shared_ptr<sonia::io::tcp_acceptor_factory> acceptor_factory_;
    shared_ptr<scheduler> scheduler_;
    std::vector<shared_ptr<listener>> listeners_;
};

}}

#endif // SONIA_SERVICES_NET_SERVICE_HPP
