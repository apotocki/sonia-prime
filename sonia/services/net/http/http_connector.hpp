//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <list>

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

    void open() override;
    void close() noexcept override;

    void connect(sonia::io::tcp_socket) override;
    void keep_alive_connect(sonia::io::tcp_socket soc);
    void one_shot_connect(sonia::io::tcp_socket soc);

    void close_connections() noexcept override;

    // route selector api
    void enable_route(string_view routeid, bool enable_val) override;

private:
    using read_iterator = socket_read_input_iterator<io::tcp_socket>;
    using write_iterator = socket_write_input_iterator<io::tcp_socket>;

    bool do_connection(read_iterator &, write_iterator &); // returns true if keep alive

    http_connector_configuration cfg_;

    fibers::mutex mtx;
    std::list<sonia::io::tcp_socket> using_set_;
    bool closed_{false};

    std::atomic<size_t> keep_alive_count_{0};
    std::atomic<size_t> one_shot_count_{0};
};

}
