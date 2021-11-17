//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sal.hpp"
#include "sonia/sal/net.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/services.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include <dlfcn.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>

#include "sonia/sys/posix/posix.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia::sal {

void set_thread_name(sonia::thread::id tid, string_view name)
{
    // do nothing
}

void set_thread_name(sonia::thread::id tid, std::string const& name)
{
    // do nothing
}

using get_bundle_fn = sonia::services::bundle* ();

shared_ptr<sonia::services::bundle> load_bundle(sonia::services::bundle_configuration const& cfg)
{
    std::string libname = "lib" + to_string(cfg.lib) + 
#if defined (__APPLE__)
        ".dylib";
#else
        ".so";
#endif
    void* handle = dlopen(
#ifndef __ANDROID__
        libname.c_str(),
#else 
        (bundles_path + libname).c_str(), // Android requires fullpath
#endif
        RTLD_LAZY
    );

    if (!handle) {
        throw internal_error("Cannot load bundle: %1%\n%2%"_fmt % libname % dlerror());
    }

    // reset errors
    dlerror();

    get_bundle_fn* fn_handle = (get_bundle_fn*)dlsym(handle, "get_bundle");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        throw internal_error("Cannot load symbol 'get_bundle' in %1% module\n%2%"_fmt % libname % dlsym_error);
    }
    shared_ptr<sonia::services::bundle> result = shared_ptr<sonia::services::bundle>(fn_handle());
    service_access::set_layer(*result, cfg.layer);
    result->set_handle(handle);
    return result;
}

uint64_t file_size(int h)
{
    struct stat filest;
    if (-1 == fstat(h, &filest)) {
        int err = errno;
        throw exception("get file size error : ", strerror(err));
    }
    return filest.st_size;
}

void delete_file(cstring_view path)
{
    if (-1 == unlink(path.c_str())) {
        int err = errno;
        throw exception("can't delete file %1%, error : %2%"_fmt % path % strerror(err));
    }
}

// net
inline int af_value(net_family_type ft)
{
    switch (ft) {
    case net_family_type::INET: return AF_INET;
    case net_family_type::INET6: return AF_INET6;
    default: return AF_UNSPEC;
    }
}

inline net_family_type ft_value(int af)
{
    switch (af) {
        case AF_INET: return net_family_type::INET;
        case AF_INET6: return net_family_type::INET6;
        default: return net_family_type::UNSPEC;
    }
}

socket_handle create_socket(net_family_type ft, int type, int protocol)
{
    return posix::create_socket(af_value(ft), type, protocol);
}

void close_socket(socket_handle sval) noexcept
{
    if (-1 == ::close(sval)) {
        int err = errno;
        GLOBAL_LOG_ERROR() << "errror while closing descriptor: " << strerror(err);
    }
}

void setsockopt(socket_handle s, int level, int optname, int val)
{
    posix::setsockopt(s, level, optname, (const char*)&optname, sizeof(int));
}

void connect_socket(socket_handle s, sockaddr* addr, size_t addrlen)
{
    posix::connect(s, reinterpret_cast<::sockaddr*>(addr), (socklen_t)addrlen);
}

void bind_socket(socket_handle s, sockaddr* addr, size_t addrlen)
{
    posix::bind_socket(s, reinterpret_cast<::sockaddr*>(addr), (socklen_t)addrlen);
}

void listen_socket(socket_handle s, int bl)
{
    posix::listen(s, bl);
}

std::string socket_address::str() const
{
    return posix::inet_ntoa(reinterpret_cast<sockaddr_in const*>(buffer_));
}

uint16_t socket_address::port() const
{
    return reinterpret_cast<sockaddr_in const*>(buffer_)->sin_port;
}

int addrinfo::ai_flags() const { return reinterpret_cast<::addrinfo const*>(this)->ai_flags; }
net_family_type addrinfo::family() const { return ft_value(reinterpret_cast<::addrinfo const*>(this)->ai_family); }
int addrinfo::ai_socktype() const { return reinterpret_cast<::addrinfo const*>(this)->ai_socktype; }
int addrinfo::ai_protocol() const { return reinterpret_cast<::addrinfo const*>(this)->ai_protocol; }
size_t addrinfo::ai_addrlen() const { return reinterpret_cast<::addrinfo const*>(this)->ai_addrlen; }
sockaddr* addrinfo::ai_addr() const { return reinterpret_cast<sockaddr*>(reinterpret_cast<::addrinfo const*>(this)->ai_addr); }

boost::coroutines2::coroutine<addrinfo const*&>::pull_type parse_net_address(net_family_type hint_ft, int hint_type, int hint_protocol, cstring_view address, uint16_t port)
{
    return boost::coroutines2::coroutine<addrinfo const*&>::pull_type
    ([=](boost::coroutines2::coroutine<addrinfo const*&>::push_type& yield)
        {
            ::addrinfo hints;
            bzero((char*)&hints, sizeof(::addrinfo));
            hints.ai_family = af_value(hint_ft);
            hints.ai_socktype = hint_type;
            hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
            hints.ai_protocol = hint_protocol;
            hints.ai_canonname = NULL;
            hints.ai_addr = NULL;
            hints.ai_next = NULL;

            ::addrinfo* rp;
            if (int s = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &rp); s != 0 || !rp) {
                throw exception("tcp socket address '%1%' is not valid, error: %2%"_fmt % address % gai_strerror(s));
            }
            SCOPE_EXIT([rp] { freeaddrinfo(rp); });
            for (::addrinfo* pai = rp; !!pai; pai = pai->ai_next)
            {
                addrinfo const* val = reinterpret_cast<addrinfo*>(pai);
                yield(val);
                if (!val) break;
            }
        });
}

}
