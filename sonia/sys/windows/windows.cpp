//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sys/windows/windows.hpp"

//#include <winsock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
//#include <mswsock.h>
#include <winioctl.h>

#include "sonia/utility/scope_exit.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/optimized/array.hpp"

#include <boost/lexical_cast.hpp>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

const DWORD MS_VC_EXCEPTION=0x406D1388;

namespace sonia::windows {

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void set_thread_name(DWORD dwThreadId, char const* threadName)
{
    // is needed and works only on Windows for VS Debugger
#if !defined(__MINGW32__) && !defined(__MINGW64__)
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = threadName;
    info.dwThreadID = dwThreadId;
    info.dwFlags = 0;

    __try
    {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
#endif
}

void set_thread_name(boost::thread::id tid, char const* threadName)
{
    unsigned int dwThreadId;
    std::stringstream ss;
    ss << std::hex << tid;
    ss >> dwThreadId;
    set_thread_name(dwThreadId, threadName);
}

std::wstring utf8_to_utf16(std::string_view str)
{
    std::wstring result;
    result.resize(str.size() + 1);
    int len = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), result.data(), (int)result.size());
    if (len > result.size()) {
        result.resize((size_t)len);
        len = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), result.data(), (int)result.size());
        BOOST_ASSERT(len <= result.size());
    }
    return std::move(result);
}

std::u8string utf16_to_utf8(std::wstring_view str)
{
    std::u8string result;
    result.resize(str.size());
    int len = WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0, NULL, NULL);
    result.resize((size_t)len);
    WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), (char*)result.data(), (int)result.size(), NULL, NULL);
    return std::move(result);
}

std::u8string win_ansi_to_utf8(std::string_view str)
{
    std::wstring wresult;
    wresult.resize(str.size() + 1);
    int len = MultiByteToWideChar(CP_ACP, 0, str.data(), (int)str.size(), wresult.data(), (int)wresult.size());
    if (len > wresult.size()) {
        wresult.resize((size_t)len);
        int len = MultiByteToWideChar(CP_ACP, 0, str.data(), (int)str.size(), wresult.data(), (int)wresult.size());
        BOOST_ASSERT(len <= wresult.size());
    }
    return utf16_to_utf8(wresult);
}

std::string error_message(DWORD errcode)
{
    LPWSTR pBuffer = nullptr;
    DWORD num = FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errcode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&pBuffer,
        0,
        NULL);

    if (pBuffer) {
        SCOPE_EXIT([&pBuffer]() { LocalFree(pBuffer); });

        int slength = (int)num + 1;
        int len = WideCharToMultiByte(CP_ACP, 0, pBuffer, slength, 0, 0, 0, 0);
        std::vector<char> buf(len);
        WideCharToMultiByte(CP_ACP, 0, pBuffer, slength, &buf[0], len, 0, 0);
        while (!buf.empty() && (buf.back() == '\0' || buf.back() == '\r' || buf.back() == '\n')) buf.pop_back();
        return std::string(buf.begin(), buf.end());
    }

    return to_string("unknown error, errcode: %1%"_fmt % errcode);
}

wsa_scope::wsa_scope()
{
    WSADATA wsa_data;
    long result = WSAStartup(WINSOCK_VERSION, &wsa_data);
    if (result) {
        DWORD err = GetLastError();
        throw exception("can't initialize windows socket subsystem,  error: %1%"_fmt % error_message(err));
    }
}

wsa_scope::~wsa_scope()
{
    WSACleanup();
}

