//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "http_connector.hpp"

#include <boost/algorithm/string/predicate.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/iterators/socket_write_iterator.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"
#include "sonia/utility/iterators/reference_wrapper_iterator.hpp"
#include "sonia/utility/iterators/iterator_of_ranges_with_limit.hpp"
#include "sonia/utility/iterators/chain_iterator.hpp"
#include "sonia/net/http/message.ipp"
#include "sonia/utility/serialization/http_request.hpp"
#include "sonia/utility/serialization/http_response.hpp"

#include "sonia/services.hpp"

namespace sonia::services {

using sonia::io::tcp_socket;
using namespace sonia::http;

http_connector::http_connector(http_connector_configuration cfg)
    : cfg_(std::move(cfg))
{
    set_log_attribute("Type", "http-connector");
}

void http_connector::open()
{
    cfg_.dos_message = to_string("HTTP/1.1 503 Service Unavailable\r\nContent-Type: text/html\r\nContent-Length: %1%\r\n\r\n%2%"_fmt % cfg_.dos_message.size() % cfg_.dos_message);
    if (cfg_.page404_application_name) {
        locate(*cfg_.page404_application_name, cfg_.page404_application);
    } else {
        cfg_.page404_message = to_string("HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: %1%\r\n\r\n%2%"_fmt % cfg_.page404_message.size() % cfg_.page404_message);
    }
    for (auto const& r : cfg_.routes) {
         locate(r.application_name, r.application);
    }
}

void http_connector::close() noexcept
{
    lock_guard guard(mtx);
    closed_ = true;
    for (auto & soc : using_set_) {
        soc.close();
    }
}

void http_connector::connect(sonia::io::tcp_socket soc)
{
    std::list<io::tcp_socket>::iterator soc_it;
    {
        lock_guard guard(mtx);
        if (closed_) throw closed_exception();
        using_set_.push_back(soc);
        soc_it = using_set_.end();
        --soc_it;
    }
    SCOPE_EXIT([this, soc_it]() {
        lock_guard guard(mtx);
        using_set_.erase(soc_it);
    });

    try {
        if (keep_alive_count_.fetch_add(1) < cfg_.keep_alive_max_count) {
            SCOPE_EXIT([this]{ --keep_alive_count_; });
            keep_alive_connect(std::move(soc));
        } else {
            --keep_alive_count_;
            if (one_shot_count_.fetch_add(1) < cfg_.not_keep_alive_max_count) {
                SCOPE_EXIT([this]{ --one_shot_count_; });
                one_shot_connect(std::move(soc));
            } else {
                --one_shot_count_;
                socket_write_iterator wit{soc};
                *wit = to_string_view(cfg_.dos_message);
                wit.flush();
            }
        }
    } catch (eof_exception const&) {
    } catch (closed_exception const&) {
    } catch (std::exception const& err) {
        LOG_ERROR(logger()) << err.what();
    }
}

bool http_connector::do_connection(read_iterator & ii, write_iterator & oi)
{
    bool handled = false;
    http::request req;
    auto it = decode<serialization::default_t>(range_dereferencing_iterator{reference_wrapper_iterator{ii}}, req);
    it.flush();

    req.build_input_iterator(ii);
    
    cstring_view uri = req.get_relative_uri();
    for (auto const& r : cfg_.routes) {
        if (regex_match(uri.c_str(), r.pathre)) {
            http::response resp;
            r.application->handle(req, resp);
            encode<serialization::default_t>(resp, reference_wrapper_iterator(oi));
            handled = true;
            break;
        }
    }
    if (!handled) {
        if (cfg_.page404_application) {
            http::response resp;
            cfg_.page404_application->handle(req, resp);
            encode<serialization::default_t>(resp, reference_wrapper_iterator(oi));
        } else {
            copy_range(string_view(cfg_.page404_message), std::move(oi)).flush();
            return false;
        }
    }

    auto hvals = req.get_header(http::header::CONNECTION);
    //return false;
    return (hvals.size() == 1 && boost::iequals(hvals[0], "keep-alive"));
}

void http_connector::keep_alive_connect(io::tcp_socket soc)
{
    std::vector<char> buff(cfg_.response_buffer_size + cfg_.request_buffer_size);
    array_view respbuff{&buff.front(), cfg_.response_buffer_size};
    array_view reqbuff{&buff.front() + cfg_.response_buffer_size, cfg_.request_buffer_size};

    write_iterator oi{soc, respbuff};
    read_iterator ii{soc, reqbuff, (size_t)0};

    static std::atomic<int> keep_alive_connnum{0};
    int curconnection = keep_alive_connnum.fetch_add(1);

    while (do_connection(ii, oi)) {
        oi.flush();
        LOG_TRACE(logger()) << "next keep alive connection: " << curconnection;
    }
}

void http_connector::one_shot_connect(sonia::io::tcp_socket soc)
{
    std::vector<char> buff(cfg_.response_buffer_size + cfg_.request_buffer_size);
    array_view respbuff{&buff.front(), cfg_.response_buffer_size};
    array_view reqbuff{&buff.front() + cfg_.response_buffer_size, cfg_.request_buffer_size};

    write_iterator oi{soc, respbuff};
    read_iterator ii{soc, reqbuff, (size_t)0};

    do_connection(ii, oi);
}

}
