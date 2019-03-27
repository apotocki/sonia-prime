//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "factory.hpp"

#include <aio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>

#include <sys/epoll.h>

#include <atomic>
#include <sstream>

#include <boost/unordered_map.hpp>

#include "sonia/utility/posix/signals.hpp"

#include "sonia/concurrency.hpp"
#include "sonia/optional.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/object_pool.hpp"
#include "sonia/exceptions/internal_errors.hpp"
#include "sonia/utility/linux.hpp"

#ifndef MAX_EPOLL_EVENTS
#   define MAX_EPOLL_EVENTS 5
#endif

#ifndef SOCKET_POOL_SIZE
#   define SOCKET_POOL_SIZE 128
#endif

namespace sonia::io {

static constexpr bool IO_DEBUG = false;
static constexpr uint64_t epool_exit_cookie_v = (std::numeric_limits<uint64_t>::max)();

namespace linapi = sonia::linux;

struct lin_impl;

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

#if 1
struct unique_fd
{
    explicit unique_fd(int fd = -1) noexcept : value(fd) {}
    unique_fd(unique_fd const&) = delete;
    unique_fd(unique_fd && rhs) noexcept : value(rhs.value) { rhs.value = -1; }
    unique_fd & operator=(unique_fd const&) = delete;
    unique_fd & operator=(unique_fd && rhs) noexcept
    {
        if (value != rhs.value) {
            if (value >= 0) do_close();
            value = rhs.value;
            rhs.value = -1;
        }
        return *this;
    }

    ~unique_fd()
    {
        if (value >= 0) do_close();
    }

    int get() const noexcept { return value; }
    int operator*() const noexcept { return value; }
    int detach() noexcept
    {
        int ret = value;
        value = -1;
        return ret;
    }

    void close()
    {
        do_close();
        value = -1;
    }

    int value;

private:
    void do_close() noexcept
    {
        if (0 != ::close(value)) {
            int err = errno;
            GLOBAL_LOG_ERROR() << "errror while closing socket: " << strerror(err);
        }
    }
};
#endif

struct lin_shared_handle : shared_handle<socket_handle_traits>
{
    using base_type = shared_handle<socket_handle_traits>;

    lin_shared_handle(handle_type h, uint32_t id) : base_type(h), bkid_(id) {}

    int fd() const noexcept { return handle; }
    uint32_t bkid() const { return bkid_; }

    virtual void on_event() noexcept
    {
        unique_lock lck(mtx);
        cnd.notify_one();
    };

    std::atomic<size_t> waiting_cnt{0};

    void wait(unique_lock<fibers::mutex> & lck)
    {
        cnd.wait(lck);
    }

