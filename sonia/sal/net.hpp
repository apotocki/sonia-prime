//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <boost/coroutine2/coroutine.hpp>

namespace sonia::sal {

#ifdef BOOST_WINDOWS
using socket_handle = uintptr_t;
static constexpr socket_handle not_initialized_socket_v = (uintptr_t)(~0);
#else
using socket_handle = int;
static constexpr socket_handle not_initialized_socket_v = -1;
#endif

enum class net_family_type
{
    UNSPEC = 0,
    INET,
    INET6
};

struct sockaddr{};

socket_handle create_socket(net_family_type af, int type, int protocol);
void close_socket(socket_handle) noexcept;
void connect_socket(socket_handle, sockaddr *, size_t addrlen);

void setsockopt(socket_handle, int level, int optname, int val);
void bind_socket(socket_handle, sockaddr *, size_t addrlen);
void listen_socket(socket_handle, int);

class socket_address
{
public:
    socket_address();
    socket_address(const void*, size_t);

    void reset();
    
    std::string str() const;
    uint16_t port() const;

    std::pair<const char*, size_t> in_addr() const;
    template <typename T> T * addr() { return std::launder(reinterpret_cast<T*>(buffer_)); }
    uint8_t size() const { return sz_; }

private:
    alignas(void*) char buffer_[31]; // to be able to store sockaddr_in6
    uint8_t sz_;
};

struct addrinfo
{
    addrinfo() = delete;
    addrinfo(addrinfo const&) = delete;
    addrinfo& operator= (addrinfo const&) = delete;
    ~addrinfo() = delete;

    net_family_type family() const;
    int ai_flags() const;
    int ai_socktype() const;
    int ai_protocol() const;
    size_t ai_addrlen() const;
    sockaddr * ai_addr() const;
};

boost::coroutines2::coroutine<addrinfo const*&>::pull_type parse_net_address(net_family_type, int hint_type, int hint_protocol, cstring_view address, uint16_t port);

}
