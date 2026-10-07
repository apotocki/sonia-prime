//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "sonia/concurrency.hpp"
#include "sonia/services/service.hpp"
#include "sonia/services/net/connector.hpp"

#include "sonia/utility/iterators/socket_read_input_iterator.hpp"
#include "sonia/utility/iterators/socket_write_input_iterator.hpp"

#include "http_connector_configuration.hpp"

namespace sonia::services {

class http_connector
    : public service
    , public net::tcp_connector
    , public route_selector
{
public:
    explicit http_connector(http_connector_configuration cfg);
    ~http_connector() override;

    void open() override;
    void close() noexcept override;

    void connect(sonia::io::tcp_socket) override;

    void close_connections() noexcept override;

    uint64_t idle_time_ms() const noexcept override;

    // route selector api
    void enable_route(string_view routeid, bool enable_val) override;

private:
    static int64_t steady_now_ms() noexcept;

    struct connection
    {
        explicit connection(io::tcp_socket s) noexcept : soc{ std::move(s) }, last_io_ms{ steady_now_ms() } {}

        io::tcp_socket soc;
        std::atomic<int64_t> last_io_ms;        // steady clock: the last byte read or written, the end of the last request
        std::atomic<bool> in_request{ false };  // io_timeout applies, keep_alive_timeout otherwise
        bool expired{ false };                  // closed by the watchdog (guarded by mtx)
    };

    // the socket the http stream works with: forwards to the connection socket and stamps its activity
    class activity_socket
    {
    public:
        explicit activity_socket(connection& c) noexcept : c_{ c } {}

        template <typename ... ArgsT>
        auto read_some(ArgsT&& ... args) noexcept { return stamp(c_.soc.read_some(std::forward<ArgsT>(args) ...)); }

        template <typename ... ArgsT>
        auto write_some(ArgsT&& ... args) noexcept { return stamp(c_.soc.write_some(std::forward<ArgsT>(args) ...)); }

        void shutdown(io::shutdown_opt opt = io::shutdown_opt::both) { c_.soc.shutdown(opt); }

    private:
        template <typename ResultT>
        ResultT stamp(ResultT r) noexcept
        {
            if (r.has_value() && r.value()) c_.last_io_ms = steady_now_ms();
            return r;
        }

        connection& c_;
    };

    using read_iterator = socket_read_input_iterator<activity_socket>;
    using write_iterator = socket_write_input_iterator<activity_socket>;

    void keep_alive_connect(connection&);
    void one_shot_connect(connection&);
    bool do_connection(connection&, read_iterator &, write_iterator &, bool keep_alive); // returns true if keep alive

    // watchdog: closes connections that exceeded keep_alive_timeout / io_timeout;
    // a plain thread (not a timer) so that close() can reliably stop it, sleeps while there are no connections
    void watchdog_proc() noexcept;
    void close_expired() noexcept;
    void stop_watchdog() noexcept;

    http_connector_configuration cfg_;

    mutable fibers::mutex routes_mutex_;
    mutable fibers::mutex mtx;
    std::list<connection> using_set_;
    bool closed_{false};

    std::atomic<size_t> keep_alive_count_{0};
    std::atomic<size_t> one_shot_count_{0};

    // activity tracking (idle_time_ms): requests being handled, steady clock time of the last completed one
    std::atomic<size_t> active_requests_{0};
    std::atomic<int64_t> last_activity_ms_;

    std::thread watchdog_;
    std::mutex watchdog_mtx_;
    std::condition_variable watchdog_cnd_;
    size_t watched_count_{0}; // connections in using_set_ (guarded by watchdog_mtx_)
    bool watchdog_stop_{false};
};

}
