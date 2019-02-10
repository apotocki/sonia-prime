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

#include "sonia/utility/posix/signals.hpp"

#include "sonia/concurrency.hpp"
#include "sonia/optional.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/object_pool.hpp"
#include "sonia/utility/linux.hpp"
//#include "sonia/utility/concurrency/rw_fiber_mutex.hpp"

#include <boost/unordered_map.hpp>

#ifndef MAX_EPOLL_EVENTS
#   define MAX_EPOLL_EVENTS 5
#endif

#ifndef SOCKET_POOL_SIZE
#   define SOCKET_POOL_SIZE 128
#endif

namespace sonia { namespace io {

struct socket_handle_base;
struct lin_impl_data;
struct socket_handle;
struct acceptor_socket_handle;

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
void epoll_ctl(int efd, int fd, uint32_t evflags, OnErrorHandlerT const& errh)
{
    epoll_event ev;
    ev.data.fd = fd;
    ev.events = evflags;
    if (-1 == ::epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev)) {
        int err = errno;
        errh(strerror(err));
    }
}

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
            GLOBAL_LOG_ERROR() << "errror while closing acceptor socket: " << strerror(err);
        }
    }
};

struct socket_handle_base
{
    unique_fd sock;
    std::atomic<long> refs_{1};

    explicit socket_handle_base(int sockfd = -1) : sock(sockfd) {}

    socket_handle_base(socket_handle_base const&) = delete;
    socket_handle_base(socket_handle_base &&) = delete;
    socket_handle_base& operator= (socket_handle_base const&) = delete;
    socket_handle_base& operator= (socket_handle_base &&) = delete;

    virtual ~socket_handle_base() noexcept {}

    void close() noexcept { sock.close(); }

    virtual void on_event(int efd, uint32_t evflags) noexcept = 0;
    virtual void free(lin_impl_data*) noexcept = 0;

    void add_ref() { ++refs_; }
    long release() { return refs_.fetch_sub(1) - 1; }
};

//struct pipe_handle : socket_handle_base
//{
//    using socket_handle_base::socket_handle_base;
//
//    bool on_event(int efd, uint32_t evflags) noexcept override
//    {
//        return false;
//    }
//};

struct lin_impl_data : public factory::impl_base
{
    friend class factory;

    int ctl_pipe[2];

    int efd;

    int acceptor_flags;

    sonia::spin_mutex bk_mtx_;
    boost::unordered_map<int, socket_handle_base*> book_keeper_;
    object_pool<socket_handle, spin_mutex> soc_pool_;

    lin_impl_data(shared_ptr<factory> itself, uint32_t thr_cnt);

    socket_handle * new_socket_handle(int sockfd);

    void park_threads() noexcept override
    {
        LOG_TRACE(wrapper->logger()) << "parking threads...";
        char ch = 'e';
        if (int r = ::write(ctl_pipe[1], &ch, 1); 1 == r) {
            thread([wr = wrapper]() {
                wr->join_threads();
                LOG_TRACE(wr->logger()) << "threads have been parked";
            }).detach();
        } else {
            int err = errno;
            LOG_ERROR(wrapper->logger()) << "can't park threads, error: "_fmt << strerror(err);
        }
    }

    ~lin_impl_data() override
    {
        close(ctl_pipe[0]);
        close(ctl_pipe[1]);
        ::close(efd);
    }

    void add_to_bk(int fd, socket_handle_base * sh)
    {
        lock_guard guard(bk_mtx_);
        if (BOOST_UNLIKELY(!book_keeper_.insert(std::pair(fd, sh)).second)) {
            throw internal_error("bk error: an attempt to add already monitored descriptor %1%"_fmt % fd);
        }
    }

    void remove_from_bk(int fd) noexcept
    {
        ::epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL);

        size_t sz;
        {
            lock_guard guard(bk_mtx_);
            sz = book_keeper_.erase(fd);
        }

