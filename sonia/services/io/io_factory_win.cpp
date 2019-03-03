//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/optional.hpp"

#include "sonia/utility/windows.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/object_pool.hpp"
#include "sonia/exceptions.hpp"
#include "factory.hpp"

#include <cstddef>
#include <memory>
#include <iostream>

#include <boost/throw_exception.hpp>
#include <boost/integer_traits.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/fiber/condition_variable.hpp>
#include <boost/algorithm/string/trim.hpp>

#pragma comment(lib, "ws2_32.lib")

namespace sonia { namespace io {

namespace winapi = sonia::windows;

using tcp_socket_service_type = tcp_socket_service<socket_traits>;
using udp_socket_service_type = udp_socket_service<socket_traits>;

struct win_impl_data;

enum class completion_port
{
    socket_callback_key = 0,
    file_callback_key,
    close_key
};

struct win_shared_handle : shared_handle<socket_handle_traits>
{
    using base_type = shared_handle<socket_handle_traits>;
    using base_type::base_type;

    SOCKET socket() const noexcept { return handle; }
};

using tcp_scoped_handle = scoped_handle<tcp_socket_service_type*, win_shared_handle>;
using udp_scoped_handle = scoped_handle<udp_socket_service_type*, win_shared_handle>;

template <typename OvT>
struct callback
{
    OvT overlapped;
    std::atomic<long> refs{1};

    explicit callback(win_impl_data *, bool pinned = false);
    virtual ~callback() {}

    virtual void on_op(std::error_code const&, size_t) = 0;

    static callback * get(OvT * pov)
    {
        return reinterpret_cast<callback*>(reinterpret_cast<char*>(pov) - offsetof(callback, overlapped));
    }

    void release(win_impl_data * impl);

    virtual void free(win_impl_data * impl) = 0;
};

struct sync_callback_base
{
    size_t handlsz;
    optional<std::error_code> code;
    fibers::mutex mtx;
    fibers::condition_variable_any cnd;

    void on_op(std::error_code const& err, size_t sz)
    {
        handlsz = sz;
        unique_lock lck(mtx);
        code = err;
        cnd.notify_one();
    }

    size_t wait()
    {
        unique_lock lck(mtx);
        cnd.wait(lck, [this] { return !!code; });
        if (code && *code) {
            //throw exception(code->message());
            return 0;
        }
        return handlsz;
    }
};

template <typename OvT>
struct sync_callback
    : callback<OvT>
    , sync_callback_base
{
    using base_type = callback<OvT>;
    win_impl_data * impl_;

    explicit sync_callback(win_impl_data * p)
        : base_type(p, true), impl_(p)
    {}

    ~sync_callback();
    
    void on_op(std::error_code const& err, size_t sz) override
    {
        sync_callback_base::on_op(err, sz);
    }

    void free(win_impl_data * impl)
    {
        BOOST_ASSERT_MSG(false, "must be never called");
    }
};

struct async_callback : callback<WSAOVERLAPPED>
{
    using base_type = callback<WSAOVERLAPPED>;
    function<void(std::error_code const&, size_t)> proc;

    explicit async_callback(win_impl_data * impl, function<void(std::error_code const&, size_t)> const& procval)
        : base_type(impl), proc(procval)
    {}

    void on_op(std::error_code const& err, size_t sz) override
    {
        proc(err, sz);
    }

    void free(win_impl_data * impl) override;
};

struct acceptor_callback : callback<WSAOVERLAPPED>
{
    using base_type = callback<WSAOVERLAPPED>;

    char buff_[2 * sizeof(sockaddr_in) + 32];
    SOCKET socket;
    win_shared_handle * ah_;
    win_impl_data * impl_;
    LPFN_ACCEPTEX lpfnAcceptEx;

    function<void(tcp_connection_handler_t<socket_traits> const&)> handler_;
    tcp_connector_t<socket_traits> conn_;

    template <typename HandlerT>
    acceptor_callback(win_impl_data * impl, win_shared_handle * ah, LPFN_ACCEPTEX fn, HandlerT const& h)
        : base_type(impl), ah_(ah), impl_(impl), lpfnAcceptEx(fn), handler_(h)
    {
        prepare();
        ah_->add_ref();
    }

    ~acceptor_callback();
    
