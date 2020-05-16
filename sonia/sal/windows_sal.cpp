//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/utility/windows.hpp"
#include "sonia/sal.hpp"
#include "sonia/sal/net.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/exceptions.hpp"

#include <process.h>

namespace sonia::sal {

namespace winapi = sonia::windows;

int get_pid()
{
    return _getpid();
}

void set_thread_name(sonia::thread::id tid, string_view name)
{
    winapi::set_thread_name(tid, (&name.back() + 1) ? to_string(name).c_str() : name.begin());
}

void set_thread_name(sonia::thread::id tid, std::string const& name)
{
    winapi::set_thread_name(tid, name.c_str());
}

typedef sonia::services::bundle*(get_bundle_fn)();

shared_ptr<sonia::services::bundle> load_bundle(sonia::services::bundle_configuration const& cfg)
{
#if !defined(__MINGW32__) && !defined(__MINGW64__)
    std::string libname = to_string(cfg.lib) + ".dll";
#else
    std::string libname = "lib" + to_string(cfg.lib) + ".dll";
#endif
    HMODULE lib_handle = LoadLibraryA(libname.c_str());
    if (!lib_handle) {
        DWORD err = GetLastError();
        THROW_INTERNAL_ERROR("Cannot load bundle: %1%\n%2%"_fmt % libname % winapi::error_message(err));
    }
    SCOPE_EXIT([&lib_handle]() { FreeLibrary(lib_handle); });

    get_bundle_fn* fn_handle = (get_bundle_fn*)GetProcAddress(lib_handle, "get_bundle");
    if (!fn_handle) {
        DWORD err = GetLastError();
        THROW_INTERNAL_ERROR("Cannot load symbol 'get_bundle' in %1% module\n%2%"_fmt % libname % winapi::error_message(err));
    }
    shared_ptr<sonia::services::bundle> result = shared_ptr<sonia::services::bundle>(fn_handle());
    service_access::set_layer(*result, cfg.layer);
    result->set_handle(lib_handle);
    lib_handle = NULL; // unpin
    return std::move(result);
}

uint64_t file_size(HANDLE h)
{
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(h, &fileSize)) {
        DWORD err = GetLastError();
        throw exception("get file size error : %1%"_fmt % winapi::error_message(err));
    }
    return (((uint64_t)fileSize.HighPart) << 32) | fileSize.LowPart;
}

void delete_file(cstring_view path)
{
    std::wstring wfname = winapi::utf8_to_utf16(path);
    winapi::delete_file(wfname.c_str(), path.c_str());
}

// net
inline int af_value(net_family_type st)
{
    switch(st) {
        case net_family_type::INET: return AF_INET;
        case net_family_type::INET6: return AF_INET6;
        default: return AF_UNSPEC;
    }
}

inline net_family_type ft_value(int af)
{
    switch(af) {
        case AF_INET: return net_family_type::INET;
        case AF_INET6: return net_family_type::INET6;
        default: return net_family_type::UNSPEC;
    }
}

socket_handle create_socket(net_family_type ft, int type, int protocol)
{
    return winapi::create_socket(af_value(ft), type, protocol);
}

void close_socket(socket_handle sval) noexcept
{
    SOCKET s = (SOCKET)sval;
    if (s != INVALID_SOCKET) {
        //::shutdown(s, SD_BOTH);
        if (closesocket(s) == SOCKET_ERROR) {
            DWORD err = WSAGetLastError();
            GLOBAL_LOG_ERROR() << "errror while closing socket: " << winapi::error_message(err);
        }
    }
}

void connect_socket(socket_handle soc, sockaddr * addr, size_t addrlen)
{
    winapi::connect_socket(soc, reinterpret_cast<::sockaddr*>(addr), (int)addrlen);
}

void setsockopt(socket_handle s, int level, int optname, int val)
{
    winapi::setsockopt((SOCKET)s, level, optname, (const char*)&optname, sizeof(int));
}

void bind_socket(socket_handle s, sockaddr * addr, size_t addrlen)
{
    winapi::bind_socket((SOCKET)s, reinterpret_cast<::sockaddr*>(addr), (int)addrlen);
}

void listen_socket(socket_handle s, int bl)
{
    return winapi::listen_socket((SOCKET)s, bl);
}

std::string socket_address::str() const
{
    return winapi::inet_ntoa(reinterpret_cast<::sockaddr const*>(buffer_), (DWORD)sz_, NULL);
}

uint16_t socket_address::port() const
{
    return reinterpret_cast<sockaddr_in const*>(buffer_)->sin_port;
}

int addrinfo::ai_flags() const { return reinterpret_cast<ADDRINFOW const*>(this)->ai_flags; }
net_family_type addrinfo::family() const { return ft_value(reinterpret_cast<ADDRINFOW const*>(this)->ai_family); }
int addrinfo::ai_socktype() const { return reinterpret_cast<ADDRINFOW const*>(this)->ai_socktype; }
int addrinfo::ai_protocol() const { return reinterpret_cast<ADDRINFOW const*>(this)->ai_protocol; }
size_t addrinfo::ai_addrlen() const { return reinterpret_cast<ADDRINFOW const*>(this)->ai_addrlen; }
sockaddr * addrinfo::ai_addr() const { return reinterpret_cast<sockaddr *>(reinterpret_cast<ADDRINFOW const*>(this)->ai_addr); }



boost::coroutines2::coroutine<addrinfo const*&>::pull_type parse_net_address(net_family_type hint_af, int hint_type, int hint_protocol, cstring_view address, uint16_t port)
{
    return boost::coroutines2::coroutine<addrinfo const*&>::pull_type
        ([=](boost::coroutines2::coroutine<addrinfo const*&>::push_type & yield)
    {
        std::wstring wadr = winapi::utf8_to_utf16(address);
        std::wstring portstr = std::to_wstring(port);

        ADDRINFOW *result = nullptr, hints;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = af_value(hint_af);
        hints.ai_socktype = hint_type;
        hints.ai_protocol = hint_protocol;

        DWORD iResult = GetAddrInfoW(wadr.c_str(), portstr.c_str(), &hints, &result);
        if (iResult) {
            DWORD err = WSAGetLastError();
            throw exception("can't understand address error: %1%"_fmt % winapi::error_message(err));
        }

        SCOPE_EXIT([result]() { FreeAddrInfoW(result); });
        for (ADDRINFOW * ptr = result; ptr; ptr = ptr->ai_next)
        {
            addrinfo const* val = reinterpret_cast<addrinfo*>(ptr);
            yield(val);
            if (!val) break;
        }
    });
}

}
