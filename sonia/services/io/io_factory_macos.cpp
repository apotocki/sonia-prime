//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "factory.hpp"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/event.h>

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

#ifndef MAX_KQUEUE_EVENTS
#   define MAX_KQUEUE_EVENTS 5
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

struct macos_impl;
struct macos_shared_handle;

struct callback : boost::intrusive::list_base_hook<>
{
    virtual ~callback() = default;

    virtual bool operator()(macos_shared_handle & sh, bool eof) noexcept = 0; // returns true if completed and needs to be removed; if eof is true, must report about eof and reutrn true

    virtual void free(macos_shared_handle & sh) = 0;
};

struct acceptor_callback : callback
{
    fibers::promise<tcp_socket> promise_;

    bool operator()(macos_shared_handle & sh, bool eof) noexcept override;

    fibers::future<tcp_socket> get_future() { return promise_.get_future(); }

    void free(macos_shared_handle & sh) override;
};

struct macos_shared_handle : macos_shared_handle<socket_handle_traits>
{
    using base_type = macos_shared_handle<socket_handle_traits>;

    using callback_t = automatic_polymorphic<callback, sizeof(acceptor_callback)>;
    using callback_list_t = boost::intrusive::list<callback>;

    shared_ptr<macos_impl> impl;
    fibers::mutex mtx;
    fibers::condition_variable_any cnd;
    sonia::sal::socket_handle handle;

    macos_shared_handle(shared_ptr<macos_impl> p, sonia::sal::socket_handle h, uint32_t id, sonia::sal::net_family_type ftval)
        : impl(std::move(p)), handle(h), family_(ftval), bkid_(id)
    {}

