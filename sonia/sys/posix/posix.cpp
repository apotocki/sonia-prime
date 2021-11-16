//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sys/posix/posix.hpp"
#include "sonia/exceptions.hpp"

#include <sys/utsname.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <string>
#include <cstdlib>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

namespace sonia::posix {

std::tuple<int, int, int> kernel_version()
{
    utsname nm;
    if (uname(&nm)) {
        int err = errno;
        throw exception("can't retrieve the information about current kernel, error : %1%"_fmt % strerror(err));
    }

    std::string kname(nm.release);

    std::vector<std::string> svec;
    boost::split(svec, kname, boost::is_any_of("."), boost::token_compress_on);

    std::tuple<int, int, int> result{ 0, 0, 0 };
    if (svec.size() > 0) std::get<0>(result) = atoi(svec[0].c_str());
    if (svec.size() > 1) std::get<1>(result) = atoi(svec[1].c_str());
    if (svec.size() > 2) std::get<2>(result) = atoi(svec[2].c_str());
    return result;
}

std::string inet_ntoa(sockaddr_in const* addr)
{
    char buff[INET6_ADDRSTRLEN];
    const char* r = inet_ntop(
        addr->sin_family,
        AF_INET == addr->sin_family ?
        (void const*)&addr->sin_addr :
        (void const*)&((sockaddr_in6 const*)addr)->sin6_addr,
        buff,
        sizeof(buff)
    );
    if (!r) {
        int err = errno;
        throw exception("can't retrieve the address, error : %1%"_fmt % strerror(err));
    }
    return std::string(r);
}

int create_socket(int af, int type, int protocol)
{
    int sockfd = socket(af, type, protocol);
    if (sockfd == -1) {
        int err = errno;
        throw exception("can't create socket, error: %1%"_fmt % strerror(err));
    }
    return sockfd;
}

void connect(int s, const sockaddr* addr, socklen_t addrlen)
{
    if (int ir = ::connect(s, addr, addrlen); ir == -1) {
        int err = errno;
        throw exception("can't connect socket, error: %1%"_fmt % strerror(err));
    }
}

void bind_socket(int sockfd, const sockaddr* addr, socklen_t addrlen)
{
    if (::bind(sockfd, addr, addrlen)) {
        int err = errno;
        throw exception("can't bind socket, error: %1%"_fmt % strerror(err));
    }
}

void listen(int sockfd, int backlog)
{
    if (int r = ::listen(sockfd, SOMAXCONN); r != 0) {
        int err = errno;
        throw exception("can't listen socket, error: %1%"_fmt % strerror(err));
    }
}

void setsockopt(int sockfd, int level, int optname, const void* optval, socklen_t optlen)
{
    if (int r = ::setsockopt(sockfd, level, optname, optval, optlen); r != 0) {
        int err = errno;
        throw exception("can't setsockopt, optname: %1%, error: %2%"_fmt % optname % strerror(err));
    }
}

void append_descriptor_flags(int fd, int appending_flags)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        int err = errno;
        throw exception("can't get descriptor flags, error: %1%"_fmt % strerror(err));
    }

    flags |= appending_flags;
    int s = fcntl(fd, F_SETFL, flags);
    if (s == -1)
    {
        int err = errno;
        throw exception("can't set descriptor flags, error: %1%"_fmt % strerror(err));
    }
}

}
