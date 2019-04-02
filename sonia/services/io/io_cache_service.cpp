//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "io_cache_service.hpp"
#include "sonia/exceptions/internal_errors.hpp"
#include "sonia/services.hpp"

namespace sonia::services {

constexpr size_t io_service_cache_buckets_size = 65537;
constexpr size_t io_service_cache_item_pool_size = 32;

io_cache_service::io_cache_service(io_cache_service_configuration const& cfg)
    : cache_(make_shared<connection_cache>(cfg))
{
    set_log_attribute("Type", "io-cache");
}

void io_cache_service::open()
{
    
}

void io_cache_service::close() noexcept
{
    cache_.reset();
}

io::tcp_socket io_cache_service::create_connected_tcp_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt)
{
    shared_ptr<connection_cache> cache_copy = cache_;
    if (cache_copy) {
        return cache_copy->create_connected_tcp_socket(address, port, dt);
    }
    throw closed_exception();
}

io_cache_service::connection_cache::connection_cache(io_cache_service_configuration const& cfg)
    : cfg_(cfg)
    , buckets_(io_service_cache_buckets_size)
    , cache_(cashe_type::bucket_traits(buckets_.get_buckets(), buckets_.size()))
    , item_pool_(io_service_cache_item_pool_size)
{
    locate(cfg_.tcp_socket_factory, tcp_factory_);
}

io_cache_service::connection_cache::~connection_cache()
{
    lru_.clear();
    while (!cache_.empty()) {
        auto it = cache_.begin();
        cache_item * itm = &*it;
        cache_.erase(it);
        itm->descriptor->group.clear();
        itm->release_weak(static_cast<io::tcp_socket_service_type*>(this));
    }
}

void io_cache_service::cache_route_descriptor::wait(unique_lock<fibers::mutex> & lck, uint32_t limit)
{
    cv.wait(lck, [this, limit]{ return count < limit || !group.empty(); });
}

void io_cache_service::connection_cache::clear_lru()
{
    while (cache_.size() > cfg_.connection_count) {
        cache_item * victim = nullptr;
        for (;;)
        {
            {
                lock_guard lru_guard(lru_mtx_);
                if (lru_.empty()) return;
                victim = &lru_.back();
                victim->add_weakref();
            }
            
            shared_ptr<cache_route_descriptor> victim_rd = victim->descriptor;
            lock_guard route_guard(victim_rd->mtx);
            lock_guard lru_guard(lru_mtx_);
            if (lru_.empty()) {
                victim->release_weak(static_cast<io::tcp_socket_service_type*>(this));
                break;
            }

            if (victim != &lru_.back()) {
                victim = &lru_.back();
                if (victim->descriptor != victim_rd) continue;
                // victim was changed, but it belongs to the same route => we can reuse the same locks
            }

            // remove the victim from everywhere
            cache_.erase(*victim);
            victim_rd->group.erase(route_list_type::s_iterator_to(*victim));
            lru_.pop_back();
            victim->release_weak(static_cast<io::tcp_socket_service_type*>(this));
        }
    }
}

io::tcp_socket io_cache_service::connection_cache::create_connected_tcp_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt)
{
    shared_ptr<cache_route_descriptor> rd;
    std::tuple cache_key{address, port, dt};
    {
        lock_guard guard(mtx);
        auto it = cache_.find(cache_key, cache_item_hasher(), cache_item_comparer());
        if (it == cache_.cend()) {
            clear_lru();
            rd = make_shared<cache_route_descriptor>(cache_key);
            io::tcp_socket sock = tcp_factory_->create_connected_tcp_socket(address, port, dt);
            cache_item *pitm = item_pool_.new_object(rd, std::move(sock));
            pitm->add_weakref();
            cache_.insert(*pitm);
            return io::tcp_socket_access::create_tcp_socket<io::socket_traits>(shared_from_this(), pitm);
        } else {
            rd = it->descriptor;
        }
    }

    cache_item * result_pitm;
    {
        unique_lock lck(rd->mtx);
        rd->wait(lck, cfg_.per_route_connection_count);

        if (!rd->group.empty()) {
            result_pitm = &rd->group.back();
            rd->group.pop_back();
            {
                lock_guard lru_guard(lru_mtx_);
                lru_.erase(lru_list_type::s_iterator_to(*result_pitm));
            }
            return io::tcp_socket_access::create_tcp_socket<io::socket_traits>(shared_from_this(), result_pitm);
        }

        BOOST_ASSERT (rd->count < cfg_.per_route_connection_count);
        io::tcp_socket sock = tcp_factory_->create_connected_tcp_socket(address, port, dt);
        result_pitm = item_pool_.new_object(rd, std::move(sock));
        result_pitm->add_weakref();
        ++rd->count;
    }

    {
        lock_guard guard(mtx);
        clear_lru();
        cache_.insert(*result_pitm);
        return io::tcp_socket_access::create_tcp_socket<io::socket_traits>(shared_from_this(), result_pitm);
    }
}

io::tcp_socket io_cache_service::create_bound_tcp_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt)
{
    THROW_NOT_SUPPORTED_ERROR("bound sockets are not supported");
}

std::pair<io::tcp_socket, size_t> io_cache_service::connection_cache::tcp_socket_accept(tcp_handle_type, char*, size_t)
{
    THROW_NOT_SUPPORTED_ERROR();
}

size_t io_cache_service::connection_cache::tcp_socket_waiting_count(tcp_handle_type h)
{
    auto * ci = static_cast<cache_item*>(h);
    return ci->sock.waiting_count();
}

expected<size_t, std::error_code> io_cache_service::connection_cache::tcp_socket_read_some(tcp_handle_type h, void * buff, size_t sz)
{
    auto * ci = static_cast<cache_item*>(h);
    return ci->sock.read_some(buff, sz);
}

expected<size_t, std::error_code> io_cache_service::connection_cache::tcp_socket_write_some(tcp_handle_type h, void const* buff, size_t sz)
{
    auto * ci = static_cast<cache_item*>(h);
    return ci->sock.write_some(buff, sz);
}

void io_cache_service::connection_cache::close_handle(identity<io::tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    auto * ci = static_cast<cache_item*>(h);
    ci->sock.reset();
}

void io_cache_service::connection_cache::release_handle(identity<io::tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    auto * ci = static_cast<cache_item*>(h);
    shared_ptr<cache_route_descriptor> rd = ci->descriptor;
    if (ci->sock) {
        lock_guard route_guard(rd->mtx);
        rd->group.push_back(*ci);
        lock_guard lru_guard(lru_mtx_);
        lru_.push_front(*ci);
    } else {
        {
            lock_guard guard(mtx);
            cache_.erase(*ci);
        }
        lock_guard route_guard(rd->mtx);
        --rd->count;
        ci->release_weak(static_cast<io::tcp_socket_service_type*>(this));
    }
    rd->cv.notify_one();
}

void io_cache_service::connection_cache::free_handle(identity<io::tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    auto * ci = static_cast<cache_item*>(h);
    item_pool_.delete_object(ci);
}

}