        if (BOOST_UNLIKELY(0 == sz)) {
            LOG_WARN(wrapper->logger()) << "can't fine the descriptor " << fd << " in BK";
        }
    }

    void free(socket_handle * sh) noexcept;
    void free(acceptor_socket_handle * sh) noexcept;
};

struct socket_handle : socket_handle_base
{
    fibers::mutex mtx;
    fibers::condition_variable_any cnd;
    bool flag{false};

    socket_handle(lin_impl_data * impl, int sockfd)
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

    void wait()
    {
        unique_lock lck(mtx);
        cnd.wait(lck, [this] { return flag; });
    }

    void on_event(int efd, uint32_t evflags) noexcept override
    {
        //GLOBAL_LOG_INFO() << "SOCKET EVENT! " << ((evflags & EPOLLIN) ? "IN" : "") << " " << ((evflags & EPOLLOUT) ? "OUT" : "");
        unique_lock lck(mtx);
        flag = true;
        cnd.notify_one();
    }

    void free(lin_impl_data* impl) noexcept override { impl->free(this); }
};

struct acceptor_socket_handle : socket_handle_base
{
    lin_impl_data * impl_;
    function<void(tcp_acceptor_factory::connection_factory_t const&)> handle_;

    template <typename HandleT>
    acceptor_socket_handle(lin_impl_data * impl, int sockfd, HandleT const& h)
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

    //std::atomic<int> enters{0};

