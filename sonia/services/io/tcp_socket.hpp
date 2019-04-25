//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_TCP_SOCKET_HPP
#define SONIA_IO_TCP_SOCKET_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <system_error>

#include "sonia/array_view.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/string.hpp"
#include "sonia/cstdint.hpp"
#include "sonia/sal/net.hpp"

#include "smart_handle_facade.hpp"

namespace sonia::io {

template <class DerivedT, class TraitsT> class tcp_socket_adapter;

template <typename TraitsT>
class tcp_socket_service
{
protected:
    using tcp_handle_type = typename TraitsT::handle_type;

public:
    virtual ~tcp_socket_service() = default;

    virtual std::pair<smart_handle_facade<TraitsT, tcp_socket_adapter>, size_t> tcp_socket_accept(tcp_handle_type, char*, size_t) = 0;
    virtual size_t tcp_socket_waiting_count(tcp_handle_type) = 0;

    virtual expected<size_t, std::error_code> tcp_socket_read_some(tcp_handle_type, void * buff, size_t sz) = 0;
    virtual expected<size_t, std::error_code> tcp_socket_write_some(tcp_handle_type, void const* buff, size_t sz) = 0;
    virtual void close_handle(identity<tcp_socket_service>, tcp_handle_type) noexcept = 0;
    virtual void release_handle(identity<tcp_socket_service>, tcp_handle_type) noexcept = 0;
    virtual void free_handle(identity<tcp_socket_service>, tcp_handle_type) noexcept = 0;
};

template <class DerivedT, class TraitsT>
class tcp_socket_adapter
{
public:
    using handle_type = typename TraitsT::handle_type;
    using service_type = tcp_socket_service<TraitsT>;
    using socket_handle = smart_handle_facade<TraitsT, sonia::io::tcp_socket_adapter>;

    std::pair<socket_handle, size_t> accept(char * buff, size_t sz)
    {
        return impl().tcp_socket_accept(handle(), buff, sz);
    }

    size_t waiting_count() const
    {
        return impl().tcp_socket_waiting_count(handle());
    }

    template <typename T>
    expected<size_t, std::error_code> read_some(array_view<T> buff)
    {
        return impl().tcp_socket_read_some(handle(), buff.begin(), buff.size() * sizeof(T));
    }

    template <typename T>
    expected<size_t, std::error_code> read_some(T * buff, size_t sz)
    {
        constexpr size_t elem_sz = is_void_v<T> ? 1 : size_of_v<T>;
        return impl().tcp_socket_read_some(handle(), buff, sz * elem_sz);
    }

    template <typename T>
    expected<size_t, std::error_code> write_some(array_view<const T> buff)
    {
        return impl().tcp_socket_write_some(handle(), buff.begin(), buff.size() * sizeof(T));
    }

    template <typename T>
    expected<size_t, std::error_code> write_some(const T * buff, size_t sz)
    {
        constexpr size_t elem_sz = is_void_v<T> ? 1 : size_of_v<T>;
        return impl().tcp_socket_write_some(handle(), buff, sz * elem_sz);
    }

    void close() noexcept
    {
        impl().close_handle(identity<service_type>(), handle());
    }

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
class tcp_socket_factory
{
public:
    virtual ~tcp_socket_factory() = default;
    
    virtual smart_handle_facade<TraitsT, tcp_socket_adapter> create_bound_tcp_socket(cstring_view address, uint16_t port = 0, sonia::sal::net_family_type ft = sonia::sal::net_family_type::INET) = 0;
    virtual smart_handle_facade<TraitsT, tcp_socket_adapter> create_connected_tcp_socket(cstring_view address, uint16_t port = 0, sonia::sal::net_family_type ft = sonia::sal::net_family_type::INET) = 0;

    smart_handle_facade<TraitsT, tcp_socket_adapter> create_bound_tcp_socket(string_view address, uint16_t port = 0, sonia::sal::net_family_type ft = sonia::sal::net_family_type::INET)
    {
        return as_cstring<64>(address, [this, port, ft](cstring_view address) { return create_bound_tcp_socket(address, port, ft); });
    }
    
    smart_handle_facade<TraitsT, tcp_socket_adapter> create_connected_tcp_socket(string_view address, uint16_t port = 0, sonia::sal::net_family_type ft = sonia::sal::net_family_type::INET)
    {
        return as_cstring<64>(address, [this, port, ft](cstring_view address) { return create_connected_tcp_socket(address, port, ft); });
    }

    template <typename StringT>
    smart_handle_facade<TraitsT, tcp_socket_adapter> create_bound_tcp_socket(StringT && address, uint16_t port = 0, sonia::sal::net_family_type ft = sonia::sal::net_family_type::INET)
    {
        return create_bound_tcp_socket(to_string_view(address), port, ft);
    }

    template <typename StringT>
    smart_handle_facade<TraitsT, tcp_socket_adapter> create_connected_tcp_socket(StringT && address, uint16_t port = 0, sonia::sal::net_family_type ft = sonia::sal::net_family_type::INET)
    {
        return create_connected_tcp_socket(to_string_view(address), port, ft);
    }
};

}

#endif // SONIA_IO_TCP_SOCKET_HPP
