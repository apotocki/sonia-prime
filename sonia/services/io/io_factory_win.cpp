//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/utility/windows.hpp"

#include <cstddef>
#include <memory>
#include <iostream>

#include <boost/integer_traits.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "sonia/optional.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/concurrency.hpp"

#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/object_pool.hpp"

#include "factory.hpp"

#pragma comment(lib, "ws2_32.lib")

namespace sonia::io {

namespace winapi = sonia::windows;

//using tcp_server_socket_service_type = tcp_server_socket_service<socket_traits>;
//using tcp_socket_service_type = tcp_socket_service<socket_traits>;
//using udp_socket_service_type = udp_socket_service<socket_traits>;

struct win_impl;

enum class completion_port
{
    socket_callback_key = 0,
    file_callback_key,
    close_key
};

struct win_shared_handle : shared_handle<socket_handle_traits>
{
    using base_type = shared_handle<socket_handle_traits>;

    win_shared_handle(sonia::sal::socket_handle s, sonia::sal::net_family_type ftval)
        : family(ftval), handle(s)
    {}

    SOCKET socket() const noexcept { return handle; }
    std::atomic<size_t> waiting_cnt{0};
    fibers::mutex mtx;
    sonia::sal::net_family_type family;
    SOCKET handle;
};

template <typename OvT>
struct callback_base
{
    OvT overlapped;

    static callback_base * get(OvT * pov)
    {
        return reinterpret_cast<callback_base*>(reinterpret_cast<char*>(pov) - offsetof(callback_base, overlapped));
    }
};

template <typename OvT>
struct callback : callback_base<OvT>
{
    explicit callback(shared_ptr<win_impl>);
    
    virtual void on_op(std::error_code const&, size_t) = 0;

