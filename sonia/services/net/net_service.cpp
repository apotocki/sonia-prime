//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "net_service.hpp"

#include "sonia/services.hpp"

namespace sonia { namespace services {

using sonia::io::tcp_socket;
using sonia::io::tcp_socket_factory;
using tcp_connection_handler_type = sonia::io::tcp_connection_handler_t<sonia::io::socket_traits>;

using sonia::io::udp_socket;
using sonia::io::udp_weak_socket;
using udp_connection_handler_type = sonia::io::udp_connection_handler_t<sonia::io::socket_traits>;

//class listener_task : public scheduler_task
//{
//    shared_ptr<net::connector> cn_;
//    single_linked_buffer_ptr<char> buff_;
//    size_t sz_;
//    sonia::io::tcp_socket soc_;
//
//public:
//    template <class CnT, class BuffT>
//    listener_task(CnT && cn, BuffT && buff, size_t sz, sonia::io::tcp_socket soc)
//        : cn_(std::forward<CnT>(cn)), buff_(std::forward<BuffT>(buff)), sz_(sz), soc_(std::move(soc))
//    {}
//
//    void run() override
//    {
//        cn_->connect(std::move(buff_), sz_, std::move(soc_));
//    }
//
//    polymorphic_movable* move(void* address, size_t sz) override
//    {
//        BOOST_ASSERT(sz >= sizeof(listener_task));
//        new (address) listener_task(std::move(*this));
//        return reinterpret_cast<listener_task*>(address);
//    }
//};

struct tcp_acceptor_listener : net_service::listener
{
    shared_ptr<net::connector> cn;
    sonia::io::tcp_socket sock;

    void close() override { sock.close(); }
};

struct udp_socket_listener : net_service::listener
{
    shared_ptr<net::connector> cn;
    sonia::io::udp_socket sock;

    void close() override { sock.close(); }
};

net_service::net_service(net_service_configuration const& cfg)
    : cfg_(cfg)
{
    set_log_attribute("Type", "net-server");
    //locate(cfg.scheduler, scheduler_);
}

void net_service::open()
{
    listeners_.reserve(cfg_.listeners.size());
    for (net::listener_configuration const& lc : cfg_.listeners)
    {
        if (net::listener_type::TCP == lc.type || net::listener_type::SSL == lc.type) {
            if (!tcp_socket_factory_) {
                locate(cfg_.tcp_factory, tcp_socket_factory_);
            }
            shared_ptr<tcp_acceptor_listener> ls = make_shared<tcp_acceptor_listener>();
            locate(lc.connector, ls->cn);
            ls->sock = tcp_socket_factory_->create_tcp_socket(net::listener_type::TCP == lc.type ? sonia::io::tcp_socket_type::TCP : sonia::io::tcp_socket_type::SSL);
            ls->sock.bind(lc.address, lc.port);
            ls->sock.listen(
                [ls, bfsz = lc.buffer_size](tcp_connection_handler_type const& c) {
                    auto buff = make_single_linked_buffer<char>(bfsz);
                    auto av = buff->to_array_view();
                    c(av, [ls, abuff = std::move(buff)](tcp_socket soc, size_t rsize) mutable {
                        ls->cn->connect(std::move(abuff), rsize, std::move(soc));
                        //ls->sched->post(scheduler_task_t(
                        //    in_place_type_t<listener_task>(),
                        //    ls->cn, std::move(abuff), rsize, std::move(soc)
                        //));
                    });
                });

            listeners_.push_back(std::move(ls));
        } else if (net::listener_type::UDP == lc.type) {
            if (!udp_socket_factory_) {
                locate(cfg_.udp_factory, udp_socket_factory_);
            }
            shared_ptr<udp_socket_listener> ls = make_shared<udp_socket_listener>();
            locate(lc.connector, ls->cn);
            ls->sock = udp_socket_factory_->create_udp_socket();
            ls->sock.bind(lc.address, lc.port);
            ls->sock.listen([ls, bfsz = lc.buffer_size](udp_connection_handler_type const& h) {
                auto buff = make_single_linked_buffer<char>(bfsz);
                auto av = buff->to_array_view();
                h(av, [ls, abuff = std::move(buff)](size_t rsize, sonia::io::socket_address const& address, udp_weak_socket s) mutable {
                    ls->cn->connect(std::move(abuff), rsize, address, std::move(s));
                });
            });
            listeners_.push_back(std::move(ls));
        }
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
