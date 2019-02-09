//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "net_service.hpp"

#include "sonia/services.hpp"

namespace sonia { namespace services {

using sonia::io::tcp_socket;
using sonia::io::tcp_acceptor;
using sonia::io::tcp_acceptor_factory;

class listener_task : public scheduler_task
{
    shared_ptr<net::connector> cn_;
    single_linked_buffer_ptr<char> buff_;
    size_t sz_;
    sonia::io::tcp_socket soc_;

public:
    template <class CnT, class BuffT>
    listener_task(CnT && cn, BuffT && buff, size_t sz, sonia::io::tcp_socket soc)
        : cn_(std::forward<CnT>(cn)), buff_(std::forward<BuffT>(buff)), sz_(sz), soc_(std::move(soc))
    {}

    void run() override
    {
        cn_->connect(std::move(buff_), sz_, std::move(soc_));
    }

    polymorphic_movable* move(void* address, size_t sz) override
    {
        BOOST_ASSERT(sz >= sizeof(listener_task));
        new (address) listener_task(std::move(*this));
        return reinterpret_cast<listener_task*>(address);
    }
};

net_service::net_service(net_service_configuration const& cfg)
    : cfg_(cfg)
{
    set_log_attribute("Type", "net-server");
    locate(cfg.acceptor_factory, acceptor_factory_);
    locate(cfg.scheduler, scheduler_);
}

void net_service::open()
{
    listeners_.reserve(cfg_.listeners.size());
    for (net::listener_configuration const& lc : cfg_.listeners) {
        shared_ptr<listener> ls = make_shared<listener>();
        ls->sched = scheduler_;
        locate(lc.connector, ls->cn);
        
        if (net::listener_type::TCP == lc.type || net::listener_type::SSL == lc.type) {
            ls->acceptor = acceptor_factory_->create_tcp_acceptor(
                lc.address, lc.port,
                net::listener_type::TCP == lc.type ? sonia::io::tcp_socket_type::TCP : sonia::io::tcp_socket_type::SSL,
                [ls, bfsz = lc.buffer_size](tcp_acceptor_factory::connection_factory_t const& c) {
                    auto buff = make_single_linked_buffer<char>(bfsz);
                    auto av = buff->to_array_view();
                    c(av, [ls, abuff = std::move(buff)](tcp_socket soc, size_t rsize) {
                        ls->sched->post(scheduler_task_t(
                            in_place_type_t<listener_task>(),
                            ls->cn, std::move(abuff), rsize, std::move(soc)
                        ));
                    });
                }
            );
        }

        listeners_.push_back(std::move(ls));
    }
}

void net_service::close() noexcept
{
    for (auto & lp : listeners_) {
        lp->close();
    }
    listeners_.clear();
}

}}
