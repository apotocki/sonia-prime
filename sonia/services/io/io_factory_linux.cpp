//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "factory.hpp"

#ifndef __ANDROID__
#   include <aio.h>
#endif

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>

#include <sys/epoll.h>

#include <atomic>
#include <sstream>

#include <boost/unordered_map.hpp>
#include <boost/intrusive/list.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/optional.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/object_pool.hpp"
#include "sonia/sys/posix/posix.hpp"
#include "sonia/sys/linux/signals.hpp"

#ifndef MAX_EPOLL_EVENTS
#   define MAX_EPOLL_EVENTS 5
#endif

#ifndef SOCKET_POOL_SIZE
#   define SOCKET_POOL_SIZE 128
#endif

#ifndef CALLBACK_POOL_SIZE
#   define CALLBACK_POOL_SIZE 128
#endif

namespace sonia::io {

static constexpr bool IO_DEBUG = false;
static constexpr uint64_t epool_exit_cookie_v = (std::numeric_limits<uint64_t>::max)();

struct lin_impl;
struct lin_shared_handle;

#if 0
struct unique_fd
{
    explicit unique_fd(int fd = -1) noexcept : value{ fd } {}
    unique_fd(unique_fd const&) = delete;
    unique_fd(unique_fd&& rhs) noexcept : value{ rhs.value } { rhs.value = -1; }
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

struct callback : boost::intrusive::list_base_hook<>
{
    virtual ~callback() = default;

    virtual bool operator()(lin_shared_handle & sh, bool eof) noexcept = 0; // returns true if completed and needs to be removed; if eof is true, must report about eof and reutrn true

    virtual void free(lin_shared_handle & sh) = 0;
};

struct acceptor_callback : callback
{
    fibers::promise<tcp_socket> promise_;
    bool operator()(lin_shared_handle & sh, bool eof) noexcept override;

    fibers::future<tcp_socket> get_future() { return promise_.get_future(); }

    void free(lin_shared_handle & sh) override;
};

struct lin_shared_handle : shared_handle<socket_handle_traits>
{
    using base_type = shared_handle<socket_handle_traits>;

    using callback_t = automatic_polymorphic<callback, sizeof(acceptor_callback)>;
    using callback_list_t = boost::intrusive::list<callback>;

    shared_ptr<lin_impl> impl;
    fibers::mutex mtx;
    fibers::condition_variable_any cnd;
    sonia::sal::socket_handle handle;

    lin_shared_handle(shared_ptr<lin_impl> p, sonia::sal::socket_handle h, uint32_t id, sonia::sal::net_family_type ftval)
        : impl(std::move(p)), handle(h), family_(ftval), bkid_(id)
    {}

    ~lin_shared_handle()
    {
        handle_callbacks(true);
        BOOST_ASSERT (callback_list_.empty());
    }

    void handle_callbacks(bool eof = false);

    uint32_t bkid() const { return bkid_; }
    sonia::sal::net_family_type family() { return family_; }

    virtual void on_event() noexcept
    {
        {
            unique_lock lck(mtx);
            handle_callbacks(false);
        }
        cnd.notify_all();
    };

    void add_callback(callback * cb)
    {
        callback_list_.push_back(*cb);
    }

    tcp_socket create_tcp_socket(int fd);

    std::atomic<size_t> waiting_cnt{0};

    void wait(unique_lock<fibers::mutex> & lck)
    {
        cnd.wait(lck);
    }

    bool close(int efd); // return true if book_keeper needs updating

private:
    uint32_t bkid_;
    sonia::sal::net_family_type family_;
    callback_list_t callback_list_;
};

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

template <typename OnErrorHandlerT>
void epoll_ctl_add(int efd, lin_shared_handle& lsh, OnErrorHandlerT const& errh)
{
    epoll_event ev;
    ev.data.u64 = lsh.bkid();
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    if (-1 == ::epoll_ctl(efd, EPOLL_CTL_ADD, lsh.handle, &ev)) {
        int err = errno;
        errh(strerror(err));
    }
}

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
    object_pool<lin_shared_handle::callback_t, spin_mutex> callbacks_;

