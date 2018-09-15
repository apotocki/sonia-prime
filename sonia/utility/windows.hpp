//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#define WIN32_LEAN_AND_MEAN
#define UNICODE
//#define WINVER 0x0600
//#define _WIN32_WINNT 0x0600

#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <mswsock.h>

#include <boost/thread/thread.hpp>
#include "sonia/string.hpp"
#include "sonia/function.hpp"

namespace sonia { namespace windows {

class wsa_scope final {
public:
    wsa_scope();
    ~wsa_scope();
};

void set_thread_name(DWORD dwThreadId, char const* threadName);
void set_thread_name(boost::thread::id tid, char const* threadName);

std::basic_string<char16_t> utf8_to_utf16(string_view);
std::string error_message(DWORD errcode);

LPFN_ACCEPTEX get_accept_function(SOCKET);
bool parse_address(string_view address, uint16_t port, function<bool(ADDRINFOW*)> rproc);

SOCKET create_socket(int af, int type, int protocol);
void async_recv(SOCKET soc, void * buff, size_t sz, WSAOVERLAPPED * pov);



HANDLE  create_completion_port(uint32_t thread_count);
void    assign_completion_port(HANDLE h, HANDLE iocp, ULONG_PTR key);
void    post_completion_port(HANDLE cp, DWORD btransf, ULONG_PTR key, OVERLAPPED * pov = nullptr);


}}