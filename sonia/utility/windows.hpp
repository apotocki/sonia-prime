//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#define WIN32_LEAN_AND_MEAN
#ifndef UNICODE
#   define UNICODE
#endif
//#define WINVER 0x0600
#ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0600
#endif

#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <mswsock.h>

#include <boost/thread/thread.hpp>
#include "sonia/string.hpp"
#include "sonia/function.hpp"

namespace sonia { namespace windows {

class wsa_scope final
{
public:
    wsa_scope();
    ~wsa_scope();
};

void set_thread_name(DWORD dwThreadId, char const* threadName);
void set_thread_name(boost::thread::id tid, char const* threadName);

std::wstring utf8_to_utf16(string_view);
std::string utf16_to_utf8(wstring_view);

std::string error_message(DWORD errcode);

LPFN_ACCEPTEX get_accept_function(SOCKET);
bool parse_address(int hint_type, int hint_protocol, string_view address, uint16_t port, function<bool(ADDRINFOW*)> rproc);

std::string inet_ntoa(sockaddr const* addr, DWORD sz, LPWSAPROTOCOL_INFOW);

SOCKET create_socket(int af, int type, int protocol);
void bind_socket(SOCKET soc, sockaddr * name, int namelen);

void async_recv(SOCKET soc, void * buff, size_t sz, WSAOVERLAPPED * pov);
void async_recvfrom(SOCKET soc, void * buff, size_t sz, SOCKADDR * sa, int * sasz, WSAOVERLAPPED * pov);
void async_send(SOCKET soc, void const * buff, size_t sz, WSAOVERLAPPED * pov);
void async_send_to(SOCKET soc, sockaddr const* addr, int addrlen, void const * buff, size_t sz, WSAOVERLAPPED * pov);

HANDLE  create_completion_port(uint32_t thread_count);
void    assign_completion_port(HANDLE h, HANDLE iocp, ULONG_PTR key);
void    post_completion_port(HANDLE cp, DWORD btransf, ULONG_PTR key, OVERLAPPED * pov = nullptr);

// file operations
std::string get_file_name(HANDLE hFile); // returns utf8 string
void delete_file(wchar_t const * path, char const* optutf8path = nullptr);
void async_read_file(HANDLE, uint64_t fileoffset, void * buff, size_t sz, OVERLAPPED * pov);
void async_write_file(HANDLE, uint64_t fileoffset, void const* buff, size_t sz, OVERLAPPED * pov);

}}