void get_host_addresses(function<bool(std::string_view)> consumer)
{
    ULONG flags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER;
    ULONG family = AF_INET;

    ULONG buflen = 0;
    DWORD rc = GetAdaptersAddresses(family, flags, nullptr, nullptr, &buflen);
    if (rc != ERROR_BUFFER_OVERFLOW || buflen == 0) {
        DWORD err = WSAGetLastError();
        throw exception("can't get addresses error: %1%"_fmt % error_message(err));
    }

    std::vector<unsigned char> buf(buflen);
    auto* addrs = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buf.data());
    rc = GetAdaptersAddresses(family, flags, nullptr, addrs, &buflen);
    if (rc != NO_ERROR) {
        DWORD err = WSAGetLastError();
        throw exception("can't get addresses error: %1%"_fmt % error_message(err));
    }

    for (auto* aa = addrs; aa; aa = aa->Next) {
        if (aa->OperStatus != IfOperStatusUp) continue;
        if (aa->IfType == IF_TYPE_SOFTWARE_LOOPBACK) continue;

        for (auto* ua = aa->FirstUnicastAddress; ua; ua = ua->Next) {
            if (!ua->Address.lpSockaddr) continue;
            if (ua->Address.lpSockaddr->sa_family != AF_INET) continue;
        
            auto* sin = reinterpret_cast<sockaddr_in*>(ua->Address.lpSockaddr);
            if (!sin) continue;

            char strbuf[INET_ADDRSTRLEN]{};
            if (!InetNtopA(AF_INET, &sin->sin_addr, strbuf, (DWORD)sizeof(strbuf))) continue;

            std::string_view sv{ strbuf };
            if (sv == "127.0.0.1"sv) continue;
            if (!consumer(sv)) return;
        }
    }
}

bool parse_address(int hint_af, int hint_type, int hint_protocol, std::string_view address, uint16_t port, function<bool(ADDRINFOW*)> rproc)
{
    std::wstring wadr = utf8_to_utf16(address);
    std::wstring portstr = boost::lexical_cast<std::wstring>(port);

    ADDRINFOW *result = nullptr, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = hint_af;
    hints.ai_socktype = hint_type;
    hints.ai_protocol = hint_protocol;

    DWORD iResult = GetAddrInfoW(wadr.c_str(), portstr.c_str(), &hints, &result);
    if (iResult) {
        DWORD err = WSAGetLastError();
        throw exception("can't understand address error: %1%"_fmt % error_message(err));
    }

    SCOPE_EXIT([result]() { FreeAddrInfoW(result); });

    for (ADDRINFOW * ptr = result; ptr; ptr = ptr->ai_next)
    {
        if (rproc(ptr)) return true;
    }
    return false;
}

std::u8string inet_ntoa(sockaddr const* addr, DWORD addrsz, LPWSAPROTOCOL_INFOW lpProtocolInfo)
{
    using arr_type = shared_optimized_array<wchar_t, 48, uint16_t>;
    arr_type temparr{(size_t)46};
    for (;;) {
        DWORD sz = (DWORD)temparr.size();
        int r = WSAAddressToStringW(const_cast<sockaddr*>(addr), addrsz, lpProtocolInfo, temparr.begin(), &sz);
        if (!r) return utf16_to_utf8((std::wstring_view)temparr.to_span().subspan(0, sz));
        DWORD err = WSAGetLastError();
        if (WSAEFAULT == err) {
            temparr = arr_type(temparr.size() * 2);
            continue;
        }
        throw exception("can't understand address error: %1%"_fmt % error_message(err));
    }
}

SOCKET create_socket(int af, int type, int protocol)
{
    SOCKET sock = socket(af, type, protocol);
    if (sock == INVALID_SOCKET) {
        DWORD err = WSAGetLastError();
        throw exception("can't create socket, error: %1%"_fmt % error_message(err));
    }
    return sock;
}

void bind_socket(SOCKET soc, sockaddr * name, int namelen)
{
    if (int iResult = bind(soc, name, namelen); SOCKET_ERROR == iResult) {
        DWORD err = WSAGetLastError();
        throw exception("can't bind socket, error: %1%"_fmt % error_message(err));
    }
}

void connect_socket(SOCKET soc, sockaddr * name, int namelen)
{
    if (int iResult = ::connect(soc, name, namelen); SOCKET_ERROR == iResult) {
        DWORD err = WSAGetLastError();
        throw exception("can't connect socket, error: %1%"_fmt % error_message(err));
    }
}

void listen_socket(SOCKET soc, int bl)
{
    if (int iResult = ::listen(soc, bl); SOCKET_ERROR == iResult) {
        DWORD err = WSAGetLastError();
        throw exception("can't listen to socket, error: %1%"_fmt % error_message(err));
    }
}