    fibers::mutex mtx;
    fibers::condition_variable_any cnd;

private:
    uint32_t bkid_;
};

using tcp_scoped_handle = scoped_handle<tcp_socket_service_type*, lin_shared_handle>;
using udp_scoped_handle = scoped_handle<udp_socket_service_type*, lin_shared_handle>;

struct lin_shared_handle_hasher
{
    size_t operator()(lin_shared_handle * ph) const { return ph->bkid(); }
    size_t operator()(uint32_t id) const { return id; }
};

struct lin_shared_handle_equal_to
{
    size_t operator()(lin_shared_handle * lhs, lin_shared_handle * rhs) const { return lhs == rhs; }
    size_t operator()(lin_shared_handle * lhs, uint32_t rhs) const { return lhs->bkid() == rhs; }
    size_t operator()(uint32_t lhs, lin_shared_handle * rhs) const { return lhs == rhs->bkid(); }
};

struct lin_impl 
    : public factory::impl_base
    , public enable_shared_from_this<lin_impl>
{
    friend class factory;

    int ctl_pipe[2];

    int efd;

    sonia::spin_mutex bk_mtx_;
    std::atomic<uint32_t> bkid_cnt_{0};
    boost::unordered_set<lin_shared_handle*, lin_shared_handle_hasher> book_keeper_;

    object_pool<lin_shared_handle, spin_mutex> sockets_;
    object_pool<lin_shared_handle, spin_mutex> acceptors_;

    lin_impl(shared_ptr<factory> itself, uint32_t thr_cnt);

    lin_shared_handle * new_socket_handle(int sockfd);
    void delete_socket_handle(lin_shared_handle * h) noexcept;

    //acceptor_shared_handle * new_acceptor_handle(int sockfd, std::function<void(tcp_acceptor_handler_type const&)> const&);
    //void delete_acceptor_handle(lin_shared_handle * h) noexcept;

    void park_threads() noexcept override
    {
        LOG_TRACE(wrapper->logger()) << "parking threads...";
        char ch = 'e';
        if (int r = ::write(ctl_pipe[1], &ch, 1); 1 != r) {
            int err = errno;
            LOG_ERROR(wrapper->logger()) << "can't park threads, error: " << strerror(err);
        }
    }

    ~lin_impl() override
    {
        ::close(ctl_pipe[0]);
        ::close(ctl_pipe[1]);
        ::close(efd);
    }

    void thread_proc() override;

    tcp_socket do_create_tcp_socket(sonia::sal::socket_handle) override;

    std::pair<tcp_socket, size_t> tcp_socket_accept(tcp_handle_type, char*, size_t) override;
    size_t tcp_socket_waiting_count(tcp_handle_type) override;
    expected<size_t, std::error_code> tcp_socket_read_some(tcp_handle_type, void * buff, size_t sz) override;
    expected<size_t, std::error_code> tcp_socket_write_some(tcp_handle_type, void const* buff, size_t sz) override;
    void tcp_socket_close(tcp_handle_type) noexcept override;
    void free_handle(identity<tcp_socket_service_type>, tcp_handle_type) noexcept override;

    //void add_to_bk(lin_shared_handle * sh)
    //{
    //    lock_guard guard(bk_mtx_);
    //    if (BOOST_UNLIKELY(!book_keeper_.insert(sh).second)) {
    //        throw internal_error("bk error: an attempt to add already monitored descriptor %1%"_fmt % fd);
    //    }
    //}

    //void remove_from_bk(int fd) noexcept
    //{
    //    ::epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL);

    //    size_t sz;
    //    {
    //        lock_guard guard(bk_mtx_);
    //        sz = book_keeper_.erase(fd);
    //    }

    //    if (BOOST_UNLIKELY(0 == sz)) {
    //        LOG_WARN(wrapper->logger()) << "can't fine the descriptor " << fd << " in BK";
    //    }
    //}

    //void free(socket_handle * sh) noexcept;
    //void free(acceptor_socket_handle * sh) noexcept;
};

//inline shared_ptr<lin_impl> get_dataptr(weak_ptr<factory::impl_base> const& wptr)
//{
//    shared_ptr<lin_impl> r = static_pointer_cast<lin_impl>(wptr.lock());
//    if (!r) throw closed_exception();
//    return r;
//}

#define dataptr static_pointer_cast<lin_impl>(get_dataptr())

#if 0
struct socket_handle : socket_handle_base
{
    fibers::mutex mtx;
    fibers::condition_variable_any cnd;
    bool flag{false};

    socket_handle(lin_impl * impl, int sockfd)
        : socket_handle_base(sockfd)
    {
        impl->wrapper->on_add_callback();
        try {
            impl->add_to_bk(sockfd, this);
        } catch (...) {
            impl->wrapper->on_release_callback();
            throw;
        }

        try {
            epoll_ctl(impl->efd, sockfd, EPOLLIN | EPOLLOUT | EPOLLET, [](const char * msg) {
                throw exception("can't watch socket, error: %1%"_fmt % msg);
            });
        } catch (...) {
            impl->remove_from_bk(sockfd);
            impl->wrapper->on_release_callback();
            throw;
        }
    }

    void on_event(int efd, uint32_t evflags) noexcept override
    {
        if constexpr (IO_DEBUG) GLOBAL_LOG_INFO() << "SOCKET EVENT! " << ((evflags & EPOLLIN) ? "IN" : "") << " " << ((evflags & EPOLLOUT) ? "OUT" : "");
        unique_lock lck(mtx);
        flag = true;
        cnd.notify_one();
    }

    void free(lin_impl* impl) noexcept override { impl->free(this); }

    void throw_socket_error(string_view errmsg) const
    {
        int err;
        socklen_t len = sizeof(int);
        if (!::getsockopt(sock.value, SOL_SOCKET, SO_ERROR, &err, &len)) {
            throw exception("%1%, sockopt error: %2%"_fmt % errmsg % strerror(err));
        }
        throw exception("%1%, unknown error"_fmt % errmsg);
    }