    explicit lin_impl(shared_ptr<factory> itself);

    lin_shared_handle * new_socket_handle(int sockfd, sonia::sal::net_family_type);
    void delete_socket_handle(lin_shared_handle*) noexcept;

    template <typename ... ArgsT>
    lin_shared_handle::callback_t * new_callback(ArgsT&& ... args);
    void delete_callback(lin_shared_handle::callback_t *) noexcept;

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
        LOG_TRACE(wrapper->logger()) << "descruction of lin_impl";
        ::close(ctl_pipe[0]);
        ::close(ctl_pipe[1]);
        ::close(efd);
    }

    void thread_proc() override;
    
    lin_shared_handle* do_create_socket(sonia::sal::socket_handle, sonia::sal::net_family_type);

    tcp_server_socket do_create_tcp_server_socket(sonia::sal::socket_handle, sonia::sal::net_family_type) override final;
    tcp_socket do_create_tcp_socket(sonia::sal::socket_handle, sonia::sal::net_family_type) override;
    udp_socket do_create_udp_socket(sonia::sal::socket_handle, sonia::sal::net_family_type) override;

    // tcp server socket service
    fibers::future<tcp_socket> tcp_server_socket_accept(tcp_handle_type) override final;
    size_t tcp_server_socket_accepting_count(tcp_handle_type) const override final;
    void close_handle(identity<tcp_server_socket_service_type>, tcp_handle_type) noexcept override final;
    void release_handle(identity<tcp_server_socket_service>, tcp_handle_type) noexcept override final;
    void free_handle(identity<tcp_server_socket_service_type>, tcp_handle_type) noexcept override final;
    
    // tcp socket service
    expected<size_t, std::exception_ptr> tcp_socket_read_some(tcp_handle_type, void * buff, size_t sz) noexcept override final;
    expected<size_t, std::exception_ptr> tcp_socket_write_some(tcp_handle_type, void const* buff, size_t sz) noexcept override final;
    void close_handle(identity<tcp_socket_service_type>, tcp_handle_type) noexcept override final;
    void release_handle(identity<tcp_socket_service>, tcp_handle_type) noexcept override final;
    void free_handle(identity<tcp_socket_service_type>, tcp_handle_type) noexcept override final;
    sal::socket_handle system_handle(tcp_handle_type) noexcept override final;

    // udp socket service
    void udp_socket_bind(udp_handle_type, cstring_view address, uint16_t port) override final;
    size_t udp_socket_waiting_count(udp_handle_type) override final;
    expected<size_t, std::exception_ptr> udp_socket_read_some(udp_handle_type, void * buff, size_t sz, sonia::sal::socket_address* addr) override final;
    expected<size_t, std::exception_ptr> udp_socket_write_some(udp_handle_type, sonia::sal::socket_address const&, void const* buff, size_t sz) override final;
    expected<size_t, std::exception_ptr> udp_socket_write_some(udp_handle_type handle, cstring_view address, uint16_t port, void const* buff, size_t sz) override final;
    void close_handle(identity<udp_socket_service_type>, udp_handle_type) noexcept override final;
    void release_handle(identity<udp_socket_service_type>, tcp_handle_type) noexcept override final;
    void free_handle(identity<udp_socket_service_type>, udp_handle_type) noexcept override final;
};

tcp_socket lin_shared_handle::create_tcp_socket(int fd)
{
    return impl->do_create_tcp_socket(fd, family());
}

void lin_shared_handle::handle_callbacks(bool eof)
{
    for (auto it = callback_list_.begin(), eit = callback_list_.end(); it != eit;) {
        callback & cb = *it;
        if (cb(*this, eof)) {
            it = callback_list_.erase(it);
            cb.free(*this);
        } else {
            BOOST_ASSERT (!eof);
            ++it;
        }
    }
}

bool lin_shared_handle::close(int efd)
{
    if (lock_guard guard(mtx); handle != -1) {
        ::epoll_ctl(efd, EPOLL_CTL_DEL, handle, NULL);
        ::close(handle);
        handle = -1;
        handle_callbacks(true);
        return true;
    }
    return false;
}