void setsockopt(SOCKET s, int level, int optname, const char* val, int optlen)
{
    if (::setsockopt(s, level, optname, val, optlen) < 0) {
        DWORD err = WSAGetLastError();
        throw exception("can't setsockopt, optname: %1%, error: %2%"_fmt % optname % error_message(err));
    }
}

HANDLE create_completion_port(uint32_t thread_count)
{
    HANDLE result = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, thread_count);
    if (!result) {
        DWORD err = GetLastError();
        throw exception("Can't create completion port, error: %1%"_fmt % error_message(err));
    }
    return result;
}

void assign_completion_port(HANDLE h, HANDLE iocp, ULONG_PTR key)
{
    if (!CreateIoCompletionPort(h, iocp, key, 0)) {
        DWORD err = GetLastError();
        throw exception("can't assign completion port, error: %1%"_fmt % error_message(err));
    }
}

void post_completion_port(HANDLE cp, DWORD btransf, ULONG_PTR key, OVERLAPPED * pov)
{
    if (!PostQueuedCompletionStatus(cp, btransf, key, pov)) {
        DWORD err = GetLastError();
        throw exception("Can't post into completion port queue, error: %1%"_fmt % error_message(err));
    }
}

void async_recvfrom(SOCKET soc, void * buff, size_t sz, SOCKADDR * sa, int * sasz, WSAOVERLAPPED * pov)
{
    WSABUF wsabuf;
    wsabuf.len = (ULONG)sz;
    wsabuf.buf = reinterpret_cast<char*>(buff);

    DWORD flags = 0;

    int rc = WSARecvFrom(soc, &wsabuf, 1, NULL, &flags, sa, sasz, pov, NULL);
    if (rc == SOCKET_ERROR) {
        DWORD err = WSAGetLastError();
        if (WSA_IO_PENDING != err) {
            throw eof_exception("can't async receive data from socket, error: %1%"_fmt % error_message(err));
        }
    }
}

std::error_code async_recv(SOCKET soc, void * buff, size_t sz, WSAOVERLAPPED * pov) noexcept
{
    WSABUF wsabuf;
    wsabuf.len = (ULONG)sz;
    wsabuf.buf = reinterpret_cast<char*>(buff);

    DWORD flags = 0;

    int rc = WSARecv(soc, &wsabuf, 1, NULL, &flags, pov, NULL);
    if (rc == SOCKET_ERROR) { 
        DWORD err = WSAGetLastError();
        if (WSA_IO_PENDING != err) {
            return std::error_code{(int)err, std::system_category()};
        }
    }
    return {};
}

std::error_code async_send(SOCKET soc, void const * buff, size_t sz, WSAOVERLAPPED * pov) noexcept
{
    WSABUF wsabuf;
    wsabuf.len = (ULONG)sz;
    wsabuf.buf = const_cast<char*>(reinterpret_cast<char const*>(buff));

    DWORD sentsz;
    int rc = WSASend(soc, &wsabuf, 1, &sentsz, 0, pov, NULL);
    if (rc == SOCKET_ERROR) {
        DWORD err = WSAGetLastError();
        if (WSA_IO_PENDING != err) {
            return std::error_code{(int)err, std::system_category()};
        }
    }
    return {};
}

void async_send_to(SOCKET soc, sockaddr const* addr, int addrlen, void const * buff, size_t sz, WSAOVERLAPPED * pov)
{
    WSABUF wsabuf;
    wsabuf.len = (ULONG)sz;
    wsabuf.buf = const_cast<char*>(reinterpret_cast<char const*>(buff));

    DWORD sentsz;
    int rc = WSASendTo(soc, &wsabuf, 1, &sentsz, 0, addr, addrlen, pov, NULL);
    if (rc == SOCKET_ERROR) {
        DWORD err = WSAGetLastError();
        if (WSA_IO_PENDING != err) {
            throw eof_exception("can't send data to socket, error: %1%"_fmt % error_message(err));
        }
    }
}