    void connect(addrinfo * rp)
    {
        unique_lock lck(mtx);
        for (;;)
        {
            if (::connect(sock.value, rp->ai_addr, rp->ai_addrlen) != -1) {
                return;
            }
            int err = errno;
            if (BOOST_UNLIKELY(EINPROGRESS != err)) {
                throw exception("can't connect socket, error: %1%"_fmt % strerror(err));
            }
            flag = false;
            cnd.wait(lck, [this] { return flag; });
        }
    }

    size_t write_some(void const* buff, size_t sz) noexcept
    {
        unique_lock lck(mtx);
        for (;;)
        {
            ssize_t n = ::write(sock.value, buff, sz);
            if (n >= 0) return (size_t)n;
            int err = errno;
            if (BOOST_UNLIKELY(EAGAIN != err)) { return 0; }
            flag = false;
            if constexpr (IO_DEBUG) GLOBAL_LOG_TRACE() << to_string("socket(%1%) write waiting..."_fmt % sock.value);
            cnd.wait(lck, [this] { return flag; });
            if constexpr (IO_DEBUG) GLOBAL_LOG_TRACE() << to_string("socket(%1%) woke up"_fmt % sock.value);
        }
    }

    size_t read_some(void * buff, size_t sz) noexcept
    {
        unique_lock lck(mtx);
        for (;;)
        {
            ssize_t n = ::read(sock.value, buff, sz);
            if constexpr (IO_DEBUG) GLOBAL_LOG_TRACE() << to_string("socket(%1%) read %2% bytes"_fmt % sock.value % n);
            if (n >= 0) return (size_t)n;
            int err = errno;
            if (BOOST_UNLIKELY(EAGAIN != err)) { return 0; }
            flag = false;
            if constexpr (IO_DEBUG) GLOBAL_LOG_TRACE() << to_string("socket(%1%) read waiting..."_fmt % sock.value);
            cnd.wait(lck, [this] { return flag; });
            if constexpr (IO_DEBUG) GLOBAL_LOG_TRACE() << to_string("socket(%1%) woke up"_fmt % sock.value);
        }
    }
};
#endif

#if 0
struct acceptor_socket_handle : socket_handle_base
{
    lin_impl * impl_;
    function<void(tcp_socket_service_type::connection_factory_t const&)> handle_;

    template <typename HandleT>
    acceptor_socket_handle(lin_impl * impl, int sockfd, HandleT const& h)
        : socket_handle_base(sockfd), impl_(impl), handle_(h)
    {
        impl_->wrapper->on_add_callback();
        try {
            impl->add_to_bk(sockfd, this);
        } catch (...) {
            impl_->wrapper->on_release_callback();
            throw;
        }
        
        try {
            epoll_ctl(impl->efd, sockfd, impl->acceptor_flags, [](const char * msg) {
                throw exception("can't watch acceptor, error: %1%"_fmt % msg);
            });
        } catch (...) {
            impl->remove_from_bk(sockfd);
            impl_->wrapper->on_release_callback();
            throw;
        }
    }

    void on_event(int efd, uint32_t evflags) noexcept override
    {
        int ent{0};
        if constexpr (IO_DEBUG)
        {
            static std::atomic<int> enters{0};
            ent = enters.fetch_add(1);
        }

        if constexpr (IO_DEBUG) LOG_TRACE(impl_->wrapper->logger()) << "ACCEPTOR EVENT " << ent;
        for (;;) {
            sockaddr in_addr;
            socklen_t in_len = sizeof(in_addr);
            int infd = accept(sock.value, &in_addr, &in_len);
            if (infd == -1) {
                if constexpr (IO_DEBUG) {
                    int err = errno;
                    if (err != EAGAIN) {
                        LOG_TRACE(impl_->wrapper->logger()) << "accept (" << ent << ") error: " << strerror(err);
                    }
                }
                break;
            }

            unique_fd ufd{infd};

            if constexpr (IO_DEBUG) {
                char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
                getnameinfo(&in_addr, in_len,
                    hbuf, sizeof(hbuf),
                    sbuf, sizeof(sbuf),
                    NI_NUMERICHOST | NI_NUMERICSERV);

                LOG_TRACE(impl_->wrapper->logger()) << hbuf << " " << sbuf << "(" << ent << ")";
            }

            linapi::append_descriptor_flags(infd, O_NONBLOCK);

            try {
                handle_([this, efd, sockfd = ufd.value](array_view<char> buff, tcp_acceptor_factory::connector_t const& conn) {
                    ssize_t sz = ::read(sockfd, buff.begin(), buff.size());
                    conn(tcp_socket_access::create_tcp_socket(impl_->wrapper, (intptr_t)impl_->new_socket_handle(sockfd)), sz > 0 ? (size_t)sz : (size_t)0);
                });
                ufd.detach();
            } catch (...) {
                GLOBAL_LOG_ERROR() << boost::current_exception_diagnostic_information();
            }
        }
    }