    void on_event(int efd, uint32_t evflags) noexcept override
    {
        //int ent = enters.fetch_add(1);
        //GLOBAL_LOG_INFO() << "ACCEPTOR EVENT! " << ent;
        for (;;) {
            sockaddr in_addr;
            socklen_t in_len = sizeof(in_addr);
            int infd = accept(sock.value, &in_addr, &in_len);
            if (infd == -1) {
                //int err = errno;
                //LOG_TRACE(impl_->wrapper->logger()) << "accept error: " << strerror(err);
                break;
            }

            unique_fd ufd{infd};

            /*
            char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
            getnameinfo(&in_addr, in_len,
                hbuf, sizeof(hbuf),
                sbuf, sizeof(sbuf),
                NI_NUMERICHOST | NI_NUMERICSERV);

            GLOBAL_LOG_TRACE() << hbuf << " " << sbuf << "(" << ent << ")";
            */

            set_descriptor_flags(infd, O_NONBLOCK);

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

    void free(lin_impl_data*) noexcept override { impl_->free(this); }
};


lin_impl_data::lin_impl_data(shared_ptr<factory> itself, uint32_t thr_cnt)
    : factory::impl_base(std::move(itself))
    , soc_pool_(SOCKET_POOL_SIZE)
{
    if (efd = epoll_create1(0); efd == -1) {
        int err = errno;
        throw exception("can't create epoll instance, error: %1%"_fmt % strerror(err));
    }

    if (int r = ::pipe(ctl_pipe); r != 0) {
        int err = errno;
        throw exception("can't create a contorl pipe instance, error: %1%"_fmt % strerror(err));
    }

    set_descriptor_flags(ctl_pipe[0], O_NONBLOCK); // read

    epoll_ctl(efd, ctl_pipe[0], EPOLLIN, [](const char* msg) {
        throw exception("can't start watch the controll pipe, error: %1%"_fmt % msg);
    });

    auto[a, b, c] = sonia::linux::kernel_version();
    LOG_INFO(wrapper->logger()) << "KERNEL: " << a << "." << b << "." << c;

    acceptor_flags = EPOLLIN | EPOLLET;
    if (a >= 4 && b >= 5) {
        acceptor_flags |= EPOLLEXCLUSIVE;
        LOG_INFO(wrapper->logger()) << "USING EPOLLEXCLUSIVE";
    }
}

socket_handle * lin_impl_data::new_socket_handle(int sockfd)
{
    return soc_pool_.new_object(this, sockfd);
}

void lin_impl_data::free(socket_handle * sh) noexcept
{
    soc_pool_.delete_object(sh);
    wrapper->on_release_callback();
}

void lin_impl_data::free(acceptor_socket_handle * sh) noexcept
{
    delete sh;
    wrapper->on_release_callback();
}

#define dataptr static_cast<lin_impl_data*>(impl_data_.get())



factory::factory() 
    : impl_data_(nullptr)
{
    
}

factory::~factory()
{

}

void factory::initialize_impl(uint32_t thr_cnt)
{
    if (impl_data_) {
        throw internal_error("io factory is already initialized");
    }

    impl_data_.reset(new lin_impl_data(shared_from_this(), thr_cnt));
}

std::string factory::name() const
{
    return std::string("linux");
}

void factory::thread_proc()
{
    epoll_event events[MAX_EPOLL_EVENTS];
    for (;;) {
        //LOG_INFO(logger()) << "before epoll_wait";
        int n = epoll_wait(dataptr->efd, events, MAX_EPOLL_EVENTS, -1);
        //LOG_INFO(logger()) << "woke up";
        if (-1 == n) {
            int err = errno;
            LOG_ERROR(logger()) << "epoll interrupted: " << strerror(err);
            return;
        }
        if (n == 0) {
            LOG_ERROR(logger()) << "epoll WRONG WOKE UP";
        }
        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;
            if (fd == dataptr->ctl_pipe[0]) return; // close event

            socket_handle_base* h;
            {
                lock_guard guard(dataptr->bk_mtx_);
                auto it = dataptr->book_keeper_.find(fd);
                if (it == dataptr->book_keeper_.end()) continue; // skip obsoleted descriptor event
                h = it->second;
                h->add_ref();
            }

            h->on_event(dataptr->efd, events[i].events); // noexcept
            if (!h->release()) {
                h->free(dataptr);
            }
        }
    }
}


// tcp_socket_factory
tcp_socket factory::create_tcp_socket(cstring_view address, uint16_t port, tcp_socket_type dt)
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
    if (int s = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &result); s != 0 || !result) {
        throw exception("tcp socket address '%1%' is not valid, error: %2%"_fmt % address % gai_strerror(s));
    }
    SCOPE_EXIT([result] { freeaddrinfo(result); });

    addrinfo * rp;
    for (rp = result; !!rp; rp = rp->ai_next) {
        int sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1) {
            int err = errno;
            LOG_ERROR(logger()) << "can't create socket, error: " << strerror(err);
            continue;
        }

        int pinsockfd = sockfd;
        SCOPE_EXIT([&pinsockfd]() { if (pinsockfd >= 0) ::close(pinsockfd); });

        //LOG_INFO(logger()) << "BEFORE CONNECT";
        /*
        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            set_descriptor_flags(sockfd, O_NONBLOCK);
            socket_handle * sh = dataptr->new_socket_handle(sockfd);
            
            pinsockfd = -1;
            LOG_INFO(logger()) << "CONNECTED!";
            return tcp_socket_access::create_tcp_socket(shared_from_this(), (intptr_t)(socket_handle_base*)sh);
        }
        //*/
        ///*
        set_descriptor_flags(sockfd, O_NONBLOCK);
        socket_handle * sh = dataptr->new_socket_handle(sockfd);
        for (;;) {
            if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1) {
                pinsockfd = -1;
                //LOG_INFO(logger()) << "CONNECTED";
                return tcp_socket_access::create_tcp_socket(shared_from_this(), (intptr_t)(socket_handle_base*)sh);
            }
            int err = errno;
            if (BOOST_UNLIKELY(EINPROGRESS != err)) {
                LOG_WARN(logger()) << "can't connect socket, error: " << strerror(err);
                break;
            }
            sh->wait();
        }
        
        tcp_socket_close((intptr_t)(socket_handle_base*)sh);
        //*/
    }

    BOOST_ASSERT(!rp);
    throw exception("can't connect socket to '%1% : %2%'"_fmt % address % port);
}