    static callback * get(OvT * pov) { return static_cast<callback*>(callback_base<OvT>::get(pov)); }

protected:
    ~callback();
    shared_ptr<win_impl> impl_;
};


struct acceptor_async_callback
    : callback<WSAOVERLAPPED>
    , private unique_lock<fibers::mutex>
{
    using base_type = callback<WSAOVERLAPPED>;
    using lock_type = unique_lock<fibers::mutex>;
    win_shared_handle * acceptor_;
    fibers::promise<tcp_socket> promise;

    SOCKET sock2accept{INVALID_SOCKET};
    char buff[2 * sizeof(sockaddr_in) + 32];

    explicit acceptor_async_callback(shared_ptr<win_impl> p, win_shared_handle * ash)
        : base_type{std::move(p)}, acceptor_{ash}
    {
        ++acceptor_->waiting_cnt;
    }

    ~acceptor_async_callback();

    void on_op(std::error_code const& err, size_t sz) override;
};

template <typename OvT>
struct sync_callback
    : callback<OvT>
    , private unique_lock<fibers::mutex>
{
    using base_type = callback<OvT>;
    using lock_type = unique_lock<fibers::mutex>;

    size_t handlsz;
    optional<std::error_code> code;
    fibers::mutex mtx;
    fibers::condition_variable_any cnd;

    explicit sync_callback(shared_ptr<win_impl>p, fibers::mutex & m)
        : base_type{std::move(p)}, lock_type{m}
    {}

    void on_op(std::error_code const& err, size_t sz) override
    {
        handlsz = sz;
        //GLOBAL_LOG_TRACE() << "notify waiters";
        unique_lock lck(mtx);
        code = err;
        if (err) {
            auto msg = err.message();
        }
        //cnd.notify_one();

        cnd.notify_all();
    }

    expected<size_t, std::error_code> wait() noexcept
    {
        this->unlock();
        unique_lock lck{mtx};
        //GLOBAL_LOG_TRACE() << "before wait";
        cnd.wait(lck, [this] { return !!code; });
        //GLOBAL_LOG_TRACE() << "after wait";
        if (*code) {
            return make_unexpected(*code);
        }
        return handlsz;
    }

    expected<size_t, std::exception_ptr> wait2() noexcept
    {
        this->unlock();
        unique_lock lck{mtx};
        //GLOBAL_LOG_TRACE() << "before wait";
        cnd.wait(lck, [this] { return !!code; });
        //GLOBAL_LOG_TRACE() << "after wait";
        if (*code) {
            return make_unexpected(std::make_exception_ptr(exception(code->message())));
        }
        return handlsz;
    }
};

/*
struct listener_callback : callback<WSAOVERLAPPED>
{
    using base_type = callback<WSAOVERLAPPED>;

    win_shared_handle * sh_;
    win_impl * impl_;
    function<void(udp_connection_handler_t<socket_traits> const&)> handler_;
    udp_connector_t<socket_traits> conn_;
    socket_address sender_addr_;
    
    template <typename HandlerT>
    listener_callback(win_impl * impl, win_shared_handle * sh, HandlerT const& h)
        : base_type(impl), sh_(sh), impl_(impl), handler_(h)
    {
        prepare();
        sh_->add_ref();
    }

    ~listener_callback();

    void prepare();

    void on_op(std::error_code const& err, size_t sz) override;

    //void free(win_impl * impl) override;
};
*/

struct win_impl 
    : public factory::impl_base
    , public enable_shared_from_this<win_impl>
{
    friend class factory;

    winapi::wsa_scope ws_;
    HANDLE iocp_;
    
    explicit win_impl(shared_ptr<factory> wr, uint32_t thr_cnt)
        : factory::impl_base(std::move(wr)), handles_(256), acceptor_callbacks_(16)
    {
        iocp_ = winapi::create_completion_port(thr_cnt);
    }

    ~win_impl() override
    {
        CloseHandle(iocp_);
    }

    void thread_proc() override;

    template <typename ... ArgsT>
    win_shared_handle * new_socket_handle(ArgsT && ... arg)
    {
        return handles_.new_object(std::forward<ArgsT>(arg) ...);
    }

    void delete_socket_handle(win_shared_handle * h) noexcept
    {
        handles_.delete_object(h);
    }

    void park_threads() noexcept override
    {
        LOG_TRACE(wrapper->logger()) << "parking threads...";
        size_t tc = wrapper->thread_count();
        for (uint32_t c = 0; c < tc; ++c) {
            try {
                winapi::post_completion_port(iocp_, 0, (ULONG_PTR)completion_port::close_key);
            } catch (std::exception const& e) {
                GLOBAL_LOG_ERROR() << e.what();
            }
        }
    }

    HANDLE iocp() const { return iocp_; }

    tcp_server_socket do_create_tcp_server_socket(sonia::sal::socket_handle, sonia::sal::net_family_type) override final;
    tcp_socket do_create_tcp_socket(sonia::sal::socket_handle, sonia::sal::net_family_type) override final;
    udp_socket do_create_udp_socket(sonia::sal::socket_handle, sonia::sal::net_family_type) override final;

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
    expected<size_t, std::error_code> udp_socket_read_some(udp_handle_type, void * buff, size_t sz, sonia::sal::socket_address* addr) override final;
    expected<size_t, std::error_code> udp_socket_write_some(udp_handle_type, sonia::sal::socket_address const&, void const* buff, size_t sz) override final;
    expected<size_t, std::error_code> udp_socket_write_some(udp_handle_type handle, cstring_view address, uint16_t port, void const* buff, size_t sz) override final;
    void close_handle(identity<udp_socket_service_type>, udp_handle_type) noexcept override final;
    void release_handle(identity<udp_socket_service_type>, tcp_handle_type) noexcept override final;
    void free_handle(identity<udp_socket_service_type>, udp_handle_type) noexcept override final;

    void free(acceptor_async_callback * cb) { acceptor_callbacks_.delete_object(cb); }

private:
    object_pool<win_shared_handle, spin_mutex> handles_;
    object_pool<acceptor_async_callback, fibers::mutex> acceptor_callbacks_;
};

#if 0
void win_impl::thread_proc()
{
    for (;;)
    {
        OVERLAPPED_ENTRY overlapped_arr[1];

        ULONG rnum;
        BOOL r = GetQueuedCompletionStatusEx(iocp_, overlapped_arr, sizeof(overlapped_arr)/sizeof(OVERLAPPED_ENTRY), &rnum, INFINITE, TRUE);
        DWORD errc = GetLastError();
        std::error_code err(errc, std::system_category());
        
        for (ULONG i = 0; i < rnum; ++i) {
            LPOVERLAPPED overlapped = overlapped_arr[i].lpOverlapped;
            ULONG_PTR key = overlapped_arr[i].lpCompletionKey;
            DWORD bytes = overlapped_arr[i].dwNumberOfBytesTransferred;

            try {
                if (overlapped) {
                    switch ((completion_port)key) {
                    case completion_port::socket_callback_key:
                    {
                        auto * cb = callback<WSAOVERLAPPED>::get(overlapped);
                        cb->on_op(err, (size_t)bytes);
                        break;
                    }
                    case completion_port::file_callback_key:
                    {
                        auto* cb = callback<OVERLAPPED>::get(overlapped);
                        cb->on_op(err, (size_t)bytes);
                        break;
                    }
                    default:
                        LOG_ERROR(wrapper->logger()) << "completion port unknown key: " << (int)key;
                    }
                } else {
                    if (!r) {
                        LOG_ERROR(wrapper->logger()) << "completion port errror: " << winapi::error_message(errc);
                    } else if (key == (ULONG_PTR)completion_port::close_key) {
                        break;
                    } else {
                        LOG_ERROR(wrapper->logger()) << "completion port unknown key: " << (int)key;
                    }
                }
            } catch (std::exception const& e) {
                LOG_ERROR(wrapper->logger()) << e.what();
            }
        }
    }
}
#else
void win_impl::thread_proc()
{
    for (;;)
    {
        DWORD bytes;
        ULONG_PTR key = 0;
        LPOVERLAPPED overlapped = nullptr;

        //LOG_INFO(wrapper->logger()) << "start listening to QC";
        BOOL r = GetQueuedCompletionStatus(iocp_, &bytes, &key, &overlapped, INFINITE);
        //LOG_INFO(wrapper->logger()) << "woke up QC";
        DWORD errc = r ? 0 : GetLastError();
        std::error_code err(errc, std::system_category());

        try {
            if (overlapped) {
                switch ((completion_port)key) {
                case completion_port::socket_callback_key:
                {
                    auto * cb = callback<WSAOVERLAPPED>::get(overlapped);
                    cb->on_op(err, (size_t)bytes);
                    break;
                }
                case completion_port::file_callback_key:
                {
                    auto* cb = callback<OVERLAPPED>::get(overlapped);
                    cb->on_op(err, (size_t)bytes);
                    break;
                }
                default:
                    LOG_ERROR(wrapper->logger()) << "completion port unknown key: " << (int)key;
                }
            } else {
                if (!r) {
                    LOG_ERROR(wrapper->logger()) << "completion port errror: " << winapi::error_message(errc);
                } else if (key == (ULONG_PTR)completion_port::close_key) {
                    break;
                } else {
                    LOG_ERROR(wrapper->logger()) << "completion port unknown key: " << (int)key;
                }
            }
        } catch (std::exception const& e) {
            LOG_ERROR(wrapper->logger()) << e.what();
        }
    }
}
#endif

tcp_server_socket win_impl::do_create_tcp_server_socket(sonia::sal::socket_handle s, sonia::sal::net_family_type ft)
{
    winapi::assign_completion_port((HANDLE)s, iocp_, (ULONG_PTR)completion_port::socket_callback_key);
    return tcp_socket_access::create_tcp_server_socket<socket_traits>(shared_from_this(), new_socket_handle(s, ft));
}

tcp_socket win_impl::do_create_tcp_socket(sonia::sal::socket_handle s, sonia::sal::net_family_type ft)
{
    winapi::assign_completion_port((HANDLE)s, iocp_, (ULONG_PTR)completion_port::socket_callback_key);
    return tcp_socket_access::create_tcp_socket<socket_traits>(shared_from_this(), new_socket_handle(s, ft));
}

udp_socket win_impl::do_create_udp_socket(sonia::sal::socket_handle s, sonia::sal::net_family_type ft)
{
    winapi::assign_completion_port((HANDLE)s, iocp_, (ULONG_PTR)completion_port::socket_callback_key);
    return udp_socket_access::create_udp_socket<socket_traits>(shared_from_this(), new_socket_handle(s, ft));
}

template <typename OvT>
callback<OvT>::callback(shared_ptr<win_impl> p) : impl_(std::move(p))
{
    BOOST_ASSERT(this == get(&this->overlapped));
    SecureZeroMemory((PVOID)&this->overlapped, sizeof(OvT));
    impl_->on_add_callback();
}

template <typename OvT>
callback<OvT>::~callback()
{
    impl_->on_release_callback();
}

acceptor_async_callback::~acceptor_async_callback()
{
    if (sock2accept != INVALID_SOCKET) closesocket(sock2accept); 
}

void acceptor_async_callback::on_op(std::error_code const& err, size_t sz)
{
    SCOPE_EXIT([this](){ impl_->free(this); });
    --acceptor_->waiting_cnt;
    if (!err) try {
        auto newh = impl_->new_socket_handle(sock2accept, acceptor_->family);
        promise.set_value(tcp_socket_access::create_tcp_socket<socket_traits>(impl_, newh));
        sock2accept = INVALID_SOCKET;
    } catch (...) {
        promise.set_exception(std::current_exception()); // very unlikely case
    } else {
        promise.set_exception(std::make_exception_ptr(eof_exception("can't accept, error: %1%"_fmt % err.message())));
    }
}

//void async_callback::free(win_impl * impl)
//{
//    impl->delete_async_callback(this);
//}

#if 0
void acceptor_callback::prepare()
{
    SOCKET accept_sock = winapi::create_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SCOPE_EXIT([&accept_sock]() { if (accept_sock != INVALID_SOCKET) closesocket(accept_sock); });

    winapi::assign_completion_port((HANDLE)accept_sock, impl_->iocp(), (ULONG_PTR)completion_port::socket_callback_key);
    socket = accept_sock;
    handler_([this](array_view<char> buff, tcp_connector_t<socket_traits> const& conn) {
        this->conn_ = conn;
        this->accept(buff);
    });
    accept_sock = INVALID_SOCKET;
}

void acceptor_callback::on_op(std::error_code const& err, size_t bytes)
{
    if (!err) {
        conn_(tcp_socket_access::create_tcp_socket<socket_traits>(impl_->wrapper, impl_->new_socket_handle(socket)), (size_t)bytes);
        try {
            prepare();
            ++refs;
        } catch (...) {
            GLOBAL_LOG_ERROR() << "acceptor closed unexpectedly, errror: " << boost::current_exception_diagnostic_information();
        }
    } else if (err.value() != ERROR_OPERATION_ABORTED) {
        GLOBAL_LOG_WARN() << "acceptor closed: " << boost::trim_right_copy(err.message());
    }
}

void acceptor_callback::free(win_impl * impl)
{
    impl->delete_acceptor_callback(this);
}

acceptor_callback::~acceptor_callback()
{
    ah_->release((tcp_socket_service_type*) impl_->wrapper.get());
}
#endif

//void listener_callback::prepare()
//{
//    handler_([this](array_view<char> buff, udp_connector_t<socket_traits> const& conn) {
//        this->conn_ = conn;
//        sender_addr_.reset();
//        int sz = sender_addr_.size();
//        winapi::async_recvfrom(sh_->socket(), buff.begin(), buff.size(), sender_addr_.addr<SOCKADDR>(), &sz, &overlapped);
//    });
//}
//
//void listener_callback::on_op(std::error_code const& err, size_t bytes)
//{
//    if (!err) {
//        conn_(bytes, sender_addr_, udp_socket_access::create_udp_socket<socket_traits::weak_traits_type>(impl_->wrapper, sh_));
//        try {
//            prepare();
////            ++refs;
//        } catch (...) {
//            GLOBAL_LOG_ERROR() << "udp listener closed unexpectedly, errror: " << boost::current_exception_diagnostic_information();
//        }
//    } else if (err.value() != ERROR_OPERATION_ABORTED) {
//        GLOBAL_LOG_WARN() << "listener socket closed: " << boost::trim_right_copy(err.message());
//    }
//}

//void listener_callback::free(win_impl * impl)
//{
//    impl->delete_listener_callback(this);
//}

//listener_callback::~listener_callback()
//{
//    sh_->release((udp_socket_service_type*)impl_->wrapper.get());
//}

//inline shared_ptr<win_impl> get_dataptr2(weak_ptr<factory::impl_base> const& wptr)
//{
//    shared_ptr<win_impl> r = static_pointer_cast<win_impl>(wptr.lock());
//    if (!r) throw closed_exception();
//    return r;
//}

#define dataptr static_pointer_cast<win_impl>(get_dataptr())

factory::factory() {}

void factory::initialize_impl(uint32_t thr_cnt)
{
    if (impl_holder_) {
        THROW_INTERNAL_ERROR("io factory is already initialized");
    }
    
    impl_holder_ = make_shared<win_impl>(shared_from_this(), thr_cnt);
    impl_ = impl_holder_;
}

std::string factory::name() const
{
    return std::string("windows");
}

// tcp_socket_factory
#if 0
tcp_socket factory::create_tcp_socket(tcp_socket_type dt)
{
    SOCKET sock = winapi::create_socket(af_value(dt), SOCK_STREAM, IPPROTO_TCP);
    SOCKET pinsock = sock;
    SCOPE_EXIT([&pinsock]() { if (pinsock != INVALID_SOCKET) closesocket(pinsock); });

    winapi::assign_completion_port((HANDLE)sock, dataptr->iocp_, (ULONG_PTR)completion_port::socket_callback_key);
    win_shared_handle * sh = dataptr->new_socket_handle(sock);

    pinsock = INVALID_SOCKET; // detach
    return tcp_socket_access::create_tcp_socket<socket_traits>(shared_from_this(), sh);
}
#endif

/*
tcp_socket factory::create_connected_tcp_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt)
{
    std::ostringstream errmsgs;
    win_shared_handle * sh;
    if (!winapi::parse_address(af_value(dt), SOCK_STREAM, IPPROTO_TCP, address, port, [&errmsgs, &sh, dataptr = get_dataptr(impl_data_)](ADDRINFOW * r)->bool {
        try {
            SOCKET soc = winapi::create_socket(r->ai_family, r->ai_socktype, r->ai_protocol);
            SOCKET pinsoc = soc;
            SCOPE_EXIT([&pinsoc]() { if (pinsoc != INVALID_SOCKET) closesocket(pinsoc); });

            //int enable = 1;
            //if (setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int)) < 0) {
            //    DWORD err = WSAGetLastError();
            //    throw exception("can't set SO_REUSEADDR, error: %1%"_fmt % winapi::error_message(err));
            //}

            winapi::connect_socket(soc, r->ai_addr, (int)r->ai_addrlen);
            winapi::assign_completion_port((HANDLE)soc, dataptr->iocp_, (ULONG_PTR)completion_port::socket_callback_key);
            sh = dataptr->new_socket_handle(soc);
            pinsoc = INVALID_SOCKET; // detach
            return true;
        } catch (std::exception const& e) {
            if (!errmsgs.str().empty()) errmsgs << '\n';
            errmsgs << e.what();
        }
        return false;
    })) {
        throw exception("can't connect socket to the address: '%1%:%2%', error(s): %3%"_fmt % address % port % errmsgs.str());
    }
    return tcp_socket_access::create_tcp_socket<socket_traits>(shared_from_this(), sh);
    
}
*/

#if 0
tcp_socket factory::bind_tcp_socket(cstring_view address, uint16_t port, tcp_socket_type dt)
{
    /*
    win_shared_handle * sh;
    if (!winapi::parse_address(af_value(dt), SOCK_STREAM, IPPROTO_TCP, address, port, [this, &errmsgs, &sh](ADDRINFOW * r)->bool {
        try {
            SOCKET soc = winapi::create_socket(r->ai_family, r->ai_socktype, r->ai_protocol);
            SOCKET pinsoc = soc;
            SCOPE_EXIT([&pinsoc]() { if (pinsoc != INVALID_SOCKET) closesocket(pinsoc); });

            int enable = 1;
            if (setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int)) < 0) {
                DWORD err = WSAGetLastError();
                throw exception("can't set SO_REUSEADDR, error: %1%"_fmt % winapi::error_message(err));
            }

            winapi::bind_socket(soc, r->ai_addr, (int)r->ai_addrlen);
            winapi::listen_socket(soc, SOMAXCONN);
            winapi::assign_completion_port((HANDLE)soc, dataptr->iocp_, (ULONG_PTR)completion_port::socket_callback_key);
            sh = dataptr->new_socket_handle(soc);
            pinsoc = INVALID_SOCKET; // detach
            return true;
        } catch (std::exception const& e) {
            if (!errmsgs.str().empty()) errmsgs << '\n';
            errmsgs << e.what();
        }
        return false;
    })) {
        throw exception("can't bind socket to the address: '%1%:%2%', error(s): %3%"_fmt % address % port % errmsgs.str());
    }
    return tcp_socket_access::create_tcp_socket<socket_traits>(shared_from_this(), sh);
    */
}
#endif

// tcp_socket_service
fibers::future<tcp_socket> win_impl::tcp_server_socket_accept(tcp_handle_type handle)
{
    auto * wh = static_cast<win_shared_handle*>(handle);
    
    acceptor_async_callback * acb = acceptor_callbacks_.new_object(shared_from_this(), wh);
    SCOPE_EXIT([this, &acb]() { if (acb) acceptor_callbacks_.delete_object(acb); });
    
    acb->sock2accept = winapi::create_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    winapi::assign_completion_port((HANDLE)acb->sock2accept, iocp_, (ULONG_PTR)completion_port::socket_callback_key);

    lock_guard guard{wh->mtx};
    if (wh->socket() == INVALID_SOCKET) throw eof_exception("closed acceptor socket");
    LPFN_ACCEPTEX lpfnAcceptEx = winapi::get_accept_function(wh->socket());

    auto fres = acb->promise.get_future();

    if (!lpfnAcceptEx(wh->socket(), acb->sock2accept, acb->buff, (DWORD)0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &acb->overlapped)) {
        DWORD err = WSAGetLastError();
        if (err != WSA_IO_PENDING) {
            throw exception("can't accept socket, error: %1%"_fmt % winapi::error_message(err));
        }
    }
    
    acb = nullptr; // unpin
    return std::move(fres);
}

size_t win_impl::tcp_server_socket_accepting_count(tcp_handle_type handle) const
{
    auto * wh = static_cast<win_shared_handle*>(handle);
    return wh->waiting_cnt.load();
}

void win_impl::close_handle(identity<tcp_server_socket_service_type>, tcp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void win_impl::release_handle(identity<tcp_server_socket_service>, tcp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void win_impl::free_handle(identity<tcp_server_socket_service_type>, tcp_handle_type h) noexcept
{
    delete_socket_handle(static_cast<win_shared_handle*>(h));
}

size_t win_impl::udp_socket_waiting_count(tcp_handle_type handle)
{
    auto * wh = static_cast<win_shared_handle*>(handle);
    return wh->waiting_cnt.load();
}

expected<size_t, std::exception_ptr> win_impl::tcp_socket_read_some(tcp_handle_type handle, void * buff, size_t sz) noexcept
{
    auto * wh = static_cast<win_shared_handle*>(handle);
    sync_callback<WSAOVERLAPPED> scb{shared_from_this(), wh->mtx};
    std::error_code err = winapi::async_recv(wh->socket(), buff, sz, &scb.overlapped);
    if (err) return make_unexpected(std::make_exception_ptr(exception(err.message())));
    return scb.wait2(); // noexcept
}

expected<size_t, std::exception_ptr> win_impl::tcp_socket_write_some(tcp_handle_type handle, void const* buff, size_t sz) noexcept
{
    auto * wh = static_cast<win_shared_handle*>(handle);
    sync_callback<WSAOVERLAPPED> scb{shared_from_this(), wh->mtx};
    std::error_code err = winapi::async_send(wh->socket(), buff, sz, &scb.overlapped);
    if (err) return make_unexpected(std::make_exception_ptr(exception(err.message())));
    return scb.wait2(); // noexcept
}

void win_impl::close_handle(identity<tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    auto * wh = static_cast<win_shared_handle*>(h);
    if (wh) {
        lock_guard guard(wh->mtx);
        if (wh->socket() != INVALID_SOCKET) {
            ::shutdown(wh->socket(), SD_BOTH);
            ::closesocket(wh->socket());
            wh->handle = INVALID_SOCKET;
        }
    }
}

void win_impl::release_handle(identity<tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void win_impl::free_handle(identity<tcp_socket_service_type>, tcp_handle_type h) noexcept
{ 
    delete_socket_handle(static_cast<win_shared_handle*>(h));
}

#if 0
void factory::tcp_acceptor_on_close(acceptor_handle_type) noexcept
{}

void factory::free_handle(identity<tcp_acceptor_service_type>, acceptor_handle_type h) noexcept
{
    dataptr->delete_socket_handle(static_cast<win_shared_handle*>(h));
}

tcp_acceptor factory::create_tcp_acceptor(cstring_view address, uint16_t port, tcp_socket_type dt, function<void(tcp_acceptor_handler_type const&)> const& handler)
{
    std::ostringstream errmsgs;
    SOCKET sock;
    if (!winapi::parse_address(SOCK_STREAM, IPPROTO_TCP, address, port, [this, &errmsgs, &sock](ADDRINFOW * r)->bool {
        try {
            sock = winapi::create_socket(r->ai_family, r->ai_socktype, r->ai_protocol);
            SOCKET pinsock = sock;
            SCOPE_EXIT([&pinsock]() { if (pinsock != INVALID_SOCKET) closesocket(pinsock); });

            winapi::bind_socket(sock, r->ai_addr, (int)r->ai_addrlen);
            pinsock = INVALID_SOCKET; // detach
            return true;
        } catch (std::exception const& e) {
            if (!errmsgs.str().empty()) errmsgs << '\n';
            errmsgs << e.what();
        }
        return false;
    })) {
        throw exception("can't bind socket at the address: '%1%' and port: '%2%', error(s): %3%"_fmt % address % port % errmsgs.str());
    }

    SOCKET pinsock = sock;
    SCOPE_EXIT([&pinsock]() { if (pinsock != INVALID_SOCKET) closesocket(pinsock); });

    if (int iResult = ::listen(sock, SOMAXCONN); SOCKET_ERROR == iResult) {
        DWORD err = WSAGetLastError();
        throw exception("can't listen to socket, error: %1%"_fmt % winapi::error_message(err));
    }

    winapi::assign_completion_port((HANDLE)sock, dataptr->iocp_, (ULONG_PTR)completion_port::socket_callback_key);

    LPFN_ACCEPTEX lpfnAcceptEx = winapi::get_accept_function(sock);

    win_shared_handle * wsh = dataptr->new_socket_handle(sock);

    for (size_t i = 0, tc = thread_count(); i < tc; ++i) {
        dataptr->new_acceptor_callback(wsh, lpfnAcceptEx, handler);
    }
    
    pinsock = INVALID_SOCKET; // detach

    return tcp_acceptor_access::create_tcp_acceptor<socket_traits>(shared_from_this(), wsh);
}

#endif

void win_impl::udp_socket_bind(udp_handle_type handle, cstring_view address, uint16_t port)
{
    auto * wh = static_cast<win_shared_handle*>(handle);
    if (!winapi::parse_address(0, SOCK_DGRAM, IPPROTO_UDP, address, port, [&wh](ADDRINFOW * r)->bool {
        winapi::bind_socket(wh->socket(), r->ai_addr, (int)r->ai_addrlen);
        return true;
    })) {
        throw exception("can't bind socket at the address: '%1%' and port: '%2%'"_fmt % address % port);
    }
}

expected<size_t, std::error_code> win_impl::udp_socket_read_some(udp_handle_type handle, void * buff, size_t sz, sonia::sal::socket_address* addr)
{
    auto * wh = static_cast<win_shared_handle*>(handle);
    sync_callback<WSAOVERLAPPED> scb{shared_from_this(), wh->mtx};
    if (addr) {
        int sz = addr->size();
        winapi::async_recvfrom(wh->socket(), buff, sz, addr->addr<SOCKADDR>(), &sz, &scb.overlapped);
    } else {
        winapi::async_recv(wh->socket(), buff, sz, &scb.overlapped);
    }
    return scb.wait();
}

expected<size_t, std::error_code> win_impl::udp_socket_write_some(udp_handle_type handle, sonia::sal::socket_address const& address, void const* buff, size_t sz)
{
    auto * wh = static_cast<win_shared_handle*>(handle);
    sync_callback<WSAOVERLAPPED> scb{shared_from_this(), wh->mtx};
    auto[ai_addr, ai_len] = address.in_addr();
    winapi::async_send_to(wh->socket(), (sockaddr*)ai_addr, (int)ai_len, buff, sz, &scb.overlapped);
    return scb.wait();
}

expected<size_t, std::error_code> win_impl::udp_socket_write_some(udp_handle_type handle, cstring_view address, uint16_t port, void const* buff, size_t sz)
{
    expected<size_t, std::error_code> wrsz;
    auto params = std::tie(handle, wrsz, buff, sz);
    if (!winapi::parse_address(0, SOCK_DGRAM, IPPROTO_UDP, address, port, [this, &params](ADDRINFOW * r)->bool {
        auto&[handle, wrsz, buff, sz] = params;
        auto * wh = static_cast<win_shared_handle*>(handle);
        sync_callback<WSAOVERLAPPED> scb{shared_from_this(), wh->mtx};
        winapi::async_send_to(wh->socket(), r->ai_addr, (int)r->ai_addrlen, buff, sz, &scb.overlapped);
        wrsz = scb.wait();
        return true;
    })) {
        throw exception("can't send data to the address: '%1%' and port: '%2%'"_fmt % address % port);
    }
    return wrsz;
}

void win_impl::close_handle(identity<udp_socket_service_type>, udp_handle_type) noexcept
{}

void win_impl::release_handle(identity<udp_socket_service_type>, udp_handle_type) noexcept
{}

void win_impl::free_handle(identity<udp_socket_service_type>, udp_handle_type h) noexcept
{
    delete_socket_handle(static_cast<win_shared_handle*>(h));
}

file factory::open_file(cstring_view path, file_open_mode fom, file_access_mode fam, file_bufferring_mode fbm)
{
    std::wstring wpath = winapi::utf8_to_utf16(path);
    DWORD dwCreationDisposition;
    switch (fom) {
    case file_open_mode::create:
        dwCreationDisposition = CREATE_NEW;
        break;
    case file_open_mode::create_or_open:
        dwCreationDisposition = OPEN_ALWAYS;
        break;
    case file_open_mode::open:
        dwCreationDisposition = OPEN_EXISTING;
        break;
    default:
        throw internal_error("unknown file open mode: %1%, file: %2%"_fmt % (int)fom % path);
    }

    DWORD dwDesiredAccess = 0;
    switch (fam) {
    case file_access_mode::write:
        dwDesiredAccess = GENERIC_WRITE;
    case file_access_mode::read:
        dwDesiredAccess |= GENERIC_READ;
        break;
    default:
        throw internal_error("unknown file access mode: %1%, file: %2%"_fmt % (int)fam % path);
    }

    DWORD dwFlagsAndAttributes = FILE_FLAG_OVERLAPPED;
    switch (fbm) {
    case file_bufferring_mode::buffered:
        dwFlagsAndAttributes |= FILE_FLAG_RANDOM_ACCESS;
        break;
    case file_bufferring_mode::not_buffered:
        dwFlagsAndAttributes |= FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING;
        break;
    default:
        throw internal_error("unknown file buffering mode: %1%, file: %2%"_fmt % (int)fbm % path);
    }

    HANDLE h = CreateFileW(wpath.c_str(), dwDesiredAccess, FILE_SHARE_READ, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        DWORD err = GetLastError();
        throw exception("can't open file %1%, error: %2%"_fmt % path % winapi::error_message(err));
    }

    winapi::assign_completion_port(h, dataptr->iocp_, (ULONG_PTR)completion_port::file_callback_key);

    return file_access::open_file(shared_from_this(), h);
}

size_t factory::file_read(void * handle, uint64_t fileoffset, array_view<char> dest)
{
    fibers::mutex mtx;
    sync_callback<OVERLAPPED> cb{dataptr, mtx};
    winapi::async_read_file(handle, fileoffset, dest.begin(), dest.size(), &cb.overlapped);
    return cb.wait().value_or(0);
}

size_t factory::file_write(void * handle, uint64_t fileoffset, array_view<const char> src)
{
    fibers::mutex mtx;
    sync_callback<OVERLAPPED> cb{dataptr, mtx};
    winapi::async_write_file(handle, fileoffset, src.begin(), src.size(), &cb.overlapped);
    return cb.wait().value_or(0);
}

}
