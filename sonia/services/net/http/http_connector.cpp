//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "http_connector.hpp"

#include <chrono>

#include <boost/algorithm/string/predicate.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/iterators/socket_write_iterator.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"
#include "sonia/utility/iterators/reference_wrapper_iterator.hpp"
#include "sonia/utility/iterators/chain_iterator.hpp"
#include "sonia/net/http/message.ipp"
#include "sonia/utility/serialization/http_request.hpp"
#include "sonia/utility/serialization/http_response.hpp"

#include "sonia/services.hpp"

namespace sonia::services {

using sonia::io::tcp_socket;
using namespace sonia::http;

int64_t http_connector::steady_now_ms() noexcept
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

http_connector::http_connector(http_connector_configuration cfg)
    : cfg_(std::move(cfg))
    , last_activity_ms_{ steady_now_ms() }
{
    set_log_attribute("Type", "http-connector");
}

http_connector::~http_connector()
{
    stop_watchdog(); // if close() was not called
}

void http_connector::open()
{
    cfg_.dos_message = to_string("HTTP/1.1 503 Service Unavailable\r\nContent-Type: text/html\r\nContent-Length: %1%\r\n\r\n%2%"_fmt % cfg_.dos_message.size() % cfg_.dos_message);
    if (cfg_.page404_application_name) {
        locate(*cfg_.page404_application_name, cfg_.page404_application);
    } else {
        cfg_.page404_message = to_string("HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: %1%\r\n\r\n%2%"_fmt % cfg_.page404_message.size() % cfg_.page404_message);
    }
    stop_watchdog(); // reopening
    watchdog_stop_ = false;
    watchdog_ = std::thread([this] { watchdog_proc(); });
}

void http_connector::close() noexcept
{
    stop_watchdog();
    lock_guard guard(mtx);
    closed_ = true;
    for (auto & c : using_set_) {
        c.soc.close();
    }
}

void http_connector::close_connections() noexcept
{
    lock_guard guard(mtx);
    for (auto& c : using_set_) {
        c.soc.close();
    }
}

void http_connector::stop_watchdog() noexcept
{
    {
        std::lock_guard lk(watchdog_mtx_);
        watchdog_stop_ = true;
    }
    watchdog_cnd_.notify_all();
    if (watchdog_.joinable()) watchdog_.join();
}

void http_connector::watchdog_proc() noexcept
{
    // checks a few times per the shortest timeout; precision is not important here
    std::chrono::milliseconds period = (std::max)(std::chrono::milliseconds(1000),
        std::chrono::duration_cast<std::chrono::milliseconds>((std::min)(cfg_.keep_alive_timeout, cfg_.io_timeout)) / 6);

    std::unique_lock lk(watchdog_mtx_);
    for (;;) {
        // no connections - nothing to watch, sleep until one comes (no periodic wakeups of an idle server)
        watchdog_cnd_.wait(lk, [this] { return watchdog_stop_ || watched_count_; });
        if (watchdog_cnd_.wait_for(lk, period, [this] { return watchdog_stop_; })) return;
        lk.unlock();
        close_expired();
        lk.lock();
    }
}

void http_connector::close_expired() noexcept
{
    int64_t keep_alive_timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(cfg_.keep_alive_timeout).count();
    int64_t io_timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(cfg_.io_timeout).count();
    int64_t now = steady_now_ms();

    lock_guard guard(mtx);
    for (auto& c : using_set_) {
        if (c.expired) continue;
        bool in_request = c.in_request.load();
        if (now - c.last_io_ms.load() > (in_request ? io_timeout_ms : keep_alive_timeout_ms)) {
            // wakes up the read/write waiting on the socket, the connection fiber ends with eof
            LOG_TRACE(logger()) << (in_request ? "io" : "keep alive") << " timeout, closing the connection";
            c.expired = true;
            c.soc.close();
        }
    }
}

void http_connector::connect(sonia::io::tcp_socket soc)
{
    std::list<connection>::iterator conn_it;
    {
        lock_guard guard(mtx);
        if (closed_) throw closed_exception();
        conn_it = using_set_.emplace(using_set_.end(), soc);
    }
    {
        std::lock_guard lk(watchdog_mtx_);
        if (!watched_count_++) watchdog_cnd_.notify_all();
    }
    SCOPE_EXIT([this, conn_it]() {
        {
            std::lock_guard lk(watchdog_mtx_);
            --watched_count_;
        }
        lock_guard guard(mtx);
        using_set_.erase(conn_it);
    });

    try {
        if (keep_alive_count_.fetch_add(1) < cfg_.keep_alive_max_count) {
            SCOPE_EXIT([this]{ --keep_alive_count_; });
            keep_alive_connect(*conn_it);
        } else {
            --keep_alive_count_;
            if (one_shot_count_.fetch_add(1) < cfg_.not_keep_alive_max_count) {
                SCOPE_EXIT([this]{ --one_shot_count_; });
                one_shot_connect(*conn_it);
            } else {
                --one_shot_count_;
                socket_write_iterator wit{soc};
                *wit = std::span{cfg_.dos_message};
                wit.flush();
            }
        }
    } catch (eof_exception const&) {
    } catch (closed_exception const&) {
    } catch (std::exception const& err) {
        LOG_ERROR(logger()) << err.what();
    }
}

bool http_connector::do_connection(connection& conn, read_iterator & ii, write_iterator & oi, bool keep_alive)
{
    bool handled = false;
    http::request req;
    req.keep_alive = keep_alive;
    auto it = decode<serialization::default_t>(range_dereferencing_iterator{reference_wrapper_iterator{ii}}, req);
    it.flush();

    // the request is in progress from here on (a keep-alive connection waiting for the next request above is not);
    // the time is stored before the decrement, so a zero counter is never seen with a stale time;
    // the keep alive timeout of the connection counts from the end of the request
    conn.in_request = true;
    ++active_requests_;
    SCOPE_EXIT([this, &conn] {
        int64_t now = steady_now_ms();
        conn.last_io_ms = now;
        conn.in_request = false;
        last_activity_ms_ = now;
        --active_requests_;
    });

    req.build_input_iterator(ii);
    
    cstring_view uri = req.get_relative_uri();
    for (auto const& r : cfg_.routes) {
        if (r.enabled && regex_match(uri.c_str(), r.pathre)) {
            if (lock_guard guard(routes_mutex_); !r.application) {
                locate(r.application_name, r.application);
            }
            http::response resp;
            r.application->handle(req, resp);
            resp.meet_keep_alive(req);
            encode<serialization::default_t>(resp, reference_wrapper_iterator(oi)).flush();
            handled = true;
            break;
        }
    }
    if (!handled) {
        if (cfg_.page404_application) {
            http::response resp;
            cfg_.page404_application->handle(req, resp);
            resp.meet_keep_alive(req);
            encode<serialization::default_t>(resp, reference_wrapper_iterator(oi)).flush();
        } else {
            copy_range(string_view(cfg_.page404_message), std::move(oi)).flush();
            return false;
        }
    }

    return req.keep_alive.value_or(false);
}

void http_connector::keep_alive_connect(connection& conn)
{
    std::vector<char> buff(cfg_.response_buffer_size + cfg_.request_buffer_size);
    array_view respbuff{&buff.front(), cfg_.response_buffer_size};
    array_view reqbuff{&buff.front() + cfg_.response_buffer_size, cfg_.request_buffer_size};

    activity_socket soc{conn};
    write_iterator oi{soc, respbuff};
    read_iterator ii{soc, reqbuff, (size_t)0};

    static std::atomic<int> keep_alive_connnum{0};
    int curconnection = keep_alive_connnum.fetch_add(1);

    while (do_connection(conn, ii, oi, true)) {
        LOG_TRACE(logger()) << "next keep alive connection: " << curconnection << ", total: " << keep_alive_count_.load();
    }
}

void http_connector::one_shot_connect(connection& conn)
{
    std::vector<char> buff(cfg_.response_buffer_size + cfg_.request_buffer_size);
    array_view respbuff{&buff.front(), cfg_.response_buffer_size};
    array_view reqbuff{&buff.front() + cfg_.response_buffer_size, cfg_.request_buffer_size};

    activity_socket soc{conn};
    write_iterator oi{soc, respbuff};
    read_iterator ii{soc, reqbuff, (size_t)0};

    do_connection(conn, ii, oi, false);
}

uint64_t http_connector::idle_time_ms() const noexcept
{
    if (active_requests_.load()) return 0;
    int64_t idle = steady_now_ms() - last_activity_ms_.load();
    return idle > 0 ? (uint64_t)idle : 0;
}

void http_connector::enable_route(string_view routeid, bool enable_val)
{
    for (auto const& r : cfg_.routes) {
        if (r.id == routeid) {
            r.enabled = enable_val;
            return;
        }
    }

    throw exception("route with id: '%1%' is not found"_fmt % routeid);
}

}
