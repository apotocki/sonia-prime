//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_LINUX_HPP
#define SONIA_UTILITY_LINUX_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <netinet/in.h>

#include <tuple>
#include <string>

namespace sonia { namespace linux {

std::tuple<int, int, int> kernel_version();

std::string inet_ntoa(sockaddr_in const* addr);

int create_socket(int af, int type, int protocol);
void connect(int s, const sockaddr *addr, socklen_t addrlen);
void bind_socket(int sockfd, const sockaddr *addr, socklen_t addrlen);
void listen(int sockfd, int backlog);
void setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

void append_descriptor_flags(int fd, int appending_flags);

}}

#endif // SONIA_UTILITY_LINUX_HPP
