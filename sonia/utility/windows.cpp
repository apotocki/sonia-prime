//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/utility/windows.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/optimized/array.hpp"

#include <boost/lexical_cast.hpp>

#pragma comment(lib, "ws2_32.lib")

const DWORD MS_VC_EXCEPTION=0x406D1388;

namespace sonia::windows {

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

bool parse_address(int hint_af, int hint_type, int hint_protocol, string_view address, uint16_t port, function<bool(ADDRINFOW*)> rproc)
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

std::string inet_ntoa(sockaddr const* addr, DWORD addrsz, LPWSAPROTOCOL_INFOW lpProtocolInfo)
{
    using arr_type = shared_optimized_array<wchar_t, 48, uint16_t>;
    arr_type temparr{(size_t)46};
    for (;;) {
        DWORD sz = (DWORD)temparr.size();
        int r = WSAAddressToStringW(const_cast<sockaddr*>(addr), addrsz, lpProtocolInfo, temparr.begin(), &sz);
        if (!r) return utf16_to_utf8(temparr.to_array_view().subview(0, sz));
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

std::string get_file_name(HANDLE hFile)
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
    return utf16_to_utf8(wstring_view(&buf.front(), dwRet));
}

void delete_file(wchar_t const * path, char const* optutf8path)
{
    if (!DeleteFileW(path)) {
        DWORD err = GetLastError();
        throw exception("can't delete file %1%, error : %2%"_fmt % (optutf8path ? optutf8path : utf16_to_utf8(path).c_str()) % error_message(err));
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

}
