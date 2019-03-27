//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
//============================================================================
#include "sonia/config.hpp"
#include <boost/test/unit_test.hpp>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include <iostream>
#include <vector>
#include <boost/thread.hpp>
#include "sonia/type_traits.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/logger/logger.hpp"

using namespace sonia;

static constexpr uint64_t epool_exit_cookie_v = (std::numeric_limits<uint64_t>::max)();

#ifndef MAX_EPOLL_EVENTS
#   define MAX_EPOLL_EVENTS 5
#endif

void set_descriptor_flags(int fd, int appending_flags)
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

template <typename OnErrorHandlerT>
void epoll_ctl(int efd, int fd, uint64_t cookie, uint32_t evflags, OnErrorHandlerT const& errh)
{
    epoll_event ev;
    ev.data.u64 = cookie;
    ev.events = evflags;
    if (-1 == ::epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev)) {
        int err = errno;
        errh(strerror(err));
    }
}

void bind(int fd, const char* address, uint16_t port)
{
    addrinfo hints;
    bzero((char*)&hints, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    addrinfo *rp;
    if (int s = getaddrinfo(address, std::to_string(port).c_str(), &hints, &rp); s != 0 || !rp) {
        throw exception("address '%1%' is not valid, error: %2%"_fmt % address % gai_strerror(s));
    }
    SCOPE_EXIT([rp] { freeaddrinfo(rp); });

    std::ostringstream errmsgs;
    for (; !!rp; rp = rp->ai_next) {
        if (!bind(fd, rp->ai_addr, rp->ai_addrlen)) return;
        int err = errno;
        if (!errmsgs.str().empty()) errmsgs << '\n';
        errmsgs << strerror(err);
    }

    throw exception("can't bind socket to '%1% : %2%', error(s): %3%"_fmt % address % port % errmsgs.str());
}

void listen(int epollfd, int fd, int flags)
{
    if (int r = listen(fd, SOMAXCONN); r != 0) {
        int err = errno;
        throw exception("can't listen socket, error: %1%"_fmt % strerror(err));
    }

    epoll_ctl(epollfd, fd, (uint32_t)fd, flags, [](const char * msg) {
        throw exception("can't watch acceptor, error: %1%"_fmt % msg);
    });
}

int create_client_socket(const char* address, uint16_t port)
{
    addrinfo hints;
    bzero((char*)&hints, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    addrinfo *result;
    if (int s = getaddrinfo(address, std::to_string(port).c_str(), &hints, &result); s != 0 || !result) {
        throw exception("tcp socket address '%1%' is not valid, error: %2%"_fmt % address % gai_strerror(s));
    }
    SCOPE_EXIT([result] { freeaddrinfo(result); });

    addrinfo * rp;
    for (rp = result; !!rp; rp = rp->ai_next) {
        int sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1) {
            int err = errno;
            GLOBAL_LOG_ERROR() << "can't create socket, error: " << strerror(err);
            continue;
        }
        GLOBAL_LOG_INFO() << "client socket created: " << sockfd;
        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            return sockfd;
        }
        int err = errno;
        GLOBAL_LOG_ERROR() << "can't create socket, error: " << strerror(err);
    }

    throw exception("can't connect socket to '%1% : %2%'"_fmt % address % port);
}

void test_1_epoll_n_threads()
{
    int epollfd;
    if (epollfd = epoll_create1(0); epollfd == -1) {
        int err = errno;
        throw exception("can't create epoll instance, error: %1%"_fmt % strerror(err));
    }

    // termination pipe
    int ctl_pipe[2];
    if (int r = ::pipe(ctl_pipe); r != 0) {
        int err = errno;
        throw exception("can't create a contorl pipe instance, error: %1%"_fmt % strerror(err));
    }
    set_descriptor_flags(ctl_pipe[0], O_NONBLOCK); // read
    epoll_ctl(epollfd, ctl_pipe[0], epool_exit_cookie_v, EPOLLIN, [](const char* msg) {
        throw exception("can't start watch the controll pipe, error: %1%"_fmt % msg);
    });

    std::vector<boost::thread> threads;
    for (size_t tid = 0; tid < 2; ++tid) {
        threads.emplace_back([tid, epollfd]()
        {
            epoll_event events[MAX_EPOLL_EVENTS];
            for (;;)
            {
                GLOBAL_LOG_INFO() << "thread #" << tid << " start waiting";
                int n = epoll_wait(epollfd, events, MAX_EPOLL_EVENTS, -1);
                GLOBAL_LOG_INFO() << "thread #" << tid << " wakes up with n = " << n;
                boost::this_thread::sleep(boost::posix_time::milliseconds(50));
                for (int i = 0; i < n; i++) {
                    if (epool_exit_cookie_v == events[i].data.u64) {
                        GLOBAL_LOG_INFO() << "thread #" << tid << " terminated";
                        return;
                    }
                    int lfd = (int)events[i].data.u32;
                    continue;
                    for (;;) {
                        sockaddr in_addr;
                        socklen_t in_len = sizeof(in_addr);
                        int infd = accept(lfd, &in_addr, &in_len);
                        if (infd == -1) {
                            int err = errno;
                            if (err != EAGAIN) {
                                GLOBAL_LOG_INFO() << "thread #" << tid << " accept error: " << strerror(err);
                            } else {
                                break;
                            }
                        }
                        GLOBAL_LOG_INFO() << "thread #" << tid << " accepted fd: " << infd;
                        //boost::this_thread::sleep(boost::posix_time::milliseconds(200));
                    }
                }
            }
        });
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    GLOBAL_LOG_INFO() << "threads started";
    
    // create listener socket
    int listener_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listener_fd == -1) {
        int err = errno;
        throw exception("can't create socket, error: %1%"_fmt % strerror(err));
    }
    set_descriptor_flags(listener_fd, O_NONBLOCK);
    bind(listener_fd, "localhost", 2222);
    listen(epollfd, listener_fd, EPOLLIN | EPOLLET); // EPOLLEXCLUSIVE

    // create client socket
    int client_fd0 = create_client_socket("localhost", 2222);
    GLOBAL_LOG_INFO() << "client " << client_fd0 << " connected";
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    int client_fd1 = create_client_socket("localhost", 2222);
    GLOBAL_LOG_INFO() << "client " << client_fd1 << " connected";

    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    ::close(client_fd0);
    ::close(client_fd1);

    GLOBAL_LOG_INFO() << "terminating...";
    char ch = 'e';
    if (int r = ::write(ctl_pipe[1], &ch, 1); 1 != r) {
        int err = errno;
        throw exception("can't park threads, error: "_fmt % strerror(err));
    }
    for (auto & t : threads) {
        t.join();
    }
    GLOBAL_LOG_INFO() << "all threads have joined";
}

BOOST_AUTO_TEST_CASE (epoll_test)
{
    try {
        test_1_epoll_n_threads();
    } catch (std::exception const& e) {
        GLOBAL_LOG_INFO() << e.what() << "\n";
    }
}
