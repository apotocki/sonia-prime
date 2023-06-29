//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/span.hpp"
#include "sonia/array_view.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/string.hpp"
#include "sonia/cstdint.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/sal/net.hpp"

#include "smart_handle_facade.hpp"

namespace sonia::io {

template <class DerivedT, class TraitsT> class tcp_socket_adapter;
template <class DerivedT, class TraitsT> class tcp_server_socket_adapter;

enum class shutdown_opt
{
    read = 1, write = 2, both = 3
};

template <typename TraitsT>
class tcp_server_socket_service
{
protected:
    using tcp_handle_type = typename TraitsT::handle_type;

public:
    virtual ~tcp_server_socket_service() = default;

    virtual fibers::future<smart_handle_facade<TraitsT, tcp_socket_adapter>> tcp_server_socket_accept(tcp_handle_type) = 0;
    virtual size_t tcp_server_socket_accepting_count(tcp_handle_type) const = 0;

    virtual void close_handle(identity<tcp_server_socket_service>, tcp_handle_type) noexcept = 0;
    virtual void release_handle(identity<tcp_server_socket_service>, tcp_handle_type) noexcept = 0;
    virtual void free_handle(identity<tcp_server_socket_service>, tcp_handle_type) noexcept = 0;
};

template <typename TraitsT>
class tcp_socket_service
{
protected:
    using tcp_handle_type = typename TraitsT::handle_type;

public:
    virtual ~tcp_socket_service() = default;

    virtual expected<size_t, std::exception_ptr> tcp_socket_read_some(tcp_handle_type, void * buff, size_t sz) noexcept = 0;
    virtual expected<size_t, std::exception_ptr> tcp_socket_write_some(tcp_handle_type, void const* buff, size_t sz) noexcept = 0;
    virtual void shutdown_handle(identity<tcp_socket_service>, tcp_handle_type, shutdown_opt) noexcept = 0;
    virtual void close_handle(identity<tcp_socket_service>, tcp_handle_type) noexcept = 0;
    virtual void release_handle(identity<tcp_socket_service>, tcp_handle_type) noexcept = 0;
    virtual void free_handle(identity<tcp_socket_service>, tcp_handle_type) noexcept = 0;

    virtual sal::socket_handle system_handle(tcp_handle_type) noexcept = 0;
};

template <class DerivedT, class TraitsT>
class tcp_server_socket_adapter
{
public:
    using handle_type = typename TraitsT::handle_type;
    using service_type = tcp_server_socket_service<TraitsT>;
    using socket_handle = smart_handle_facade<TraitsT, sonia::io::tcp_socket_adapter>;

    fibers::future<socket_handle> accept()
    {
        return impl().tcp_server_socket_accept(handle());
    }

    size_t accepting_count() const
    {
        return impl().tcp_server_socket_accepting_count(handle());
    }

    void close() noexcept
    {
        impl().close_handle(identity<service_type>(), handle());
    }

private:
    service_type & impl() const { return *static_cast<DerivedT const*>(this)->impl_; }
    handle_type handle() const { return static_cast<DerivedT const*>(this)->handle_; }
};

template <class DerivedT, class TraitsT>
class tcp_socket_adapter
{
public:
    using handle_type = typename TraitsT::handle_type;
    using service_type = tcp_socket_service<TraitsT>;

    template <typename T>
    expected<size_t, std::exception_ptr> read_some(array_view<T> buff) noexcept
    {
        return impl().tcp_socket_read_some(handle(), buff.begin(), buff.size() * sizeof(T));
    }

    template <typename T, size_t EV>
    expected<size_t, std::exception_ptr> read_some(std::span<T, EV> buff) noexcept
    {
        return impl().tcp_socket_read_some(handle(), buff.data(), buff.size() * sizeof(T));
    }

    template <typename T>
    expected<size_t, std::exception_ptr> read_some(T * buff, size_t sz) noexcept
    {
        constexpr size_t elem_sz = is_void_v<T> ? 1 : size_of_v<T>;
        return impl().tcp_socket_read_some(handle(), buff, sz * elem_sz);
    }

    template <typename T>
    expected<size_t, std::exception_ptr> write_some(array_view<T> buff) noexcept
    {
        return impl().tcp_socket_write_some(handle(), buff.begin(), buff.size() * sizeof(T));
    }

