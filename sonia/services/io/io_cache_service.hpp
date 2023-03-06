//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <tuple>
#include <boost/intrusive/unordered_set.hpp>
#include <boost/intrusive/list.hpp>

#include "sonia/services/service.hpp"
#include "sonia/services/io/io_cache_service_configuration.hpp"

#include "sonia/utility/object_pool.hpp"
#include "sonia/utility/caches/utility.hpp"

#include "tcp_socket.hpp"
#include "udp_socket.hpp"
#include "file.hpp"
#include "sockets.hpp"

#include "sonia/utility/functional/hash.hpp"

namespace sonia::services {

class io_cache_service 
    : public service
    , public io::tcp_socket_factory_type
{
public:
    explicit io_cache_service(io_cache_service_configuration const& cfg);

    void open() override;
    void close() noexcept override;

    // tcp_socket_factory
    io::tcp_socket create_connected_tcp_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt) override;

private:
    
    struct cache_route_descriptor;
    
    struct cache_item : shared_handle<io::socket_handle_traits>
    {
        shared_ptr<cache_route_descriptor> descriptor;

        boost::intrusive::unordered_set_member_hook<> key_hook;
        boost::intrusive::list_member_hook<> connection_hook;
        boost::intrusive::list_member_hook<> lru_hook;

        io::tcp_socket sock;

        explicit cache_item(shared_ptr<cache_route_descriptor> d, io::tcp_socket s) 
            : descriptor(std::move(d)), sock(std::move(s))
        {
            
        }

        cache_item(cache_item const&) = delete;
        cache_item& operator=(cache_item const&) = delete;
    };

    using route_list_type = boost::intrusive::list<
        cache_item,
        boost::intrusive::member_hook<cache_item, boost::intrusive::list_member_hook<>, &cache_item::connection_hook>>;

    using lru_list_type = boost::intrusive::list<
        cache_item,
        boost::intrusive::member_hook<cache_item, boost::intrusive::list_member_hook<>, &cache_item::lru_hook>>;

    struct cache_route_descriptor
    {
        std::tuple<std::string, uint16_t, sal::net_family_type> key;
        fibers::mutex mtx;
        fibers::condition_variable cv;

        route_list_type group;
        uint32_t count{1};

        template <typename ArgT>
        explicit cache_route_descriptor(ArgT && arg) 
            : key(to_string(std::get<0>(std::forward<ArgT>(arg))), std::get<1>(arg), std::get<2>(arg))
        {
            
        }

        void wait(unique_lock<fibers::mutex> & lck, uint32_t limit);
    };

    struct cache_item_hasher
    {
        template <class StringT>
        size_t operator()(std::tuple<StringT, uint16_t, sal::net_family_type> const& itm) const
        {
            size_t seed = hash_init_value();
            hash_combine(seed, std::get<0>(itm));
            hash_combine(seed, std::get<1>(itm));
            hash_combine(seed, (int)std::get<2>(itm));
            return seed;
        }

        size_t operator()(cache_item const& itm) const
        {
            return operator()(itm.descriptor->key);
        }
    };

    struct cache_item_comparer
    {
        bool operator()(cache_item const& l, cache_item const& r) const
        {
            return l.descriptor->key == r.descriptor->key;
        }

        template <class StringT>
        bool operator()(std::tuple<StringT, uint16_t, sal::net_family_type> const& l, cache_item const& r) const
        {
            return l == r.descriptor->key;
        }
    };

    using cashe_type = boost::intrusive::unordered_multiset<
        cache_item,
        boost::intrusive::member_hook<cache_item, boost::intrusive::unordered_set_member_hook<>, &cache_item::key_hook>,
        boost::intrusive::hash<cache_item_hasher>,
        boost::intrusive::equal<cache_item_comparer>
    >;

    struct connection_cache 
        : io::tcp_socket_service_type
        , enable_shared_from_this<connection_cache>
    {
        io_cache_service_configuration cfg_;
        shared_ptr<io::tcp_socket_factory_type> tcp_factory_;

        //spin_mutex mtx;
        fibers::mutex mtx;
        cache::buckets<cashe_type::bucket_type> buckets_;
        cashe_type cache_;
        object_pool<cache_item, spin_mutex> item_pool_;

        explicit connection_cache(io_cache_service_configuration const&);
        ~connection_cache();

        io::tcp_socket create_connected_tcp_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt);

        expected<size_t, std::exception_ptr> tcp_socket_read_some(tcp_handle_type, void * buff, size_t sz) noexcept override final;
        expected<size_t, std::exception_ptr> tcp_socket_write_some(tcp_handle_type, void const* buff, size_t sz) noexcept override final;
        void shutdown_handle(identity<io::tcp_socket_service_type>, tcp_handle_type, io::shutdown_opt) noexcept override final;
        void close_handle(identity<io::tcp_socket_service_type>, tcp_handle_type) noexcept override final;
        void release_handle(identity<io::tcp_socket_service_type>, tcp_handle_type h) noexcept override final;
        void free_handle(identity<io::tcp_socket_service_type>, tcp_handle_type) noexcept override final;
        sal::socket_handle system_handle(tcp_handle_type) noexcept override final;

        spin_mutex lru_mtx_;
        //fibers::mutex lru_mtx_;
        lru_list_type lru_;
        void clear_lru(); // should be called unde locked mtx

    };

    shared_ptr<connection_cache> cache_;
};

}