    void accept(array_view<char> buff)
    {
        if (buff.size() < sizeof(buff_)) {
            buff = array_view<char>(buff_);
        } else {
            buff.advance_back(-(ptrdiff_t)sizeof(buff_));
        }

        SecureZeroMemory((PVOID)&overlapped, sizeof(WSAOVERLAPPED));

        if (!lpfnAcceptEx(ah_->socket(), socket, buff.begin(), (DWORD)buff.size(), sizeof (sockaddr_in) + 16, sizeof (sockaddr_in) + 16, NULL, &overlapped)) {
            DWORD err = WSAGetLastError();
            if (err != WSA_IO_PENDING) {
                throw exception("can't accept socket, error: %1%"_fmt % winapi::error_message(err));
            }
        }
    }

    void prepare();
    void on_op(std::error_code const& err, size_t bytes) override;
    void free(win_impl_data * impl) override;
};

struct listener_callback : callback<WSAOVERLAPPED>
{
    using base_type = callback<WSAOVERLAPPED>;

    win_shared_handle * sh_;
    win_impl_data * impl_;
    function<void(udp_connection_handler_t<socket_traits> const&)> handler_;
    udp_connector_t<socket_traits> conn_;
    socket_address sender_addr_;
    
    template <typename HandlerT>
    listener_callback(win_impl_data * impl, win_shared_handle * sh, HandlerT const& h)
        : base_type(impl), sh_(sh), impl_(impl), handler_(h)
    {
        prepare();
        sh_->add_ref();
    }

    ~listener_callback();

    void prepare();

    void on_op(std::error_code const& err, size_t sz) override;

    void free(win_impl_data * impl) override;
};

struct win_impl_data : public factory::impl_base
{
    friend class factory;

    winapi::wsa_scope ws_;
    HANDLE iocp_;
    
    explicit win_impl_data(shared_ptr<factory> wr, uint32_t thr_cnt)
        : factory::impl_base(std::move(wr)), handles_(256), async_callbacks_(256), acceptor_callbacks_(16), listener_callbacks_(16)
    {
        iocp_ = winapi::create_completion_port(thr_cnt);
    }

    ~win_impl_data() override
    {
        CloseHandle(iocp_);
    }

    template <typename ArgT>
    win_shared_handle * new_socket_handle(ArgT && arg)
    {
        return handles_.new_object(std::forward<ArgT>(arg));
    }

    template <typename ArgT>
    async_callback * new_async_callback(ArgT && arg)
    {
        return async_callbacks_.new_object(this, std::forward<ArgT>(arg));
    }

    template <typename ... ArgsT>
    acceptor_callback * new_acceptor_callback(ArgsT&& ... args)
    {
        return acceptor_callbacks_.new_object(this, std::forward<ArgsT>(args) ...);
    }

    template <typename ... ArgsT>
    listener_callback * new_listener_callback(ArgsT&& ... args)
    {
        return listener_callbacks_.new_object(this, std::forward<ArgsT>(args) ...);
    }

    void delete_socket_handle(win_shared_handle * h) noexcept
    {
        handles_.delete_object(h);
    }

    void delete_async_callback(async_callback * cb) noexcept
    {
        async_callbacks_.delete_object(cb);
    }

    void delete_acceptor_callback(acceptor_callback * cb) noexcept
    {
        acceptor_callbacks_.delete_object(cb);
    }

    void delete_listener_callback(listener_callback * cb) noexcept
    {
        listener_callbacks_.delete_object(cb);
    }

    void park_threads() noexcept override
    {
        size_t tc = wrapper->thread_count();
        for (uint32_t c = 0; c < tc; ++c) {
            try {
                winapi::post_completion_port(iocp_, 0, (ULONG_PTR)completion_port::close_key);
            } catch (std::exception const& e) {
                GLOBAL_LOG_ERROR() << e.what();
            }
        }
        
        thread([wr = std::move(wrapper)]() { wr->join_threads(); }).detach();
    }