    ~macos_shared_handle()
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

struct shared_handle_hasher
{
    size_t operator()(macos_shared_handle * ph) const { return ph->bkid(); }
    size_t operator()(uint32_t id) const { return id; }
};

struct shared_handle_equal_to
{
    size_t operator()(macos_shared_handle * lhs, macos_shared_handle * rhs) const { return lhs == rhs; }
    size_t operator()(macos_shared_handle * lhs, uint32_t rhs) const { return lhs->bkid() == rhs; }
    size_t operator()(uint32_t lhs, macos_shared_handle * rhs) const { return lhs == rhs->bkid(); }
};

struct macos_impl 
    : public factory::impl_base
    , public enable_shared_from_this<macos_impl>
{
    friend class factory;

    int kq;
    int ctl_pipe[2];

    sonia::spin_mutex bk_mtx_;
    std::atomic<uint32_t> bkid_cnt_{0};
    boost::unordered_set<macos_shared_handle*, shared_handle_hasher> book_keeper_;

    object_pool<macos_shared_handle, spin_mutex> sockets_;
    object_pool<macos_shared_handle::callback_t, spin_mutex> callbacks_;

    explicit macos_impl(shared_ptr<factory> itself);

    macos_shared_handle * new_socket_handle(int sockfd, sonia::sal::net_family_type);

    void delete_socket_handle(macos_shared_handle*) noexcept;

    template <typename ... ArgsT>
    macos_shared_handle::callback_t * new_callback(ArgsT&& ... args);
    void delete_callback(macos_shared_handle::callback_t *) noexcept;

    void park_threads() noexcept override
    {
        LOG_TRACE(wrapper->logger()) << "parking threads...";
        char ch = 'e';
        if (int r = ::write(ctl_pipe[1], &ch, 1); 1 != r) {
            int err = errno;
            LOG_ERROR(wrapper->logger()) << "can't park threads, error: " << strerror(err);
        }
    }

    ~macos_impl() override
    {
        LOG_TRACE(wrapper->logger()) << "descruction of macos_impl";
        ::close(ctl_pipe[0]);
        ::close(ctl_pipe[1]);
        ::close(kq);
    }

    void thread_proc() override;

    macos_shared_handle* do_create_socket(sonia::sal::socket_handle, sonia::sal::net_family_type);

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

tcp_socket macos_shared_handle::create_tcp_socket(int fd)
{
    return impl->do_create_tcp_socket(fd, family());
}

void macos_shared_handle::handle_callbacks(bool eof)
{
    for (auto it = callback_list_.begin(), eit = callback_list_.end(); it != eit; it) {
        callback & cb = *it;
        if (cb(*this, eof)) {
            --waiting_cnt;
            it = callback_list_.erase(it);
            cb.free(*this);
        } else {
            BOOST_ASSERT (!eof);
            ++it;
        }
    }
}

bool macos_shared_handle::close(int kq)
{
    if (lock_guard guard(mtx); handle != -1) {
        struct kevent ev;
        EV_SET(&ev, epool_exit_cookie_v, EVFILT_READ | EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);
        if (-1 == kevent(kq, &ev, 1, NULL, 0, NULL)) {
            int err = errno;
            LOG_WARN(impl->logger()) << ("can't start watching the controll pipe, error: %1%"_fmt % strerror(err)).str();
        }
        ::close(handle);
        handle = -1;
        handle_callbacks(true);
        return true;
    }
    return false;
}

bool acceptor_callback::operator()(macos_shared_handle & sh, bool eof) noexcept
{
    if (eof) {
        promise_.set_exception(std::make_exception_ptr(eof_exception()));
        return true;
    }

    ::sockaddr in_addr;
    socklen_t in_len = sizeof(::in_addr);
    int infd = accept(sh.handle, &in_addr, &in_len);
    if (infd == -1) {
        int err = errno;
        if (err == EAGAIN) return false;
        promise_.set_exception(std::make_exception_ptr(eof_exception("accept error: %1%"_fmt % strerror(err))));
        return true;
    }

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

void acceptor_callback::free(macos_shared_handle & sh)
{
    sh.impl->delete_callback(reinterpret_cast<macos_shared_handle::callback_t*>(this));
}
     
macos_impl::macos_impl(shared_ptr<factory> itself)
    : factory::impl_base(std::move(itself))
    , sockets_(SOCKET_POOL_SIZE)
    , callbacks_(CALLBACK_POOL_SIZE)
{
    if (kq = kqueue(); kq == -1) {
        int err = errno;
        throw exception("can't create kqueue instance, error: %1%"_fmt % strerror(err));
    }

    if (int r = ::pipe(ctl_pipe); r != 0) {
        int err = errno;
        throw exception("can't create a contorl pipe instance, error: %1%"_fmt % strerror(err));
    }

    posix::append_descriptor_flags(ctl_pipe[0], O_NONBLOCK); // read

    struct kevent ev;
    EV_SET(&ev, epool_exit_cookie_v, EVFILT_READ, EV_ADD, 0, 0, nullptr);
    if (-1 == kevent(kq, &ev, 1, NULL, 0, NULL)) {
        int err = errno;
        throw exception("can't start watching the controll pipe, error: %1%"_fmt % strerror(err));
    }

    auto[a, b, c] = posix::kernel_version();
    LOG_INFO(wrapper->logger()) << "KERNEL: " << a << "." << b << "." << c;
}

void macos_impl::thread_proc()
{
    struct kevent events[MAX_KQUEUE_EVENTS];
    for (;;) {
        if constexpr (IO_DEBUG) LOG_INFO(wrapper->logger()) << "before kevent, thread: " << this_thread::get_id();
        int n = kevent(kq, nullptr, 0, events, MAX_KQUEUE_EVENTS, nullptr);
        if constexpr (IO_DEBUG) LOG_INFO(wrapper->logger()) << "woke up thread: " << this_thread::get_id();
        if (-1 == n) {
            int err = errno;
            LOG_ERROR(wrapper->logger()) << "kevent interrupted: " << strerror(err) ;
            return;
        }
        if (n == 0) {
            LOG_ERROR(wrapper->logger()) << "kevent WRONG WOKE UP";
        }
        for (int i = 0; i < n; i++) {
            if (epool_exit_cookie_v == (uint64_t)events[i].ident) return; // close event
            uint32_t bkid = (uint32_t)events[i].ident;

            macos_shared_handle* h;
            {
                lock_guard guard(bk_mtx_);
                auto it = book_keeper_.find(bkid, shared_handle_hasher(), shared_handle_equal_to());
                if (it == book_keeper_.end()) continue; // skip obsoleted descriptor event
                h = static_cast<macos_shared_handle*>(*it);
                h->add_weakref();
            }

            if (h) {
                h->on_event(); // noexcept
                h->release_weak((tcp_socket_service_type*)this);
            }
        }
    }
}

macos_shared_handle * macos_impl::new_socket_handle(int sockfd, sonia::sal::net_family_type ft)
{
    for (;;) {
        macos_shared_handle *ph = sockets_.new_object(shared_from_this(), sockfd, bkid_cnt_.fetch_add(1), ft);
        lock_guard guard(bk_mtx_);
        if (BOOST_LIKELY(book_keeper_.insert(ph).second)) {
            ph->add_weakref();
            return ph;
        }
        sockets_.delete_object(ph);
    }
}

void macos_impl::delete_socket_handle(macos_shared_handle * h) noexcept
{
    sockets_.delete_object(h);
}

template <typename ... ArgsT>
macos_shared_handle::callback_t * macos_impl::new_callback(ArgsT&& ... args)
{
    return callbacks_.new_object(std::forward<ArgsT>(args) ...);
}

void macos_impl::delete_callback(macos_shared_handle::callback_t * cb) noexcept
{
    callbacks_.delete_object(cb);
}

tcp_server_socket macos_impl::do_create_tcp_server_socket(sonia::sal::socket_handle s, sonia::sal::net_family_type dt)
{
    posix::append_descriptor_flags(s, O_NONBLOCK);

    macos_shared_handle * sh = new_socket_handle(s, dt);
    
    struct kevent ev;
    EV_SET(&ev, sh->bkid(), EVFILT_READ, EV_ADD, 0, 0, nullptr);
    if (-1 == kevent(kq, &ev, 1, NULL, 0, NULL)) {
        int err = errno;
        delete_socket_handle(sh);
        throw exception("can't watch socket, error: %1%"_fmt % strerror(err));
    });

    return tcp_socket_access::create_tcp_server_socket<socket_traits>(shared_from_this(), sh);
}

tcp_socket macos_impl::do_create_tcp_socket(sonia::sal::socket_handle s, sonia::sal::net_family_type dt)
{
    posix::append_descriptor_flags(s, O_NONBLOCK);

    macos_shared_handle * sh = new_socket_handle(s, dt);
    
    struct kevent ev;
    EV_SET(&ev, sh->bkid(), EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, nullptr);
    if (-1 == kevent(kq, &ev, 1, NULL, 0, NULL)) {
        int err = errno;
        delete_socket_handle(sh);
        throw exception("can't watch socket, error: %1%"_fmt % strerror(err));
    });

    return tcp_socket_access::create_tcp_socket<socket_traits>(shared_from_this(), sh);
}

udp_socket macos_impl::do_create_udp_socket(sonia::sal::socket_handle s, sonia::sal::net_family_type dt)
{
    posix::append_descriptor_flags(s, O_NONBLOCK);

    macos_shared_handle * sh = new_socket_handle(s, dt);
    
    struct kevent ev;
    EV_SET(&ev, sh->bkid(), EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, nullptr);
    if (-1 == kevent(kq, &ev, 1, NULL, 0, NULL)) {
        int err = errno;
        delete_socket_handle(sh);
        throw exception("can't watch socket, error: %1%"_fmt % strerror(err));
    });

