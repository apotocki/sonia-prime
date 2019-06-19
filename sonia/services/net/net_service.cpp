//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "net_service.hpp"

#include <boost/exception/diagnostic_information.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "sonia/services.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/utility/scope_exit.hpp"

#include "connector.hpp"

namespace sonia::services {

using sonia::io::tcp_socket;
using sonia::io::udp_socket;

using namespace std::chrono_literals;

struct tcp_acceptor_listener : net_service::listener
{
    shared_ptr<net::tcp_connector> cn;
    sonia::io::tcp_server_socket sock;
    void close() override { closed.store(true); sock.close(); }
};

struct udp_socket_listener : net_service::listener
{
    shared_ptr<net::udp_connector> cn;
    sonia::io::udp_socket sock;
    void close() override { closed.store(true); sock.close(); }
};

class acceptor_task : public scheduler_task
{
    shared_ptr<tcp_acceptor_listener> al_;
    shared_ptr<scheduler> sched_;

public:
    acceptor_task(shared_ptr<tcp_acceptor_listener> al, shared_ptr<scheduler> sched)
        : al_(std::move(al)), sched_(std::move(sched))
    {}

    void run() override
    {
#if 1
        SCOPE_EXIT([this]{--al_->workers_count;});
        GLOBAL_LOG_TRACE() << "new acceptor threadid: " << this_thread::get_id() << ", fiberid: " << this_fiber::get_id();

        for (;;) try {
            auto sock = al_->sock.accept().get();
            //GLOBAL_LOG_TRACE() << "after accept" << this_thread::get_id() << ", fiberid: " << this_fiber::get_id();
            // if there is no waiting workers and current number of workers is less than configured max count then create a new worker
            if (0 == al_->sock.accepting_count()) {
                if (al_->workers_max > al_->workers_count.fetch_add(1)) {
                    sched_->post(0ms, scheduler_task_t(in_place_type_t<acceptor_task>(), al_, sched_));
                } else {
                    --al_->workers_count;
                }
            }

            al_->cn->connect(std::move(sock));

        } catch (eof_exception const&) { // on socket closing
            //GLOBAL_LOG_TRACE() << "acceptor_task accept eof_exception";
            if (al_->closed.load()) break;
        } catch (closed_exception const&) {
            //GLOBAL_LOG_TRACE() << "acceptor_task accept closed_exception";
            throw;
        } catch (...) {
            GLOBAL_LOG_TRACE() << "acceptor error: " << boost::trim_right_copy(boost::current_exception_diagnostic_information());
            if (al_->closed.load()) break;
        }
#else
        GLOBAL_LOG_TRACE() << "new acceptor threadid: " << this_thread::get_id() << ", fiberid: " << this_fiber::get_id();
        for (;;) {
            try {
                auto sock = al_->sock.accept().get();
                sched_->post([al = al_, sock = std::move(sock)]() mutable {
                    try {
                        al->cn->connect(std::move(sock));
                    } catch (...) {
                        GLOBAL_LOG_TRACE() << "acceptor error: " << boost::trim_right_copy(boost::current_exception_diagnostic_information());
                    }
                });
            } catch (closed_exception const&) {
                throw;
            } catch (...) {
                GLOBAL_LOG_TRACE() << "acceptor error: " << boost::trim_right_copy(boost::current_exception_diagnostic_information());
                if (al_->closed.load()) break;
            }
        }
#endif
    }

    polymorphic_movable* move(void* address, size_t sz) override
    {
        BOOST_ASSERT(sz >= sizeof(acceptor_task));
        new (address) acceptor_task(std::move(*this));
        return reinterpret_cast<acceptor_task*>(address);
    }
};

class listener_task : public scheduler_task
{
    shared_ptr<udp_socket_listener> al_;
    shared_ptr<scheduler> sched_;

public:
    listener_task(shared_ptr<udp_socket_listener> al, shared_ptr<scheduler> sched)
        : al_(std::move(al)), sched_(std::move(sched))
    {}

    void run() override
    {
        SCOPE_EXIT([this]{--al_->workers_count;});
        GLOBAL_LOG_TRACE() << "new udp listener socket threadid: " << this_thread::get_id() << ", fiberid: " << this_fiber::get_id() << ", buff: " << al_->buffer_size;
        std::vector<char> buff(al_->buffer_size);
        char *buffstart = buff.empty() ? nullptr : &buff.front();

        try {
            for (;;) {
                sonia::sal::socket_address sa;
                auto res = al_->sock.read_some(buffstart, buff.size(), sa);
                if (!res.has_value()) throw eof_exception(res.error().message());
                if (0 == al_->sock.waiting_count()) {
                    if (al_->workers_max > al_->workers_count.fetch_add(1)) {
                        sched_->post(0ms, scheduler_task_t(in_place_type_t<listener_task>(), al_, sched_));
                    } else {
                        --al_->workers_count;
                    }
                }
                try {
                    al_->cn->connect(to_array_view(buff), res.value(), sa, al_->sock);
                } catch (eof_exception const&) { // on socket closing
                } catch (closed_exception const&) { // on host termination
                } catch (...) {
                    GLOBAL_LOG_WARN() << boost::trim_right_copy(boost::current_exception_diagnostic_information());
                }
            }

        } catch (eof_exception const&) { // on socket closing just quit the thread
        } catch (closed_exception const&) { // on host termination just quit the thread
        } catch (...) {
            GLOBAL_LOG_ERROR() << "acceptor terminated unexpectedly, cause: " << boost::trim_right_copy(boost::current_exception_diagnostic_information());
        }
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
    locate(cfg.scheduler, scheduler_);
}

void net_service::open()
{
    listeners_.reserve(cfg_.listeners.size());
    try {
        for (net::listener_configuration const& lc : cfg_.listeners)
        {
            if (net::listener_type::TCP == lc.type || net::listener_type::SSL == lc.type) {
                if (!tcp_server_socket_factory_) {
                    locate(cfg_.tcp_server_socket_factory, tcp_server_socket_factory_);
                }
                shared_ptr<tcp_acceptor_listener> ls = make_shared<tcp_acceptor_listener>();
                locate(lc.connector, ls->cn);
                ls->workers_max = lc.workers_count;
                ls->buffer_size = lc.buffer_size;
                ls->sock = tcp_server_socket_factory_->create_server_socket(to_string_view(lc.address), lc.port, lc.family);
                scheduler_->post(0ms, scheduler_task_t(in_place_type_t<acceptor_task>(), ls, scheduler_));
                
                listeners_.push_back(std::move(ls));
            } else if (net::listener_type::UDP == lc.type) {
                if (!udp_socket_factory_) {
                    locate(cfg_.udp_socket_factory, udp_socket_factory_);
                }

                shared_ptr<udp_socket_listener> ls = make_shared<udp_socket_listener>();
                locate(lc.connector, ls->cn);
                ls->workers_max = lc.workers_count;
                ls->buffer_size = lc.buffer_size;
                ls->sock = udp_socket_factory_->create_udp_socket(lc.family);
                ls->sock.bind(lc.address, lc.port);
                scheduler_->post(0ms, scheduler_task_t(in_place_type_t<listener_task>(), ls, scheduler_));
                
                listeners_.push_back(std::move(ls));
            }
        }
    } catch (...) {
        close();
        throw;
    }
}

void net_service::close() noexcept
{
    for (auto & lp : listeners_) {
        lp->close();
    }
    listeners_.clear();
}

}