    void free(lin_impl*) noexcept override { impl_->free(this); }
};
#endif

lin_impl::lin_impl(shared_ptr<factory> itself, uint32_t thr_cnt)
    : factory::impl_base(std::move(itself))
    , sockets_(SOCKET_POOL_SIZE)
    , acceptors_(SOCKET_POOL_SIZE)
{
    if (efd = epoll_create1(0); efd == -1) {
        int err = errno;
        throw exception("can't create epoll instance, error: %1%"_fmt % strerror(err));
    }

    if (int r = ::pipe(ctl_pipe); r != 0) {
        int err = errno;
        throw exception("can't create a contorl pipe instance, error: %1%"_fmt % strerror(err));
    }

    linapi::append_descriptor_flags(ctl_pipe[0], O_NONBLOCK); // read

    epoll_ctl(efd, ctl_pipe[0], epool_exit_cookie_v, EPOLLIN, [](const char* msg) {
        throw exception("can't start watch the controll pipe, error: %1%"_fmt % msg);
    });

    auto[a, b, c] = sonia::linux::kernel_version();
    LOG_INFO(wrapper->logger()) << "KERNEL: " << a << "." << b << "." << c;
}

void lin_impl::thread_proc()
{
    epoll_event events[MAX_EPOLL_EVENTS];
    for (;;) {
        if constexpr (IO_DEBUG) LOG_INFO(wrapper->logger()) << "before epoll_wait, thread: " << this_thread::get_id();
        int n = epoll_wait(efd, events, MAX_EPOLL_EVENTS, -1);
        if constexpr (IO_DEBUG) LOG_INFO(wrapper->logger()) << "woke up thread: " << this_thread::get_id();
        if (-1 == n) {
            int err = errno;
            LOG_ERROR(wrapper->logger()) << "epoll interrupted: " << strerror(err) ;
            return;
        }
        if (n == 0) {
            LOG_ERROR(wrapper->logger()) << "epoll WRONG WOKE UP";
        }
        for (int i = 0; i < n; i++) {
            if (epool_exit_cookie_v == events[i].data.u64) return; // close event
            uint32_t bkid = events[i].data.u32;

            lin_shared_handle* h;
            {
                lock_guard guard(bk_mtx_);
                auto it = book_keeper_.find(bkid, lin_shared_handle_hasher(), lin_shared_handle_equal_to());
                if (it == book_keeper_.end()) continue; // skip obsoleted descriptor event
                h = static_cast<lin_shared_handle*>(*it);
                h->add_weakref();
            }

            if (h) {
                h->on_event(); // noexcept
                h->release_weak((tcp_socket_service_type*)this);
            }
        }
    }
}

lin_shared_handle * lin_impl::new_socket_handle(int sockfd)
{
    for (;;) {
        lin_shared_handle *ph = sockets_.new_object(sockfd, bkid_cnt_.fetch_add(1));
        lock_guard guard(bk_mtx_);
        if (BOOST_LIKELY(book_keeper_.insert(ph).second)) {
            try {
                on_add_callback();
            } catch (...) {
                sockets_.delete_object(ph);
                throw;
            }
            ph->add_weakref();
            return ph;
        }
        sockets_.delete_object(ph);
    }
}

void lin_impl::delete_socket_handle(lin_shared_handle * h) noexcept
{
    {
        lock_guard guard(bk_mtx_);
        BOOST_VERIFY(1 == book_keeper_.erase(h));
    }

    sockets_.delete_object(h);
    on_release_callback();
}

tcp_socket lin_impl::do_create_tcp_socket(sonia::sal::socket_handle s)
{
    linapi::append_descriptor_flags(s, O_NONBLOCK);

    lin_shared_handle * sh = new_socket_handle(s);
    
    epoll_ctl(efd, s, sh->bkid(), EPOLLIN | EPOLLOUT | EPOLLET, [this, sh](const char * msg) {
        delete_socket_handle(sh);
        throw exception("can't watch acceptor, error: %1%"_fmt % msg);
    });

    return tcp_socket_access::create_tcp_socket<socket_traits>(shared_from_this(), sh);
}

//acceptor_shared_handle * lin_impl::new_acceptor_handle(int sockfd, std::function<void(tcp_acceptor_handler_type const&)> const& handler)
//{
//    for (;;) {
//        acceptor_shared_handle *ph = acceptors_.new_object(bkid_cnt_.fetch_add(1), sockfd, handler);
//        lock_guard guard(bk_mtx_);
//        if (BOOST_LIKELY(book_keeper_.insert(ph).second)) break;
//        acceptors_.delete_object(ph);
//    }
//    wrapper->on_add_callback();
//}

//void lin_impl::delete_socket_handle(acceptor_shared_handle * h) noexcept
//{
//    ::epoll_ctl(efd, EPOLL_CTL_DEL, h->fd(), NULL);
//
//    {
//        lock_guard guard(bk_mtx_);
//        book_keeper_.erase(h);
//    }
//
//    handlers_.delete_object(h);
//    wrapper->on_release_callback();
//}

//void lin_impl::free(lin_shared_handle * sh) noexcept
//{
//    sockets_.delete_object(sh);
//    wrapper->on_release_callback();
//}

factory::factory() {}

factory::~factory() {}

void factory::initialize_impl(uint32_t thr_cnt)
{
    if (impl_holder_) {
        THROW_INTERNAL_ERROR("io factory is already initialized");
    }
    impl_holder_ = make_shared<lin_impl>(shared_from_this(), thr_cnt);
    impl_ = impl_holder_;
}

std::string factory::name() const
{
    return std::string("linux");
}

// tcp_socket_factory
#if 0
tcp_socket factory::create_tcp_socket(tcp_socket_type dt)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        int err = errno;
        throw exception("can't create socket, error: %1%"_fmt % strerror(err));
    }

    int pinsockfd = sockfd;
    SCOPE_EXIT([&pinsockfd]() { if (pinsockfd >= 0) ::close(pinsockfd); });

    linapi::append_descriptor_flags(sockfd, O_NONBLOCK);
    lin_shared_handle * sh = dataptr->new_socket_handle(sockfd);

    pinsockfd = -1;
    return tcp_socket_access::create_tcp_socket<socket_traits>(shared_from_this(), sh);
}
#endif