    return udp_socket_access::create_udp_socket<socket_traits>(shared_from_this(), sh);
}


factory::factory() {}

void factory::initialize_impl()
{
    if (impl_holder_) {
        THROW_INTERNAL_ERROR("io factory is already initialized");
    }
    impl_holder_ = make_shared<macos_impl>(shared_from_this());
    impl_ = impl_holder_;
}

std::string factory::name() const
{
    return std::string("macos");
}


fibers::future<tcp_socket> macos_impl::tcp_server_socket_accept(tcp_handle_type handle)
{
    auto * sh = static_cast<macos_shared_handle*>(handle);
    SCOPE_EXIT([&sh]() { if (sh) --sh->waiting_cnt; });
    ++sh->waiting_cnt;

    auto * pcb = new_callback(in_place_type<acceptor_callback>);
    SCOPE_EXIT([this, &pcb]() { if (pcb) delete_callback(pcb); });

    auto ftr = static_cast<acceptor_callback*>(pcb->get_pointer())->get_future();

    unique_lock lck(sh->mtx);
    
    if (!(**pcb)(*sh, false)) {
        sh->add_callback(pcb->get_pointer());
        pcb = nullptr; // unpin
        sh = nullptr; // unpin
    }
        
    return std::move(ftr);
}

size_t macos_impl::tcp_server_socket_accepting_count(tcp_handle_type handle) const
{
    auto * sh = static_cast<macos_shared_handle*>(handle);
    return sh->waiting_cnt.load();
}

