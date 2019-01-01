//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "net_service.hpp"

#include "sonia/services.hpp"

namespace sonia { namespace services {

using sonia::io::tcp_socket;
using sonia::io::tcp_acceptor;

net_service::net_service(net_service_configuration const& cfg)
    : cfg_(cfg)
{
    set_attribute("Type", "net-server");
    locate(cfg.acceptor_factory, acceptor_factory_);
    locate(cfg.scheduler, scheduler_);
}

void net_service::open() {
    listeners_.reserve(cfg_.listeners.size());
    for (net::listener_configuration const& lc : cfg_.listeners) {
        shared_ptr<listener> ls = make_shared<listener>(logger());
        locate(lc.connector, ls->cn);
        
        if (net::listener_type::TCP == lc.type || net::listener_type::SSL == lc.type) {
            ls->acceptor = acceptor_factory_->create_tcp_acceptor(
                lc.address, lc.port,
                net::listener_type::TCP == lc.type ? sonia::io::tcp_socket_type::TCP : sonia::io::tcp_socket_type::SSL);

            ls->buff = make_single_linked_buffer<char>(lc.buffer_size);

            ls->acceptor.async_accept_and_read_some(
                ls->buff->to_array_view(),
                [schd = scheduler_, ls](std::error_code const& err, size_t sz, tcp_socket soc, tcp_acceptor::renew_functor const& rfunc) mutable {
                    ls->acceptor_proc(err, sz, std::move(soc), rfunc, std::move(schd));
                }
            );
            //shared_ptr<listener> ls = make_shared<listener>(std::move(cn));
        }

        listeners_.push_back(std::move(ls));
    }
}

void net_service::close() noexcept {
    for (auto & lp : listeners_) {
        lp->close();
    }
    listeners_.clear();
}

class listener_task : public scheduler_task {
    shared_ptr<net::connector> cn_;
    single_linked_buffer_ptr<char> buff_;
    size_t sz_;
    sonia::io::tcp_socket soc_;

public:
    template <class CnT, class BuffT>
    listener_task(CnT && cn, BuffT && buff, size_t sz, sonia::io::tcp_socket soc)
        : cn_(std::forward<CnT>(cn)), buff_(std::forward<BuffT>(buff)), sz_(sz), soc_(std::move(soc))
    {}

    void run() override {
        cn_->connect(std::move(buff_), sz_, std::move(soc_));
    }
};

void net_service::listener::acceptor_proc(std::error_code const& err, size_t sz, sonia::io::tcp_socket soc, sonia::io::tcp_acceptor::renew_functor const& rfunc, shared_ptr<scheduler> sched)
{
    if (!err) {
        buff_ptr cur_buff_ptr = std::move(buff);
        buff = make_single_linked_buffer<char>(cur_buff_ptr->size());
        rfunc(buff->begin(), buff->size());

        sched->post(scheduler_task_t(
            in_place_type_t<listener_task>(),
            cn, std::move(cur_buff_ptr), sz, std::move(soc)
        ));

    } else {
        LOG_SEV(logger(), closing ? sonia::logger::severity_level::trace : sonia::logger::severity_level::error)
            << err.message();
    }
}

}}