    HANDLE iocp() const { return iocp_; }

private:
    object_pool<win_shared_handle, mutex> handles_;
    object_pool<async_callback, mutex> async_callbacks_;
    object_pool<acceptor_callback, mutex> acceptor_callbacks_;
    object_pool<listener_callback, mutex> listener_callbacks_;
};

template <typename OvT>
callback<OvT>::callback(win_impl_data * p, bool pinned)
{
    BOOST_ASSERT(this == get(&overlapped));
    SecureZeroMemory((PVOID)&overlapped, sizeof(OvT));
    p->wrapper->on_add_callback();
    if (pinned) ++refs;
}

template <typename OvT>
void callback<OvT>::release(win_impl_data * impl)
{
    if (1 == refs.fetch_sub(1)) {
        impl->wrapper->on_release_callback();
        free(impl);
    }
}

template <typename OvT>
sync_callback<OvT>::~sync_callback()
{
    impl_->wrapper->on_release_callback();
}

void async_callback::free(win_impl_data * impl)
{
    impl->delete_async_callback(this);
}

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

void acceptor_callback::free(win_impl_data * impl)
{
    impl->delete_acceptor_callback(this);
}

acceptor_callback::~acceptor_callback()
{
    ah_->release((tcp_socket_service_type*) impl_->wrapper.get());
}

void listener_callback::prepare()
{
    handler_([this](array_view<char> buff, udp_connector_t<socket_traits> const& conn) {
        this->conn_ = conn;
        sender_addr_.reset();
        int sz = sender_addr_.size();
        winapi::async_recvfrom(sh_->socket(), buff.begin(), buff.size(), sender_addr_.addr<SOCKADDR>(), &sz, &overlapped);
    });
}

void listener_callback::on_op(std::error_code const& err, size_t bytes)
{
    if (!err) {
        conn_(bytes, sender_addr_, udp_socket_access::create_udp_socket<socket_traits::weak_traits_type>(impl_->wrapper, sh_));
        try {
            prepare();
            ++refs;
        } catch (...) {
            GLOBAL_LOG_ERROR() << "udp listener closed unexpectedly, errror: " << boost::current_exception_diagnostic_information();
        }
    } else if (err.value() != ERROR_OPERATION_ABORTED) {
        GLOBAL_LOG_WARN() << "listener socket closed: " << boost::trim_right_copy(err.message());
    }
}

void listener_callback::free(win_impl_data * impl)
{
    impl->delete_listener_callback(this);
}

listener_callback::~listener_callback()
{
    sh_->release((udp_socket_service_type*)impl_->wrapper.get());
}

#define dataptr static_cast<win_impl_data*>(impl_data_.get())

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
    