void macos_impl::close_handle(identity<tcp_server_socket_service_type>, tcp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void macos_impl::release_handle(identity<tcp_server_socket_service>, tcp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void macos_impl::free_handle(identity<tcp_server_socket_service>, tcp_handle_type h) noexcept
{
    delete_socket_handle(static_cast<macos_shared_handle*>(h));
}

expected<size_t, std::exception_ptr> macos_impl::tcp_socket_read_some(tcp_handle_type handle, void * buff, size_t sz) noexcept
{
    auto * sh = static_cast<macos_shared_handle*>(handle);
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
        sh->wait(lck);
        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) woke up"_fmt % sh->handle);
    }
}

expected<size_t, std::exception_ptr> macos_impl::tcp_socket_write_some(tcp_handle_type handle, void const* buff, size_t sz) noexcept
{
    auto * sh = static_cast<macos_shared_handle*>(handle);
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
        sh->wait(lck);
        if constexpr (IO_DEBUG) LOG_TRACE(wrapper->logger()) << to_string("socket(%1%) woke up"_fmt % sh->handle);
    }
}

void macos_impl::close_handle(identity<tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    auto * sh = static_cast<macos_shared_handle*>(h);
    if (sh) {
        if (sh->close(kq)) {
            lock_guard guard(bk_mtx_);
            BOOST_VERIFY(1 == book_keeper_.erase(sh));
            sh->release_weak((tcp_socket_service_type*)this);
        }
        sh->cnd.notify_all();
    }
}

void macos_impl::release_handle(identity<tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void macos_impl::free_handle(identity<tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    delete_socket_handle(static_cast<macos_shared_handle*>(h));
}

void macos_impl::udp_socket_bind(udp_handle_type handle, cstring_view address, uint16_t port)
{
    THROW_NOT_IMPLEMENTED_ERROR("udp_socket_bind");
}

size_t macos_impl::udp_socket_waiting_count(tcp_handle_type handle)
{
    auto * sh = static_cast<macos_shared_handle*>(handle);
    return sh->waiting_cnt.load();
}

expected<size_t, std::exception_ptr> macos_impl::udp_socket_read_some(udp_handle_type, void * buff, size_t sz, sonia::sal::socket_address* addr)
{
    THROW_NOT_IMPLEMENTED_ERROR("udp_socket_read_some");
}

expected<size_t, std::exception_ptr> macos_impl::udp_socket_write_some(udp_handle_type handle, sonia::sal::socket_address const& address, void const* buff, size_t sz)
{
    THROW_NOT_IMPLEMENTED_ERROR("udp_socket_write_some");
}

expected<size_t, std::exception_ptr> macos_impl::udp_socket_write_some(udp_handle_type handle, cstring_view address, uint16_t port, void const* buff, size_t sz)
{
    THROW_NOT_IMPLEMENTED_ERROR("udp_socket_write_some");
}

void macos_impl::close_handle(identity<udp_socket_service_type>, udp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void macos_impl::release_handle(identity<udp_socket_service_type>, udp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void macos_impl::free_handle(identity<udp_socket_service_type>, udp_handle_type h) noexcept
{
    delete_socket_handle(static_cast<macos_shared_handle*>(h));
}

file factory::open_file(cstring_view path, file_open_mode fom, file_access_mode fam, file_bufferring_mode fbm)
{
    int flags = 0;
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
        flags |= O_SYNC | F_NOCACHE;
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

#if 0
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
#endif
size_t factory::file_read(int handle, uint64_t fileoffset, array_view<char> dest)
{
	THROW_NOT_IMPLEMENTED_ERROR();
	#if 0
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
    #endif
}

size_t factory::file_write(int handle, uint64_t fileoffset, array_view<const char> src)
{
	THROW_NOT_IMPLEMENTED_ERROR();
	#if 0
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
    #endif
}

}
