//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "factory.hpp"

#include "sonia/sal.hpp"
#include "sonia/utility/scope_exit.hpp"

#ifdef BOOST_WINDOWS
#else
#   include <sys/types.h>
#   include <sys/socket.h>
#endif

namespace sonia::io {

void factory::open(uint32_t thr_cnt)
{
    unique_lock lk(close_mtx_);

    initialize_impl(thr_cnt);
    threads_.reserve(thr_cnt);

    for (size_t i = 0; i < thr_cnt; ++i) {
        threads_.emplace_back(thread([impl = impl_holder_] { impl->thread_proc(); }));
        sonia::sal::set_thread_name(threads_.back().get_id(), ("%1% factory thread #%2%"_fmt % name() % i).str());
    }
}

void factory::close() noexcept
{
    unique_lock lk(close_mtx_);
    if (impl_holder_) {
        impl_holder_->close();
        impl_holder_.reset();

        // fiber friendly thread join procedure
        thread([this] {
            for (thread & t : threads_) {
                t.join();
            }
            {
                unique_lock lk(close_mtx_);
                threads_.clear();
            }
            close_cond_.notify_one();
        }).detach();
        close_cond_.wait(lk, [this] { return threads_.empty(); });
    }
}

size_t factory::thread_count() const
{
    return threads_.size();
}

void factory::impl_base::close() noexcept
{
    auto qszval = qsz.load();
    //LOG_TRACE(wrapper->logger()) << "close, queue size: " << qszval;
    if (qszval >= 0) {
        if (0 == qsz.fetch_add(qsz_min_value)) {
            park_threads();
        }
    }
}

void factory::impl_base::on_add_callback()
{
    long qszval = qsz.fetch_add(1);
    //LOG_TRACE(wrapper->logger()) << "add callback, queue size: " << qszval << ", norm: " << (qszval < 0 ? qszval - qsz_min_value : qszval);
    if (qszval < 0) {
        if (qsz_min_value != qszval) {
            on_release_callback();
        } else {
            // if qsz_min_value == qszval then initiate_thread_parking was already called, 
            // so we just keep it actual
            --qsz;
        }
        throw closed_exception("io::factory");
    }
}

void factory::impl_base::on_release_callback() noexcept
{
    auto qszval = qsz.fetch_sub(1);
    //LOG_TRACE(wrapper->logger()) << "release callback, queue size: " << qszval << ", norm: " << (qszval < 0 ? qszval - qsz_min_value : qszval);
    if ((qsz_min_value + 1) == qszval) {
        park_threads();
    }
}

shared_ptr<factory::impl_base> factory::get_dataptr()
{
    auto r = impl_.lock();
    if (!r) throw closed_exception();
    return r;
}

tcp_socket factory::create_bound_tcp_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type ft)
{
    using namespace sonia::sal;

    std::ostringstream errmsgs;

    auto addrsrc = parse_net_address(ft, SOCK_STREAM, 0, address, port);
    for (sonia::sal::addrinfo const*& ai : addrsrc) {
        try {
            socket_handle sock = create_socket(ai->family(), ai->ai_socktype(), ai->ai_protocol());
            SCOPE_EXIT([&sock]() { if (sock != not_initialized_socket_v) close_socket(sock); });

            setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 1);
            bind_socket(sock, ai->ai_addr(), ai->ai_addrlen());
            listen_socket(sock, SOMAXCONN);

            tcp_socket result = get_dataptr()->do_create_tcp_socket(sock, ft);

            sock = not_initialized_socket_v;
            ai = nullptr;
            addrsrc(); // break coroutine

            return result;
        } catch (closed_exception const&) {
            throw;
        } catch (std::exception const& e) {
            if (!errmsgs.str().empty()) errmsgs << '\n';
            errmsgs << e.what();
        }
    }
    throw exception("can't bind socket to the address: '%1%:%2%', error(s): %3%"_fmt % address % port % errmsgs.str());
}

tcp_socket factory::create_connected_tcp_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type ft)
{
    using namespace sonia::sal;

    std::ostringstream errmsgs;
    auto addrsrc = parse_net_address(ft, SOCK_STREAM, 0, address, port);
    for (sonia::sal::addrinfo const*& ai : addrsrc) {
        try {
            socket_handle sock = create_socket(ai->family(), ai->ai_socktype(), ai->ai_protocol());
            SCOPE_EXIT([&sock]() { if (sock != not_initialized_socket_v) close_socket(sock); });

            connect_socket(sock, ai->ai_addr(), ai->ai_addrlen());

            tcp_socket result = get_dataptr()->do_create_tcp_socket(sock, ft);
            
            sock = not_initialized_socket_v;
            ai = nullptr;
            addrsrc(); // break coroutine

            return result;
        } catch (closed_exception const&) {
            throw;
        } catch (std::exception const& e) {
            if (!errmsgs.str().empty()) errmsgs << '\n';
            errmsgs << e.what();
        }
    }
    throw exception("can't connect socket to the address: '%1%:%2%', error(s): %3%"_fmt % address % port % errmsgs.str());
}

udp_socket factory::create_udp_socket(sonia::sal::net_family_type dt)
{
    return get_dataptr()->do_create_udp_socket(create_socket(dt, SOCK_DGRAM, 0), dt);
}

}