bool acceptor_callback::operator()(lin_shared_handle & sh, bool eof) noexcept
{
    if (eof) {
        --sh.waiting_cnt;
        promise_.set_exception(std::make_exception_ptr(eof_exception()));
        return true;
    }

    ::sockaddr in_addr;
    socklen_t in_len = sizeof(::in_addr);
    int infd = accept(sh.handle, &in_addr, &in_len);
    if (infd == -1) {
        int err = errno;
        if (err == EAGAIN) return false;
        --sh.waiting_cnt;
        promise_.set_exception(std::make_exception_ptr(eof_exception("accept error: %1%"_fmt % strerror(err))));
        return true;
    }

    --sh.waiting_cnt;

    if constexpr (IO_DEBUG) {
        char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
        getnameinfo(&in_addr, in_len,
            hbuf, sizeof(hbuf),
            sbuf, sizeof(sbuf),
            NI_NUMERICHOST | NI_NUMERICSERV);

        LOG_TRACE(sh.impl->wrapper->logger()) << hbuf << " " << sbuf;
    }

    posix::append_descriptor_flags(infd, O_NONBLOCK);
    promise_.set_value(sh.create_tcp_socket(infd));

    return true;
}

void acceptor_callback::free(lin_shared_handle & sh)
{
    sh.impl->delete_callback(reinterpret_cast<lin_shared_handle::callback_t*>(this));
}
          
