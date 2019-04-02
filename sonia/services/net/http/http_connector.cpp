//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "http_connector.hpp"

#include "sonia/exceptions.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/iterators/socket_write_iterator.hpp"
#include "sonia/utility/iterators/socket_read_iterator.hpp"

namespace sonia::services {

using sonia::io::tcp_socket;

http_connector::http_connector(http_connector_configuration cfg)
    : cfg_(std::move(cfg))
{
    set_log_attribute("Type", "http-connector");
}

void http_connector::open()
{
    cfg_.dos_message = to_string("HTTP/1.1 503 Service Unavailable\r\nContent-Type: text/html\r\nContent-Length: %1%\r\n\r\n%2%"_fmt % cfg_.dos_message.size() % cfg_.dos_message);
    cfg_.page_not_found_message = to_string("HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: %1%\r\n\r\n%2%"_fmt % cfg_.page_not_found_message.size() % cfg_.page_not_found_message);
}

void http_connector::close() noexcept
{
    lock_guard guard(mtx);
    closed_ = true;
    for (auto & soc : using_set_) {
        soc.close();
    }
}

void http_connector::connect(array_view<char> buff, size_t rsz, sonia::io::tcp_socket soc)
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

    if (keep_alive_count_.fetch_add(1) < cfg_.keep_alive_max_count) {
        keep_alive_connect(buff, rsz, std::move(soc));
    } else {
        --keep_alive_count_;
        if (one_shot_count_.fetch_add(1) < cfg_.not_keep_alive_max_count) {
            one_shot_connect(buff, rsz, std::move(soc));
        } else {
            --one_shot_count_;
            socket_write_iterator wit{soc};
            *wit = to_string_view(cfg_.dos_message);
            wit.flush();
        }
    }
}

void http_connector::keep_alive_connect(array_view<char> buff, size_t rsz, sonia::io::tcp_socket soc)
{
    socket_read_iterator rit{soc};
    socket_write_iterator wit{soc};

    *wit = to_string_view(cfg_.page_not_found_message);
    wit.flush();

    //try {
    //    array_view<const char> readybuff = buff.subview(0, rsz);
    //    while (!wit.empty() && !rit.empty()) {
    //        *wit = readybuff; ++wit;
    //        *rit = buff; ++rit;
    //        readybuff = *rit;
    //    }
    //} catch (...) {}    
}

void http_connector::one_shot_connect(array_view<char> buff, size_t rsz, sonia::io::tcp_socket soc)
{
    socket_write_iterator wit{soc};
    *wit = to_string_view(cfg_.page_not_found_message);
    wit.flush();
}

}