    template <typename T, size_t EV>
    expected<size_t, std::exception_ptr> write_some(std::span<T, EV> buff) noexcept
    {
        return impl().tcp_socket_write_some(handle(), buff.data(), buff.size() * sizeof(T));
    }

    template <typename T>
    expected<size_t, std::exception_ptr> write_some(const T * buff, size_t sz) noexcept
    {
        constexpr size_t elem_sz = is_void_v<T> ? 1 : size_of_v<T>;
        return impl().tcp_socket_write_some(handle(), buff, sz * elem_sz);
    }

    void shutdown(shutdown_opt opt = shutdown_opt::both)
    {
        impl().shutdown_handle(identity<service_type>(), handle(), opt);
    }

    void close() noexcept
    {
        impl().close_handle(identity<service_type>(), handle());
    }

    sal::socket_handle system_handle() noexcept { return impl().system_handle(handle()); }

private:
    service_type & impl() const { return *static_cast<DerivedT const*>(this)->impl_; }
    handle_type handle() const { return static_cast<DerivedT const*>(this)->handle_; }
};

class tcp_socket_access : smart_handle_access<tcp_socket_adapter>
{
public:
    template <typename TraitsT>
    static smart_handle_facade<TraitsT, tcp_socket_adapter> create_tcp_socket(
        shared_ptr<tcp_socket_service<typename TraitsT::strong_traits_type>> impl,
        typename TraitsT::handle_type handle) noexcept
    {
        return smart_handle_access<tcp_socket_adapter>::create<TraitsT>(std::move(impl), handle);
    }

    template <typename TraitsT>
    static smart_handle_facade<TraitsT, tcp_server_socket_adapter> create_tcp_server_socket(
        shared_ptr<tcp_server_socket_service<typename TraitsT::strong_traits_type>> impl,
        typename TraitsT::handle_type handle) noexcept
    {
        return smart_handle_access<tcp_server_socket_adapter>::create<TraitsT>(std::move(impl), handle);
    }

    template <typename TraitsT>
    static typename TraitsT::handle_type handle(smart_handle_facade<TraitsT, tcp_socket_adapter> const& h)
    {
        return smart_handle_access<tcp_socket_adapter>::handle(h);
    }

    template <typename TraitsT>
    static shared_ptr<tcp_socket_service<TraitsT>> const& impl(smart_handle_facade<TraitsT, tcp_socket_adapter> const& h)
    {
        return smart_handle_access<tcp_socket_adapter>::impl(h);
    }
};

template <class TraitsT>
class tcp_server_socket_factory
{
public:
    virtual ~tcp_server_socket_factory() = default;

    virtual smart_handle_facade<TraitsT, tcp_server_socket_adapter> create_server_socket(cstring_view address, uint16_t port = 0, sonia::sal::net_family_type ft = sonia::sal::net_family_type::INET) = 0;

    smart_handle_facade<TraitsT, tcp_server_socket_adapter> create_server_socket(std::string_view address, uint16_t port = 0, sonia::sal::net_family_type ft = sonia::sal::net_family_type::INET)
    {
        return as_cstring<64>(address, [this, port, ft](cstring_view address) { return create_server_socket(address, port, ft); });
    }
};

template <class TraitsT>
class tcp_socket_factory
{
public:
    virtual ~tcp_socket_factory() = default;
    
    virtual smart_handle_facade<TraitsT, tcp_socket_adapter> create_connected_tcp_socket(cstring_view address, uint16_t port = 0, sonia::sal::net_family_type ft = sonia::sal::net_family_type::INET) = 0;
    
    smart_handle_facade<TraitsT, tcp_socket_adapter> create_connected_tcp_socket(std::string_view address, uint16_t port = 0, sonia::sal::net_family_type ft = sonia::sal::net_family_type::INET)
    {
        return as_cstring<64>(address, [this, port, ft](cstring_view address) { return create_connected_tcp_socket(address, port, ft); });
    }

    template <typename StrT>
    smart_handle_facade<TraitsT, tcp_socket_adapter> create_connected_tcp_socket(StrT && address, uint16_t port = 0, sonia::sal::net_family_type ft = sonia::sal::net_family_type::INET)
    {
        return create_connected_tcp_socket(to_string_view(std::forward<StrT>(address)), port, ft);
    }
};

}