lin_impl::lin_impl(shared_ptr<factory> itself)
    : factory::impl_base(std::move(itself))
    , sockets_(SOCKET_POOL_SIZE)
    , callbacks_(CALLBACK_POOL_SIZE)
{
    if (efd = epoll_create1(0); efd == -1) {
        int err = errno;
        throw exception("can't create epoll instance, error: %1%"_fmt % strerror(err));
    }

    if (int r = ::pipe(ctl_pipe); r != 0) {
        int err = errno;
        throw exception("can't create a contorl pipe instance, error: %1%"_fmt % strerror(err));
    }

    posix::append_descriptor_flags(ctl_pipe[0], O_NONBLOCK); // read

    epoll_event ev;
    ev.data.u64 = epool_exit_cookie_v;
    ev.events = EPOLLIN;
    if (-1 == ::epoll_ctl(efd, EPOLL_CTL_ADD, ctl_pipe[0], &ev)) {
        int err = errno;
        throw exception("can't start watching the controll pipe, error: %1%"_fmt % strerror(err));
    }

    auto[a, b, c] = posix::kernel_version();
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

lin_shared_handle * lin_impl::new_socket_handle(int sockfd, sonia::sal::net_family_type ft)
{
    for (;;) {
        lin_shared_handle *ph = sockets_.new_object(shared_from_this(), sockfd, bkid_cnt_.fetch_add(1), ft);
        lock_guard guard(bk_mtx_);
        if (BOOST_LIKELY(book_keeper_.insert(ph).second)) {
            ph->add_weakref();
            return ph;
        }
        sockets_.delete_object(ph);
    }
}

void lin_impl::delete_socket_handle(lin_shared_handle * h) noexcept
{
    sockets_.delete_object(h);
}

template <typename ... ArgsT>
lin_shared_handle::callback_t * lin_impl::new_callback(ArgsT&& ... args)
{
    return callbacks_.new_object(std::forward<ArgsT>(args) ...);
}

void lin_impl::delete_callback(lin_shared_handle::callback_t * cb) noexcept
{
    callbacks_.delete_object(cb);
}

lin_shared_handle * lin_impl::do_create_socket(sonia::sal::socket_handle s, sonia::sal::net_family_type dt)
{
    posix::append_descriptor_flags(s, O_NONBLOCK);
    lin_shared_handle* sh = new_socket_handle(s, dt);
    epoll_ctl_add(efd, *sh, [this, sh](const char* msg) {
        if (sh->close(efd)) {
            lock_guard guard(bk_mtx_);
            BOOST_VERIFY(1 == book_keeper_.erase(sh));
        }
        sh->release_weak((tcp_socket_service_type*)this);
        //delete_socket_handle(sh);
        throw exception("can't watch socket, error: %1%"_fmt % msg);
    });
    return sh;
}

tcp_server_socket lin_impl::do_create_tcp_server_socket(sonia::sal::socket_handle s, sonia::sal::net_family_type dt)
{
    lin_shared_handle* sh = do_create_socket(s, dt);
    return tcp_socket_access::create_tcp_server_socket<socket_traits>(shared_from_this(), sh);
}

tcp_socket lin_impl::do_create_tcp_socket(sonia::sal::socket_handle s, sonia::sal::net_family_type dt)
{
    lin_shared_handle* sh = do_create_socket(s, dt);
    return tcp_socket_access::create_tcp_socket<socket_traits>(shared_from_this(), sh);
}

udp_socket lin_impl::do_create_udp_socket(sonia::sal::socket_handle s, sonia::sal::net_family_type dt)
{
    lin_shared_handle* sh = do_create_socket(s, dt);
    return udp_socket_access::create_udp_socket<socket_traits>(shared_from_this(), sh);
}

factory::factory() {}

void factory::initialize_impl()
{
    if (impl_holder_) {
        THROW_INTERNAL_ERROR("io factory is already initialized");
    }
    impl_holder_ = make_shared<lin_impl>(shared_from_this());
    impl_ = impl_holder_;
}

std::string factory::name() const
{
    return std::string("linux");
}

fibers::future<tcp_socket> lin_impl::tcp_server_socket_accept(tcp_handle_type handle)
{
    auto * sh = static_cast<lin_shared_handle*>(handle);
    ++sh->waiting_cnt;

    auto * pcb = new_callback(in_place_type<acceptor_callback>);
    SCOPE_EXIT([this, &pcb]() { if (pcb) delete_callback(pcb); });

    auto ftr = static_cast<acceptor_callback*>(pcb->get_pointer())->get_future();

    unique_lock lck(sh->mtx);
    
    if (!(**pcb)(*sh, false)) {
        
        sh->add_callback(pcb->get_pointer());
        pcb = nullptr; // unpin
    } // else waiting_cnt has been already decremented
    
    return std::move(ftr);
}

size_t lin_impl::tcp_server_socket_accepting_count(tcp_handle_type handle) const
{
    auto * sh = static_cast<lin_shared_handle*>(handle);
    return sh->waiting_cnt.load();
}

void lin_impl::close_handle(identity<tcp_server_socket_service_type>, tcp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void lin_impl::release_handle(identity<tcp_server_socket_service>, tcp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void lin_impl::free_handle(identity<tcp_server_socket_service>, tcp_handle_type h) noexcept
{
    delete_socket_handle(static_cast<lin_shared_handle*>(h));
}

expected<size_t, std::exception_ptr> lin_impl::tcp_socket_read_some(tcp_handle_type handle, void * buff, size_t sz) noexcept
{
    auto * sh = static_cast<lin_shared_handle*>(handle);
    SCOPE_EXIT([&sh]() { --sh->waiting_cnt; });
    ++sh->waiting_cnt;
    unique_lock lck(sh->mtx);

    for (;;)
    {
        if (sh->handle == -1) return 0;
        ssize_t n = ::read(sh->handle, buff, sz);

        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) read %2% bytes"_fmt % sh->handle % n);
        if (n >= 0) return (size_t)n;
        int err = errno;
        if (BOOST_UNLIKELY(EAGAIN != err)) return make_unexpected(std::make_exception_ptr(exception(strerror(err))));
        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) read waiting..."_fmt % sh->handle);
        //++wrapper->pending_reads_;
        sh->wait(lck);
        //--wrapper->pending_reads_;
        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) woke up"_fmt % sh->handle);
    }
}

expected<size_t, std::exception_ptr> lin_impl::tcp_socket_write_some(tcp_handle_type handle, void const* buff, size_t sz) noexcept
{
    auto * sh = static_cast<lin_shared_handle*>(handle);
    SCOPE_EXIT([&sh]() { --sh->waiting_cnt; });
    ++sh->waiting_cnt;
    unique_lock lck(sh->mtx);

    for (;;)
    {
        if (sh->handle == -1) return 0;
        ssize_t n = ::write(sh->handle, buff, sz);
        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) write %2% bytes"_fmt % sh->handle % n);
        if (n >= 0) return (size_t)n;
        int err = errno;
        if (BOOST_UNLIKELY(EAGAIN != err)) return make_unexpected(std::make_exception_ptr(exception(strerror(err))));
        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) write waiting..."_fmt % sh->handle);
        //++wrapper->pending_writes_;
        sh->wait(lck);
        //--wrapper->pending_writes_;
        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) woke up"_fmt % sh->handle);
    }
}