#if 0
tcp_socket factory::create_connected_tcp_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt)
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
    if (int s = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &rp); s != 0 || !rp) {
        throw exception("tcp socket address '%1%' is not valid, error: %2%"_fmt % address % gai_strerror(s));
    }
    SCOPE_EXIT([rp] { freeaddrinfo(rp); });

    std::ostringstream errmsgs;
    for (; !!rp; rp = rp->ai_next) {
        try {
            int sockfd = linapi::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            int pinsockfd = sockfd;
            SCOPE_EXIT([&pinsockfd]() { if (pinsockfd >= 0) ::close(pinsockfd); });

            if constexpr (IO_DEBUG) LOG_TRACE(logger()) << "BEFORE CONNECT";

            linapi::connect(sockfd, rp->ai_addr, rp->ai_addrlen);

            if constexpr (IO_DEBUG) LOG_TRACE(logger()) << "CONNECTED!";

            linapi::append_descriptor_flags(sockfd, O_NONBLOCK);

            socket_handle * sh = dataptr->new_socket_handle(sockfd);

            pinsockfd = -1;

            return tcp_socket_access::create_tcp_socket<socket_traits>(shared_from_this(), dataptr->new_socket_handle(sh));

        } catch (std::exception const& e) {
            if (!errmsgs.str().empty()) errmsgs << '\n';
            errmsgs << e.what();
        }
    }
    throw exception("can't connect socket to '%1%:%2%', error(s): %3%"_fmt % address % port % errmsgs.str());
    THROW_NOT_IMPLEMENTED_ERROR();
}
#endif

//tcp_socket factory::create_bound_tcp_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt)
//{
//    THROW_NOT_IMPLEMENTED_ERROR("bind_tcp_socket");
//}

