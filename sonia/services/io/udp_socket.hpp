//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <expected>

#include "sonia/array_view.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/string.hpp"
#include "sonia/cstdint.hpp"
#include "sonia/sal/net.hpp"

#include "smart_handle_facade.hpp"

namespace sonia::io {

template <class DerivedT, class TraitsT> class udp_socket_adapter;

template <typename TraitsT>
class udp_socket_service
{
protected:
    using udp_handle_type = typename TraitsT::handle_type;

public:
    virtual ~udp_socket_service() = default;
    virtual void udp_socket_bind(udp_handle_type, cstring_view address, uint16_t port) = 0;
    virtual size_t udp_socket_waiting_count(udp_handle_type) = 0;
    virtual std::expected<size_t, std::exception_ptr> udp_socket_read_some(udp_handle_type, void * buff, size_t sz, sonia::sal::socket_address* addr) = 0;
    virtual std::expected<size_t, std::exception_ptr> udp_socket_write_some(udp_handle_type, sonia::sal::socket_address const&, void const* buff, size_t sz) = 0;
    virtual std::expected<size_t, std::exception_ptr> udp_socket_write_some(udp_handle_type, cstring_view address, uint16_t port, void const* buff, size_t sz) = 0;
    virtual void close_handle(identity<udp_socket_service>, udp_handle_type) noexcept = 0;
    virtual void release_handle(identity<udp_socket_service>, udp_handle_type) noexcept = 0;
    virtual void free_handle(identity<udp_socket_service>, udp_handle_type) noexcept = 0;
};

template <class DerivedT, class TraitsT>
class udp_socket_adapter
{
public:
    using handle_type = typename TraitsT::handle_type;
    using service_type = udp_socket_service<typename TraitsT::strong_traits_type>;

    void bind(cstring_view address, uint16_t port)
    {
        impl().udp_socket_bind(handle(), address, port);
    }

    size_t waiting_count() const
    {
        return impl().udp_socket_waiting_count(handle());
    }

    template <typename T>
    std::expected<size_t, std::exception_ptr> read_some(array_view<T> buff)
    {
        return impl().udp_socket_read_some(handle(), buff.begin(), buff.size() * sizeof(T), nullptr);
    }

    template <typename T>
    std::expected<size_t, std::exception_ptr> read_some(array_view<T> buff, sonia::sal::socket_address& addr)
    {
        return impl().udp_socket_read_some(handle(), buff.begin(), buff.size() * sizeof(T), &addr);
    }

    template <typename T>
    std::expected<size_t, std::exception_ptr> read_some(T * buff, size_t sz)
    {
        return impl().udp_socket_read_some(handle(), buff, sz * sizeof(T), nullptr);
    }

    template <typename T>
    std::expected<size_t, std::exception_ptr> read_some(T * buff, size_t sz, sonia::sal::socket_address& addr)
    {
        return impl().udp_socket_read_some(handle(), buff, sz * sizeof(T), &addr);
    }

    template <typename T>
    std::expected<size_t, std::exception_ptr> write_some(sonia::sal::socket_address const& address, array_view<T> buff)
    {
        return impl().udp_socket_write_some(handle(), address, buff.begin(), buff.size() * sizeof(T));
    }

    template <typename T>
    std::expected<size_t, std::exception_ptr> write_some(cstring_view address, uint16_t port, array_view<T> buff)
    {
        return impl().udp_socket_write_some(handle(), address, port, buff.begin(), buff.size() * sizeof(T));
    }

    template <typename T>
    std::expected<size_t, std::exception_ptr> write_some(cstring_view address, uint16_t port, const T * buff, size_t sz)
    {
        return impl().udp_socket_write_some(handle(), address, port, buff, sz * sizeof(T));
    }

    void close() noexcept
    {
        impl().close_handle(identity<service_type>(), handle());
    }

private:
    service_type & impl() const { return *static_cast<DerivedT const*>(this)->impl_; }
    handle_type handle() const { return static_cast<DerivedT const*>(this)->handle_; }
};

class udp_socket_access : smart_handle_access<udp_socket_adapter>
{
public:
    template <typename TraitsT>
    static smart_handle_facade<TraitsT, udp_socket_adapter> create_udp_socket(
        shared_ptr<udp_socket_service<typename TraitsT::strong_traits_type>> impl,
        typename TraitsT::handle_type handle) noexcept
    {
        return smart_handle_access<udp_socket_adapter>::create<TraitsT>(std::move(impl), handle);
    }
};

template <class TraitsT>
class udp_socket_factory
{
public:
    virtual ~udp_socket_factory() = default;

    virtual smart_handle_facade<TraitsT, udp_socket_adapter> create_udp_socket(sonia::sal::net_family_type dt = sonia::sal::net_family_type::INET) = 0;
};

}