void lin_impl::close_handle(identity<tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    auto * sh = static_cast<lin_shared_handle*>(h);
    if (sh) {
        int soc = sh->handle;
        bool erased = sh->close(efd);
        if (erased) {
            lock_guard guard(bk_mtx_);
            BOOST_VERIFY(1 == book_keeper_.erase(sh));
            sh->release_weak((tcp_socket_service_type*)this);
        }
        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) closed, book_keeper erased: (%2%) ..."_fmt % soc % erased);
        sh->cnd.notify_all();
    }
}

void lin_impl::release_handle(identity<tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void lin_impl::free_handle(identity<tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    delete_socket_handle(static_cast<lin_shared_handle*>(h));
}

sal::socket_handle lin_impl::system_handle(tcp_handle_type h) noexcept
{
    auto* sh = static_cast<lin_shared_handle*>(h);
    return sh ? sh->handle : -1;
}

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
            int sockfd = posix::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            int pinsockfd = sockfd;
            SCOPE_EXIT([&pinsockfd]() { if (pinsockfd >= 0) { ::close(pinsockfd); }});
        
            int enable = 1;
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
                int err = errno;
                throw exception("can't set socket(SO_REUSEADDR), error: %1%"_fmt % strerror(err));
            }

            posix::bind_socket(sockfd, rp->ai_addr, rp->ai_addrlen));
                
            posix::append_descriptor_flags(sockfd, O_NONBLOCK);

            posix::listen(sockfd, SOMAXCONN);

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

void lin_impl::udp_socket_bind(udp_handle_type handle, cstring_view address, uint16_t port)
{
    THROW_NOT_IMPLEMENTED_ERROR("udp_socket_bind");
}

size_t lin_impl::udp_socket_waiting_count(tcp_handle_type handle)
{
    auto * sh = static_cast<lin_shared_handle*>(handle);
    return sh->waiting_cnt.load();
}

expected<size_t, std::exception_ptr> lin_impl::udp_socket_read_some(udp_handle_type, void * buff, size_t sz, sonia::sal::socket_address* addr)
{
    THROW_NOT_IMPLEMENTED_ERROR("udp_socket_read_some");
}

expected<size_t, std::exception_ptr> lin_impl::udp_socket_write_some(udp_handle_type handle, sonia::sal::socket_address const& address, void const* buff, size_t sz)
{
    THROW_NOT_IMPLEMENTED_ERROR("udp_socket_write_some");
}

expected<size_t, std::exception_ptr> lin_impl::udp_socket_write_some(udp_handle_type handle, cstring_view address, uint16_t port, void const* buff, size_t sz)
{
    THROW_NOT_IMPLEMENTED_ERROR("udp_socket_write_some");
}

void lin_impl::close_handle(identity<udp_socket_service_type>, udp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void lin_impl::release_handle(identity<udp_socket_service_type>, udp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void lin_impl::free_handle(identity<udp_socket_service_type>, udp_handle_type h) noexcept
{
    delete_socket_handle(static_cast<lin_shared_handle*>(h));
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

#ifndef __ANDROID__
struct file_callback
{
    aiocb cb;
    linux::user_handler_type h_;

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
        cb.aio_sigevent.sigev_signo = linux::get_user_signal();
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
#else
size_t factory::file_read(sonia::sal::file_handle_type, uint64_t fileoffset, array_view<char> dest)
{
    THROW_NOT_IMPLEMENTED_ERROR("io_factory::file_read");
}

size_t factory::file_write(sonia::sal::file_handle_type, uint64_t fileoffset, array_view<const char> src)
{
    THROW_NOT_IMPLEMENTED_ERROR("io_factory::file_write");
}

#endif

}