std::pair<tcp_socket, size_t> lin_impl::tcp_socket_accept(tcp_handle_type handle, char* buff, size_t sz)
{
    tcp_scoped_handle sh{this, handle};
    SCOPE_EXIT([&sh]() { --sh->waiting_cnt; });
    ++sh->waiting_cnt;
    unique_lock lck(sh->mtx);

    ::sockaddr in_addr;
    socklen_t in_len = sizeof(::in_addr);
    for (;;) {
        if (sh->fd() == -1) throw eof_exception("closed acceptor socket");
        int infd = accept(sh->fd(), &in_addr, &in_len);
        if (infd != -1) {
            sh->cnd.notify_one();
            lck.unlock();
            SCOPE_EXIT([&infd]() { if (infd != -1) ::close(infd); });
            if constexpr (IO_DEBUG) {
                char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
                getnameinfo(&in_addr, in_len,
                    hbuf, sizeof(hbuf),
                    sbuf, sizeof(sbuf),
                    NI_NUMERICHOST | NI_NUMERICSERV);

                LOG_TRACE(wrapper->logger()) << hbuf << " " << sbuf;
            }
            linapi::append_descriptor_flags(infd, O_NONBLOCK);
            tcp_socket rs = do_create_tcp_socket(infd);
            ssize_t n = ::read(infd, buff, sz);
            infd = -1;
            return {std::move(rs), n >= 0 ? n : 0};
        }

        int err = errno;
        if (err != EAGAIN) {
            throw exception("accept error: %1%"_fmt % strerror(err));
        }
        sh->wait(lck);
    }
}

// tcp_socket_service
#if 0
void factory::tcp_socket_bind(tcp_handle_type handle, cstring_view address, uint16_t port)
{
    tcp_scoped_handle sh{this, handle};

    int enable = 1;
    if (setsockopt(sh->fd(), SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        int err = errno;
        throw exception("can't set socket(SO_REUSEADDR) for '%1% : %2%', error: %3%"_fmt % address % port % strerror(err));
    }

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
    if (int s = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &rp); s != 0 || !rp) {
        throw exception("address '%1%' is not valid, error: %2%"_fmt % address % gai_strerror(s));
    }
    SCOPE_EXIT([rp] { freeaddrinfo(rp); });

    std::ostringstream errmsgs;
    for (; !!rp; rp = rp->ai_next) {
        if (!bind(sh->fd(), rp->ai_addr, rp->ai_addrlen)) return;
        int err = errno;
        if (!errmsgs.str().empty()) errmsgs << '\n';
        errmsgs << strerror(err);
    }

    throw exception("can't bind socket to '%1% : %2%', error(s): %3%"_fmt % address % port % errmsgs.str());
}

void factory::tcp_socket_listen(tcp_handle_type handle, function<void(tcp_connection_handler_type const&)> const& handler)
{
    tcp_scoped_handle sh{this, handle};

    if (int r = listen(sh->fd(), SOMAXCONN); r != 0) {
        int err = errno;
        throw exception("can't listen socket, error: %1%"_fmt % strerror(err));
    }

    epoll_ctl(dataptr->efd, sh->fd(), sh->bkid(), dataptr->acceptor_flags, [](const char * msg) {
        throw exception("can't watch acceptor, error: %1%"_fmt % msg);
    });
    //acceptor_socket_handle * sh = new acceptor_socket_handle(dataptr, sockfd, handler);
    //pinsockfd = -1;
    //return tcp_acceptor_access::create_tcp_acceptor(shared_from_this(), (void*)sh);

    BOOST_THROW_EXCEPTION(not_implemented_error("tcp_socket_listen"));
}

#endif

expected<size_t, std::error_code> lin_impl::tcp_socket_read_some(tcp_handle_type handle, void * buff, size_t sz)
{
    tcp_scoped_handle sh{this, handle};
    SCOPE_EXIT([&sh]() { --sh->waiting_cnt; });
    ++sh->waiting_cnt;
    unique_lock lck(sh->mtx);

    for (;;)
    {
        if (sh->fd() == -1) return 0;
        ssize_t n = ::read(sh->fd(), buff, sz);

        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) read %2% bytes"_fmt % sh->fd() % n);
        if (n >= 0) return (size_t)n;
        int err = errno;
        if (BOOST_UNLIKELY(EAGAIN != err)) { return 0; }
        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) read waiting..."_fmt % sh->fd());
        sh->wait(lck);
        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) woke up"_fmt % sh->fd());
    }
}