    impl_data_.reset(new win_impl_data(shared_from_this(), thr_cnt));
}

std::string factory::name() const
{
    return std::string("windows");
}

void factory::thread_proc()
{
    for (;;)
    {
        DWORD bytes;
        ULONG_PTR key = 0;
        LPOVERLAPPED overlapped = nullptr;

        BOOL r = GetQueuedCompletionStatus(dataptr->iocp_, &bytes, &key, &overlapped, INFINITE);
        std::error_code err(r ? 0 : GetLastError(), std::system_category());

        try {
            if (overlapped) {
                switch ((completion_port)key) {
                case completion_port::socket_callback_key:
                {
                    auto * cb = callback<WSAOVERLAPPED>::get(overlapped);
                    SCOPE_EXIT([cb, this]() { cb->release(dataptr); });
                    cb->on_op(err, (size_t)bytes);
                    break;
                }
                case completion_port::file_callback_key:
                {
                    auto* cb = callback<OVERLAPPED>::get(overlapped);
                    SCOPE_EXIT([&cb, this]() { cb->release(dataptr); });
                    cb->on_op(err, (size_t)bytes);
                    break;
                }
                default:
                    LOG_ERROR(logger()) << "completion port unknown key: " << (int)key;
                }
            } else {
                if (!r) {
                    DWORD err = GetLastError();
                    LOG_ERROR(logger()) << "completion port errror: " << winapi::error_message(err);
                } else if (key == (ULONG_PTR)completion_port::close_key) {
                    break;
                } else {
                    LOG_ERROR(logger()) << "completion port unknown key: " << (int)key;
                }
            }
        } catch (std::exception const& e) {
            LOG_ERROR(logger()) << e.what();
        }
    }
}

// tcp_socket_factory
tcp_socket factory::create_tcp_socket(tcp_socket_type dt)
{
    SOCKET sock = winapi::create_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKET pinsock = sock;
    SCOPE_EXIT([&pinsock]() { if (pinsock != INVALID_SOCKET) closesocket(pinsock); });

    winapi::assign_completion_port((HANDLE)pinsock, dataptr->iocp_, (ULONG_PTR)completion_port::socket_callback_key);
    win_shared_handle * wsh = dataptr->new_socket_handle(sock);
    pinsock = INVALID_SOCKET; // detach
    return tcp_socket_access::create_tcp_socket<socket_traits>(shared_from_this(), wsh);
}

tcp_socket factory::create_tcp_socket(cstring_view address, uint16_t port, tcp_socket_type dt)
{
    SOCKET sock;
    if (winapi::parse_address(SOCK_STREAM, IPPROTO_TCP, address, port, [this, &sock](ADDRINFOW * r)->bool {
        if ((r->ai_family != AF_INET /* && r->ai_family != AF_INET6*/) || r->ai_socktype != SOCK_STREAM || r->ai_protocol != IPPROTO_TCP)
            return false;

        sock = winapi::create_socket(r->ai_family, r->ai_socktype, r->ai_protocol);
        SOCKET pinsock = sock;
        SCOPE_EXIT([&pinsock]() { if (pinsock != INVALID_SOCKET) closesocket(pinsock); });

        DWORD iResult = connect(sock, r->ai_addr, (int)r->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            DWORD err = WSAGetLastError();
            throw exception("can't connect socket, error: %1%"_fmt % winapi::error_message(err));
        }

        winapi::assign_completion_port((HANDLE)sock, dataptr->iocp_, (ULONG_PTR)completion_port::socket_callback_key);
        pinsock = INVALID_SOCKET; // detach
        return true;
    })) {
        return tcp_socket_access::create_tcp_socket<socket_traits>(shared_from_this(), dataptr->new_socket_handle(sock));
    }

    throw exception("can't create socket with address: '%1%' and port: '%2%'"_fmt % address % port);
}

// tcp_socket_service
void factory::tcp_socket_bind(tcp_handle_type handle, cstring_view address, uint16_t port)
{
    tcp_scoped_handle sh{this, handle};

    if (!winapi::parse_address(SOCK_DGRAM, IPPROTO_UDP, address, port, [&sh](ADDRINFOW * r)->bool {
        winapi::bind_socket(sh->socket(), r->ai_addr, (int)r->ai_addrlen);
        return true;
    })) {
        throw exception("can't bind socket at the address: '%1%' and port: '%2%'"_fmt % address % port);
    }
}

void factory::tcp_socket_listen(tcp_handle_type handle, function<void(tcp_connection_handler_type const&)> const& handler)
{
    tcp_scoped_handle sh{this, handle};

    int iResult = ::listen(sh->socket(), SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        DWORD err = WSAGetLastError();
        throw exception("can't listen to socket, error: %1%"_fmt % winapi::error_message(err));
    }

    LPFN_ACCEPTEX lpfnAcceptEx = winapi::get_accept_function(sh->socket());

    for (size_t i = 0, tc = thread_count(); i < tc; ++i) {
        dataptr->new_acceptor_callback(sh.get(), lpfnAcceptEx, handler);
    }
}

void factory::tcp_socket_async_read_some(tcp_handle_type handle, void * buff, size_t sz, function<void(std::error_code const&, size_t)> const& ftor)
{
    tcp_scoped_handle sh{this, handle};
    async_callback * cb = dataptr->new_async_callback(ftor);
    SCOPE_EXIT([&cb, this]() { if (cb) cb->release(dataptr); });

    winapi::async_recv(sh->socket(), buff, sz, &cb->overlapped);
    cb = nullptr; // detach
}

size_t factory::tcp_socket_read_some(tcp_handle_type handle, void * buff, size_t sz)
{
    tcp_scoped_handle sh{this, handle};
    sync_callback<WSAOVERLAPPED> scb{dataptr};
    winapi::async_recv(sh->socket(), buff, sz, &scb.overlapped);
    return scb.wait();
}

size_t factory::tcp_socket_write_some(tcp_handle_type handle, void const* buff, size_t sz)
{
    tcp_scoped_handle sh{this, handle};
    sync_callback<WSAOVERLAPPED> scb{dataptr};
    winapi::async_send(sh->socket(), buff, sz, &scb.overlapped);
    return scb.wait();
}

void factory::tcp_socket_on_close(tcp_handle_type)
{}

void factory::free_handle(identity<tcp_socket_service_type>, tcp_handle_type h)
{
    dataptr->delete_socket_handle(static_cast<win_shared_handle*>(h));
}

udp_socket factory::create_udp_socket()
{
    SOCKET sock = winapi::create_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    SOCKET pinsock = sock;
    SCOPE_EXIT([&pinsock]() { if (pinsock != INVALID_SOCKET) closesocket(pinsock); });

    winapi::assign_completion_port((HANDLE)sock, dataptr->iocp_, (ULONG_PTR)completion_port::socket_callback_key);
    win_shared_handle * wsh = dataptr->new_socket_handle(sock);
    pinsock = INVALID_SOCKET;
    return udp_socket_access::create_udp_socket<socket_traits>(shared_from_this(), wsh);
}

void factory::udp_socket_bind(udp_handle_type handle, cstring_view address, uint16_t port)
{
    udp_scoped_handle sh{this, handle};

    if (!winapi::parse_address(SOCK_DGRAM, IPPROTO_UDP, address, port, [&sh](ADDRINFOW * r)->bool {
        winapi::bind_socket(sh->socket(), r->ai_addr, (int)r->ai_addrlen);
        return true;
    })) {
        throw exception("can't bind socket at the address: '%1%' and port: '%2%'"_fmt % address % port);
    }
}

void factory::udp_socket_listen(udp_handle_type handle, function<void(udp_connection_handler_type const&)> const& handler)
{
    udp_scoped_handle sh{this, handle};

    for (size_t i = 0, tc = thread_count(); i < tc; ++i) {
        dataptr->new_listener_callback(sh.get(), handler);
    }
}

size_t factory::udp_socket_read_some(udp_handle_type handle, void * buff, size_t sz)
{
    udp_scoped_handle sh{this, handle};
    sync_callback<WSAOVERLAPPED> scb{dataptr};
    winapi::async_recv(sh->socket(), buff, sz, &scb.overlapped);
    return scb.wait();
}

size_t factory::udp_socket_write_some(udp_handle_type handle, socket_address const& address, void const* buff, size_t sz)
{
    udp_scoped_handle sh{this, handle};
    sync_callback<WSAOVERLAPPED> scb{dataptr};
    auto[ai_addr, ai_len] = address.in_addr();
    winapi::async_send_to(sh->socket(), (sockaddr*)ai_addr, (int)ai_len, buff, sz, &scb.overlapped);
    return scb.wait();
}

size_t factory::udp_socket_write_some(udp_handle_type handle, cstring_view address, uint16_t port, void const* buff, size_t sz)
{
    size_t wrsz;
    auto params = std::tie(handle, wrsz, buff, sz);
    if (!winapi::parse_address(SOCK_DGRAM, IPPROTO_UDP, address, port, [this, &params](ADDRINFOW * r)->bool {
        auto&[handle, wrsz, buff, sz] = params;
        udp_scoped_handle sh{this, handle};
        sync_callback<WSAOVERLAPPED> scb{dataptr};
        winapi::async_send_to(sh->socket(), r->ai_addr, (int)r->ai_addrlen, buff, sz, &scb.overlapped);
        wrsz = scb.wait();
        return true;
    })) {
        throw exception("can't send data to the address: '%1%' and port: '%2%'"_fmt % address % port);
    }
    return wrsz;
}

void factory::udp_socket_on_close(udp_handle_type)
{}

void factory::free_handle(identity<udp_socket_service_type>, udp_handle_type h)
{
    dataptr->delete_socket_handle(static_cast<win_shared_handle*>(h));
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
    sync_callback<OVERLAPPED> cb{dataptr};
    winapi::async_read_file(handle, fileoffset, dest.begin(), dest.size(), &cb.overlapped);
    return cb.wait();
}

size_t factory::file_write(void * handle, uint64_t fileoffset, array_view<const char> src)
{
    sync_callback<OVERLAPPED> cb{dataptr};
    winapi::async_write_file(handle, fileoffset, src.begin(), src.size(), &cb.overlapped);
    return cb.wait();
}

}}