size_t factory::tcp_socket_count(tcp_socket_type) const
{
    throw not_implemented_error("tcp_socket_count");
}

// tcp_socket_service
void factory::tcp_socket_close(intptr_t handle)
{
    socket_handle_base * ah = reinterpret_cast<socket_handle_base*>(handle);
    dataptr->remove_from_bk(ah->sock.value);
    if (!ah->release()) {
        ah->free(dataptr);
    }
}

size_t factory::tcp_socket_read_some(intptr_t handle, void * buff, size_t sz)
{
    socket_handle * sh = static_cast<socket_handle*>(reinterpret_cast<socket_handle_base*>(handle));

    for (;;) {
        ssize_t n = ::read(sh->sock.value, buff, sz);
        if (n >= 0) return (size_t)n;
        int err = errno;
        if (BOOST_UNLIKELY(EAGAIN != err)) {
            throw exception("can't read from socket, error: %1%"_fmt % strerror(err));
        }
        sh->wait();
    }
}

void factory::tcp_socket_async_read_some(intptr_t soc, void * buff, size_t sz, function<void(std::error_code const&, size_t)> const& ftor)
{
    throw not_implemented_error("tcp_socket_async_read_some");
}

size_t factory::tcp_socket_write_some(intptr_t handle, void const* buff, size_t sz)
{
    socket_handle * sh = static_cast<socket_handle*>(reinterpret_cast<socket_handle_base*>(handle));

    for (;;) {
        ssize_t n = ::write(sh->sock.value, buff, sz);
        if (n >= 0) return (size_t)n;
        int err = errno;
        if (BOOST_UNLIKELY(EAGAIN != err)) {
            throw exception("can't write to socket, error: %1%"_fmt % strerror(err));
        }
        sh->wait();
    }
}

tcp_acceptor factory::create_tcp_acceptor(cstring_view address, uint16_t port, tcp_socket_type dt, function<void(tcp_acceptor_factory::connection_factory_t const&)> const& handler)
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
    if (int s = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &result); s != 0 || !result) {
        throw exception("tcp acceptor's address '%1%' is not valid, error: %2%"_fmt % address % gai_strerror(s));
    }
    SCOPE_EXIT([result] { freeaddrinfo(result); });

    addrinfo * rp;
    for (rp = result; !!rp; rp = rp->ai_next) {
        // LOG_INFO(logger()) << "family: " << rp->ai_family << ", socktype: " << rp->ai_socktype << ", protocol: " << rp->ai_protocol;
        int sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1) continue;

        int pinsockfd = sockfd;
        SCOPE_EXIT([this, &pinsockfd]() { if (pinsockfd >= 0) { ::close(pinsockfd); dataptr->remove_from_bk(pinsockfd); }});
        
        int enable = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
            int err = errno;
            throw exception("can't set socket(SO_REUSEADDR) for '%1% : %2%', error: %3%"_fmt % address % port % strerror(err));
        }

        if (!bind(sockfd, rp->ai_addr, rp->ai_addrlen)) {
            set_descriptor_flags(sockfd, O_NONBLOCK);
            if (int r = listen(sockfd, SOMAXCONN); r != 0) {
                int err = errno;
                throw exception("can't listen socket '%1% : %2%', error: %3%"_fmt % address % port % strerror(err));
            }

            acceptor_socket_handle * sh = new acceptor_socket_handle(dataptr, sockfd, handler);

            pinsockfd = -1;
            return tcp_acceptor_access::create_tcp_acceptor(shared_from_this(), (void*)sh);
        }
    }
    BOOST_ASSERT(!rp);
    throw exception("can't bind socket to '%1% : %2%'"_fmt % address % port);
}

void factory::tcp_acceptor_close(void * handle) noexcept
{
    socket_handle_base * ah = reinterpret_cast<socket_handle_base*>(handle);
    dataptr->remove_from_bk(ah->sock.value);
    if (!ah->release()) {
        ah->free(dataptr);
    }
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

}}