expected<size_t, std::error_code> lin_impl::tcp_socket_write_some(tcp_handle_type handle, void const* buff, size_t sz)
{
    tcp_scoped_handle sh{this, handle};
    SCOPE_EXIT([&sh]() { --sh->waiting_cnt; });
    ++sh->waiting_cnt;
    unique_lock lck(sh->mtx);

    for (;;)
    {
        if (sh->fd() == -1) return 0;
        ssize_t n = ::write(sh->fd(), buff, sz);
        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) write %2% bytes"_fmt % sh->fd() % n);
        if (n >= 0) return (size_t)n;
        int err = errno;
        if (BOOST_UNLIKELY(EAGAIN != err)) { return 0; }
        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) write waiting..."_fmt % sh->fd());
        sh->wait(lck);
        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) woke up"_fmt % sh->fd());
    }
}

void lin_impl::tcp_socket_close(tcp_handle_type h) noexcept
{
    lin_shared_handle * lh = static_cast<lin_shared_handle*>(h->lock());
    if (lh) {
        lock_guard guard(lh->mtx);
        if (lh->fd() != -1) {
            ::epoll_ctl(efd, EPOLL_CTL_DEL, lh->fd(), NULL);
            ::close(lh->fd());
            lh->handle = -1;
        }
        lh->cnd.notify_all();
        lh->release(static_cast<tcp_socket_service_type*>(this));
    }
}

void lin_impl::free_handle(identity<tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    delete_socket_handle(static_cast<lin_shared_handle*>(h));
}

#if 0
tcp_acceptor factory::create_tcp_acceptor(cstring_view address, uint16_t port, tcp_socket_type dt, function<void(tcp_acceptor_handler_type const&)> const& handler)
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
    if (int s = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &rp); s != 0 || !rp) {
        throw exception("tcp acceptor's address '%1%' is not valid, error: %2%"_fmt % address % gai_strerror(s));
    }
    SCOPE_EXIT([result] { freeaddrinfo(result); });

    std::ostringstream errmsgs;
    for (; !!rp; rp = rp->ai_next) {
        try {
            int sockfd = linapi::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            int pinsockfd = sockfd;
            SCOPE_EXIT([&pinsockfd]() { if (pinsockfd >= 0) { ::close(pinsockfd); }});
        
            int enable = 1;
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
                int err = errno;
                throw exception("can't set socket(SO_REUSEADDR), error: %1%"_fmt % strerror(err));
            }

            linapi::bind_socket(sockfd, rp->ai_addr, rp->ai_addrlen));
                
            linapi::append_descriptor_flags(sockfd, O_NONBLOCK);

            linapi::listen(sockfd, SOMAXCONN);

            acceptor_socket_handle * sh = dataptr->new_acceptor_handle(sockfd, handler);

            pinsockfd = -1;

            return tcp_acceptor_access::create_tcp_acceptor(shared_from_this(), sh);
        } catch (std::exception const& e) {
            if (!errmsgs.str().empty()) errmsgs << '\n';
            errmsgs << e.what();
        }
    }

    throw exception("can't bind socket to '%1%:%2%', error(s): %3%"_fmt % address % port % errmsgs.str());
}

void factory::tcp_acceptor_on_close(acceptor_handle_type handle) noexcept
{

}

#endif

size_t lin_impl::tcp_socket_waiting_count(tcp_handle_type handle)
{
    tcp_scoped_handle sh{this, handle};
    return sh->waiting_cnt.load();
}

udp_socket factory::create_udp_socket()
{
    THROW_NOT_IMPLEMENTED_ERROR("create_udp_socket");
}

void factory::udp_socket_bind(udp_handle_type handle, cstring_view address, uint16_t port)
{
    THROW_NOT_IMPLEMENTED_ERROR("udp_socket_bind");
}

void factory::udp_socket_listen(udp_handle_type handle, function<void(udp_connection_handler_type const&)> const& handler)
{
    THROW_NOT_IMPLEMENTED_ERROR("udp_socket_listen");
}

expected<size_t, std::error_code> factory::udp_socket_read_some(udp_handle_type handle, void * buff, size_t sz)
{
    THROW_NOT_IMPLEMENTED_ERROR("udp_socket_read_some");
}

expected<size_t, std::error_code> factory::udp_socket_write_some(udp_handle_type handle, socket_address const& address, void const* buff, size_t sz)
{
    THROW_NOT_IMPLEMENTED_ERROR("udp_socket_write_some");
}

expected<size_t, std::error_code> factory::udp_socket_write_some(udp_handle_type handle, cstring_view address, uint16_t port, void const* buff, size_t sz)
{
    THROW_NOT_IMPLEMENTED_ERROR("udp_socket_write_some");
}