LPFN_ACCEPTEX get_accept_function(SOCKET soc)
{
    LPFN_ACCEPTEX lpfnAcceptEx;
    GUID GuidAcceptEx = WSAID_ACCEPTEX;
    DWORD dwBytes;

    if (SOCKET_ERROR == WSAIoctl(soc, SIO_GET_EXTENSION_FUNCTION_POINTER,
        &GuidAcceptEx, sizeof (GuidAcceptEx), 
        &lpfnAcceptEx, sizeof (lpfnAcceptEx), 
        &dwBytes, NULL, NULL))
    {
        DWORD err = WSAGetLastError();
        throw exception("can't retrieve accept function for the given socket, error: %1%"_fmt % error_message(err));
    }

    return lpfnAcceptEx;
}

HANDLE create_file(
    const wchar_t* file_name,
    DWORD desired_access,
    DWORD share_mode,
    LPSECURITY_ATTRIBUTES security_attributes,
    DWORD creation_disposition,
    DWORD flags_and_attributes,
    HANDLE template_file)
{
    HANDLE result = CreateFileW(file_name, desired_access, share_mode, security_attributes, creation_disposition, flags_and_attributes, template_file);
    if (INVALID_HANDLE_VALUE == result)
    {
        DWORD err = GetLastError();
        throw exception(error_message(err));
    }
    return result;
}

std::u8string get_file_name(HANDLE hFile)
{
    std::vector<WCHAR> buf(64);
    DWORD dwRet = GetFinalPathNameByHandleW(hFile, &buf.front(), (DWORD)buf.size(), FILE_NAME_NORMALIZED);
    if (dwRet >= buf.size()) {
        buf.resize(dwRet);
        dwRet = GetFinalPathNameByHandleW(hFile, &buf.front(), (DWORD)buf.size(), FILE_NAME_NORMALIZED);
    }
    if (!dwRet || dwRet >= buf.size()) {
        DWORD err = WSAGetLastError();
        throw exception("can't retrieve file name from handle, error: %1%"_fmt % error_message(err));
    }
    buf[dwRet] = 0;
    return utf16_to_utf8(std::wstring_view(&buf.front(), dwRet));
}

void delete_file(wchar_t const * path)
{
    if (!DeleteFileW(path)) {
        DWORD err = GetLastError();
        throw exception("can't delete file %1%, error : %2%"_fmt % (const char*)utf16_to_utf8(path).c_str() % error_message(err));
    }
}

void async_read_file(HANDLE handle, uint64_t fileoffset, void * buff, size_t sz, OVERLAPPED * pov)
{
    pov->Offset = (DWORD)(fileoffset & 0xFFFFFFFF);
    pov->OffsetHigh = (DWORD)(fileoffset >> 32);

    DWORD sz2read = sz > 0xffffFFFF ? 0xffffFFFF : (DWORD)sz;
    SetLastError(0);
    if (!ReadFileEx(handle, buff, sz2read, pov, NULL)) {
        DWORD err = GetLastError();
        if (ERROR_IO_PENDING != err) {
            throw exception("read file error : %1%"_fmt % error_message(err));
        }
    }
}

void async_write_file(HANDLE handle, uint64_t fileoffset, void const * buff, size_t sz, OVERLAPPED * pov)
{
    pov->Offset = (DWORD)(fileoffset & 0xFFFFFFFF);
    pov->OffsetHigh = (DWORD)(fileoffset >> 32);

    DWORD sz2write = sz > 0xffffFFFF ? 0xffffFFFF : (DWORD)sz;
    SetLastError(0);
    if (!WriteFileEx(handle, buff, sz2write, pov, NULL)) {
        DWORD err = GetLastError();
        if (ERROR_IO_PENDING != err) {
            throw exception("write file error : %1%"_fmt % error_message(err));
        }
    }
}

void ioctl(
    HANDLE device,
    DWORD io_control_code,
    LPVOID in_buffer,
    DWORD in_buffer_size,
    LPVOID out_buffer,
    DWORD out_buffer_size,
    LPDWORD bytes_returned,
    LPOVERLAPPED overlapped
)
{
    if (!DeviceIoControl(device, io_control_code, in_buffer, in_buffer_size, out_buffer, out_buffer_size, bytes_returned, overlapped)) {
        DWORD err = GetLastError();
        throw exception(error_message(err));
    }
}

}
