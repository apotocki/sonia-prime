//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sys/windows/windows.hpp"
#include "sonia/sal.hpp"
#include "sonia/sal/net.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/exceptions.hpp"

#include <process.h>
#include <WS2tcpip.h>
#include <winioctl.h>

namespace sonia::sal {

namespace winapi = sonia::windows;

int get_pid()
{
    return _getpid();
}

void set_thread_name(sonia::thread::id tid, string_view name)
{
    winapi::set_thread_name(tid, std::string{name}.c_str());
}

void set_thread_name(sonia::thread::id tid, std::string const& name)
{
    winapi::set_thread_name(tid, name.c_str());
}

std::u8string reencode_system_message(std::string_view message)
{
    return winapi::win_ansi_to_utf8(message);
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
    std::wstring wfname = winapi::utf8_to_utf16(std::string_view{(const char*)path.data(), path.size()});
    winapi::delete_file(wfname.c_str());
}

void get_disk_info(u8string_view path, disk_info& di)
{
    std::wstring wfname = winapi::utf8_to_utf16(std::string_view{(const char*)path.data(), path.size()});

    ULARGE_INTEGER ulFreeSpace;
    ULARGE_INTEGER ulTotalSpace;
    ULARGE_INTEGER ulTotalFreeSpace;
    if (!GetDiskFreeSpaceExW(wfname.data(), &ulFreeSpace, &ulTotalSpace, &ulTotalFreeSpace)) {
        DWORD err = GetLastError();
        throw exception("get_disk_info error : %1%"_fmt % winapi::error_message(err));
    }

    di.total_size = ulTotalSpace.QuadPart;
    di.free_size = ulFreeSpace.QuadPart;

    return;
    /*
    WCHAR volMntPt[256];
    memcpy(volMntPt, L"\\\\.\\", 4 * sizeof(WCHAR));
    if (!GetVolumePathNameW(wfname.c_str(), volMntPt + 4, sizeof(volMntPt)/sizeof(WCHAR) - 4)) {
        DWORD err = GetLastError();
        throw exception("get_disk_info error : %1%"_fmt % winapi::error_message(err));
    }
    std::span<WCHAR> volMntPtSp(volMntPt, wcslen(volMntPt));
    while (!volMntPtSp.empty() && volMntPtSp.back() == '\\') {
        volMntPtSp.back() = '\0';
        volMntPtSp = volMntPtSp.subspan(0, volMntPtSp.size() - 1);
    }
    */
    /*
    WCHAR volName[256];
    DWORD r = QueryDosDeviceW(volMntPtSp.data(), volName, sizeof(volName) / sizeof(WCHAR));
    if (!r) {
        DWORD err = GetLastError();
        throw exception("get_disk_info error : %1%"_fmt % winapi::error_message(err));
    }
    
    
    if (!GetVolumeNameForVolumeMountPointW(volMntPt, volName, sizeof(volName) / sizeof(WCHAR))) {
        DWORD err = GetLastError();
        throw exception("get_disk_info error : %1%"_fmt % winapi::error_message(err));
    }
    */

/*
    HANDLE hDevice = CreateFileW(volMntPtSp.data(),          // drive to open
        0,                // no access to the drive
        FILE_SHARE_READ | // share mode
        FILE_SHARE_WRITE,
        NULL,             // default security attributes
        OPEN_EXISTING,    // disposition
        0,                // file attributes
        NULL);            // do not copy file attributes
    
    if (hDevice == INVALID_HANDLE_VALUE) {
        DWORD err = GetLastError();
        throw exception("get_disk_info error : %1%"_fmt % winapi::error_message(err));
    }

    SCOPE_EXIT([hDevice]{
        CloseHandle(hDevice);
    });
    
    DISK_GEOMETRY dg;
    DWORD junk = 0;                     // discard results
    if (!DeviceIoControl(hDevice,        // device to be queried
        IOCTL_DISK_GET_DRIVE_GEOMETRY, // operation to perform
        NULL, 0,                       // no input buffer
        &dg, sizeof(DISK_GEOMETRY),    // output buffer
        &junk,                         // # bytes returned
        (LPOVERLAPPED)NULL))
    {
        DWORD err = GetLastError();
        throw exception("get_disk_info error : %1%"_fmt % winapi::error_message(err));
    }
    */
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

std::u8string socket_address::str() const
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
        std::wstring wadr = winapi::utf8_to_utf16(std::string_view{(const char*)address.data(), address.size()});
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