void factory::udp_socket_on_close(udp_handle_type) noexcept
{

}

void factory::free_handle(identity<udp_socket_service_type>, udp_handle_type h) noexcept
{
    dataptr->delete_socket_handle(static_cast<lin_shared_handle*>(h));
}

file factory::open_file(cstring_view path, file_open_mode fom, file_access_mode fam, file_bufferring_mode fbm)
{
    int flags = O_LARGEFILE | O_NOATIME;
    int fmode = 0;
    switch (fom) {
    case file_open_mode::create:
        flags |= (O_CREAT | O_EXCL);
        break;
    case file_open_mode::create_or_open:
        flags |= O_CREAT;
        break;
    case file_open_mode::open:
        break;
    default:
        throw internal_error("unknown file open mode: %1%, file: %2%"_fmt % (int)fom % path);
    }

    switch (fam) {
    case file_access_mode::write:
        flags |= O_RDWR;
        fmode |= S_IRUSR | S_IWUSR;
        break;
    case file_access_mode::read:
        flags |= O_RDONLY;
        fmode |= S_IRUSR;
        break;
    default:
        throw internal_error("unknown file access mode: %1%, file: %2%"_fmt % (int)fam % path);
    }

    switch (fbm) {
    case file_bufferring_mode::buffered:
        break;
    case file_bufferring_mode::not_buffered:
        flags |= O_SYNC | O_DIRECT;
        break;
    default:
        throw internal_error("unknown file buffering mode: %1%, file: %2%"_fmt % (int)fbm % path);
    }

    int h = ::open(path.c_str(), flags, fmode);
    if (-1 == h) {
        int err = errno;
        throw exception("can't open file %1%, error: %2%"_fmt % path % strerror(err));
    }

    if (file_access_mode::write == fam) {
        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;
        /* Place a write lock on the file. */
        if (-1 == fcntl(h, F_SETLK, &lock)) {
            int err = errno;
            ::close(h);
            throw exception("can't write lock file %1%, error: %2%"_fmt % path % strerror(err));
        }
    }

    return file_access::open_file(shared_from_this(), h, path);
}

struct file_callback
{
    aiocb cb;
    sonia::posix::user_handler_type h_;

    optional<std::error_code> code;

    fibers::mutex mtx;
    fibers::condition_variable_any cnd;

    template <class BuffT>
    explicit file_callback(uint64_t fileoffset, BuffT buff)
    {
        memset(&cb, 0, sizeof(aiocb));
        cb.aio_offset = fileoffset;
        cb.aio_buf = (void*)buff.begin();
        cb.aio_nbytes = buff.size();
        cb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
        cb.aio_sigevent.sigev_signo = sonia::posix::get_user_signal();
        cb.aio_sigevent.sigev_value.sival_ptr = &h_;
        h_ = [this] { on_op(); };
    }

    void on_op()
    {
        unique_lock lck(mtx);
        int err = aio_error(&cb) ? errno : 0;
        code.emplace(err, std::system_category());
        cnd.notify_one();
    }

    void wait()
    {
        unique_lock lck(mtx);
        cnd.wait(lck, [this] { return !!code; });
        if (code && *code) {
            throw exception(code->message());
        }
    }
};

size_t factory::file_read(int handle, uint64_t fileoffset, array_view<char> dest)
{
    file_callback fcb(fileoffset, dest);
    fcb.cb.aio_fildes = handle;
    
    int r = aio_read(&fcb.cb);
    if (-1 == r) {
        int err = errno;
        throw exception("read file error : %1%"_fmt % strerror(err));
    }
    fcb.wait();
    ssize_t rsz = aio_return(&fcb.cb);
    if (rsz < 0) {
        int err = errno;
        throw exception("read file error : %1%"_fmt % strerror(err));
    }

    return (size_t)rsz;
}

size_t factory::file_write(int handle, uint64_t fileoffset, array_view<const char> src)
{
    file_callback fcb(fileoffset, src);
    fcb.cb.aio_fildes = handle;

    int r = aio_write(&fcb.cb);
    if (-1 == r) {
        int err = errno;
        throw exception("write file error : %1%"_fmt % strerror(err));
    }
    fcb.wait();
    ssize_t rsz = aio_return(&fcb.cb);
    if (rsz < 0) {
        int err = errno;
        throw exception("write file error : %1%"_fmt % strerror(err));
    }

    return (size_t)rsz;
}

}
