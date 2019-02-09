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

enum class completion_port
{
    socket_callback_key = 0,
    acceptor_callback_key,
    file_callback_key,
    close_key
};

struct callback
{
    WSAOVERLAPPED overlapped;
    function<void(std::error_code const&, size_t)> proc;

    explicit callback(function<void(std::error_code const&, size_t)> const& p)
        : proc(p)
    {
        SecureZeroMemory((PVOID)&overlapped, sizeof (WSAOVERLAPPED));
    }
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
            throw exception(code->message());
        }
        return handlsz;
    }
};

template <class OvT>
struct sync_callback
{
    OvT overlapped;

    sync_callback()
    {
        SecureZeroMemory((PVOID)&overlapped, sizeof(OvT));
    }
};

struct socket_sync_callback
    : sync_callback<WSAOVERLAPPED>
    , sync_callback_base
{

};

struct file_callback 
    : sync_callback<OVERLAPPED>
    , sync_callback_base
{
    explicit file_callback(uint64_t fileoffset)
    {
        overlapped.Offset = (DWORD)(fileoffset & 0xFFFFFFFF);
        overlapped.OffsetHigh = (DWORD)(fileoffset >> 32);
    }
};

struct acceptor_handle : public loggable
{
    SOCKET socket;
    LPFN_ACCEPTEX lpfnAcceptEx;
    std::atomic<long> refs_{1};

    acceptor_handle(logger::logger_ptr lgp, SOCKET soc, LPFN_ACCEPTEX fn) 
        : loggable(std::move(lgp)), socket(soc), lpfnAcceptEx(fn)
    {}

    acceptor_handle(acceptor_handle const&) = delete;
    acceptor_handle(acceptor_handle &&) = delete;
    acceptor_handle& operator= (acceptor_handle const&) = delete;
    acceptor_handle& operator= (acceptor_handle &&) = delete;

    void add_ref()
    {
        ++refs_;
    }

    void release()
    {
        if (--refs_ == 0) {
            delete this;
        }
    }

    ~acceptor_handle()
    {
        close();
    }

    void close() noexcept
    {
        if (socket != INVALID_SOCKET && closesocket(socket) == SOCKET_ERROR) {
            DWORD err = WSAGetLastError();
            LOG_ERROR(logger()) << "errror while closing acceptor socket: " << winapi::error_message(err);
        }
        socket = INVALID_SOCKET;
    }
};

struct acceptor_callback
{
    WSAOVERLAPPED overlapped;
    char buff_[2 * sizeof(sockaddr_in) + 32];
    SOCKET socket;
    acceptor_handle * ah_;
    HANDLE iocp_;

    function<void(sonia::io::tcp_acceptor_factory::connection_factory_t const&)> handler_;
    sonia::io::tcp_acceptor_factory::connector_t conn_;

    template <typename HandlerT>
    acceptor_callback(acceptor_handle * ah, HANDLE iocp, HandlerT const& h)
        : ah_(ah), iocp_(iocp), handler_(h)
    {
        SecureZeroMemory((PVOID)&overlapped, sizeof(WSAOVERLAPPED));
        ah_->add_ref();
        prepare();
    }

    void prepare()
    {
        SOCKET accept_sock = winapi::create_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        SCOPE_EXIT([&accept_sock]() { if (accept_sock != INVALID_SOCKET) closesocket(accept_sock); });

        winapi::assign_completion_port((HANDLE)accept_sock, iocp_, (ULONG_PTR)completion_port::socket_callback_key);
        socket = accept_sock;
        handler_([this](array_view<char> buff, sonia::io::tcp_acceptor_factory::connector_t const& conn) {
            this->conn_ = conn;
            this->accept(buff);
        });
        accept_sock = INVALID_SOCKET;
    }

    ~acceptor_callback()
    {
        ah_->release();
    }

    void accept(array_view<char> buff)
    {
        if (buff.size() < sizeof(buff_)) {
            buff = array_view<char>(buff_);
        } else {
            buff.advance_back(-(ptrdiff_t)sizeof(buff_));
        }

        if (!ah_->lpfnAcceptEx(ah_->socket, socket, buff.begin(), (DWORD)buff.size(), sizeof (sockaddr_in) + 16, sizeof (sockaddr_in) + 16, NULL, &overlapped)) {
            DWORD err = WSAGetLastError();
            if (err != WSA_IO_PENDING) {
                throw exception("can't accept socket, error: %1%"_fmt % winapi::error_message(err));
            }
        }
    }
};

struct win_impl_data : public factory::impl_base
{
    friend class factory;

    winapi::wsa_scope ws_;
    HANDLE iocp_;
    
    explicit win_impl_data(shared_ptr<factory> wr, uint32_t thr_cnt)
        : factory::impl_base(std::move(wr)), socket_callbacks_(256), acceptor_callbacks_(16)
    {
        iocp_ = winapi::create_completion_port(thr_cnt);
    }

    ~win_impl_data() override
    {
        CloseHandle(iocp_);
    }

    template <typename ArgT>
    callback * new_socket_callback(ArgT && arg)
    {
        wrapper->on_add_callback();
        try {
            return socket_callbacks_.new_object(std::forward<ArgT>(arg));
        } catch (...) {
            wrapper->on_release_callback();
            throw;
        }
    }

    template <typename ... ArgsT>
    acceptor_callback * new_acceptor_callback(ArgsT&& ... args)
    {
        wrapper->on_add_callback();
        try {
            return acceptor_callbacks_.new_object(std::forward<ArgsT>(args) ...);
        } catch (...) {
            wrapper->on_release_callback();
            throw;
        }
    }

    void delete_socket_callback(callback * cb) noexcept
    {
        socket_callbacks_.delete_object(cb);
        wrapper->on_release_callback();
    }

    void delete_acceptor_callback(acceptor_callback * cb) noexcept
    {
        acceptor_callbacks_.delete_object(cb);
        wrapper->on_release_callback();
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

private:
    object_pool<callback, mutex> socket_callbacks_;
    object_pool<acceptor_callback, mutex> acceptor_callbacks_;
};

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
                    callback* cb = reinterpret_cast<callback*>(overlapped);
                    SCOPE_EXIT([&cb, this]() { if (cb) dataptr->delete_socket_callback(cb); });
                    cb->proc(err, (size_t)bytes);
                    break;
                }
                case completion_port::acceptor_callback_key:
                {
                    acceptor_callback* cb = reinterpret_cast<acceptor_callback*>(overlapped);
                    SCOPE_EXIT([&cb, this]() { if (cb) dataptr->delete_acceptor_callback(cb); });
                    if (!err) {
                        cb->conn_(tcp_socket_access::create_tcp_socket(shared_from_this(), cb->socket), (size_t)bytes);
                        try {
                            cb->prepare();
                            cb = nullptr;
                        } catch (...) {
                            LOG_ERROR(logger()) << "acceptor closed unexpectedly, errror: " << boost::current_exception_diagnostic_information();
                        }
                    } else if (err.value() != ERROR_OPERATION_ABORTED) {
                        LOG_WARN(logger()) << "acceptor closed: " << boost::trim_right_copy(err.message());
                    }
                    break;
                }
                case completion_port::file_callback_key:
                {
                    file_callback* cb = reinterpret_cast<file_callback*>(overlapped);
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
tcp_socket factory::create_tcp_socket(cstring_view address, uint16_t port, tcp_socket_type dt)
{
    SOCKET sock;
    if (winapi::parse_address(address, port, [this, &sock](ADDRINFOW * r)->bool {
        if ((r->ai_family != AF_INET /* && r->ai_family != AF_INET6*/) || r->ai_socktype != SOCK_STREAM || r->ai_protocol != IPPROTO_TCP)
            return false;

        sock = winapi::create_socket(r->ai_family, r->ai_socktype, r->ai_protocol);
        SOCKET pinsock = sock;
        SCOPE_EXIT([&pinsock]() { if (pinsock) closesocket(pinsock); });

        DWORD iResult = connect(sock, r->ai_addr, (int)r->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            DWORD err = WSAGetLastError();
            throw exception("can't connect socket, error: %1%"_fmt % winapi::error_message(err));
        }

        winapi::assign_completion_port((HANDLE)sock, dataptr->iocp_, (ULONG_PTR)completion_port::socket_callback_key);
        pinsock = 0; // detach
        return true;
    })) {
        return tcp_socket_access::create_tcp_socket(shared_from_this(), sock);
    }

    throw exception("can't create socket with address: '%1%' and port: '%2%'"_fmt % address % port);
}

size_t factory::tcp_socket_count(tcp_socket_type) const
{
    throw not_implemented_error("tcp_socket_count");
}

// tcp_socket_service
void factory::tcp_socket_close(intptr_t handle)
{
    if (closesocket((SOCKET)handle) == SOCKET_ERROR) {
        DWORD err = WSAGetLastError();
        LOG_ERROR(logger()) << "errror while closing socket: " << winapi::error_message(err);
    }
}

void factory::tcp_socket_async_read_some(intptr_t soc, void * buff, size_t sz, function<void(std::error_code const&, size_t)> const& ftor)
{
    callback * cb = dataptr->new_socket_callback(ftor);
    SCOPE_EXIT([&cb, this]() { if (cb) dataptr->delete_socket_callback(cb); });

    winapi::async_recv((SOCKET)soc, buff, sz, reinterpret_cast<WSAOVERLAPPED*>(cb));
    cb = nullptr; // detach
}

size_t factory::tcp_socket_read_some(intptr_t soc, void * buff, size_t sz)
{
    sync_callback_base scb;

    callback * cb = dataptr->new_socket_callback([&scb](std::error_code const& err, size_t sz) { scb.on_op(err, sz); });
    SCOPE_EXIT([&cb, this]() { if (cb) dataptr->delete_socket_callback(cb); });

    winapi::async_recv((SOCKET)soc, buff, sz, reinterpret_cast<WSAOVERLAPPED*>(cb));
    cb = nullptr; // detach

    return scb.wait();
}

size_t factory::tcp_socket_write_some(intptr_t soc, void const* buff, size_t sz)
{
    sync_callback_base scb;

    callback * cb = dataptr->new_socket_callback([&scb](std::error_code const& err, size_t sz) { scb.on_op(err, sz); });
    SCOPE_EXIT([&cb, this]() { if (cb) dataptr->delete_socket_callback(cb); });

    winapi::async_send((SOCKET)soc, buff, sz, reinterpret_cast<WSAOVERLAPPED*>(cb));
    cb = nullptr; // detach

    return scb.wait();
}

//size_t factory::tcp_socket_read_some(void * handle, void * buff, size_t sz)
//{
//    WSABUF wsabuf;
//    wsabuf.len = (ULONG)sz;
//    wsabuf.buf = reinterpret_cast<char*>(buff);
//
//
//    DWORD recvBytes, flags = 0;
//    int rc = WSARecv((SOCKET)handle, &wsabuf, 1, &recvBytes, &flags, NULL, NULL);
//    if (rc == SOCKET_ERROR) { 
//        DWORD err = WSAGetLastError();
//        throw exception("can't receive data from socket, error: %1%"_fmt % winapi::error_message(err));
//    }
//    return recvBytes;
//}

//size_t factory::tcp_socket_write_some(void * handle, void const* buff, size_t sz)
//{
//    WSABUF wsabuf;
//    wsabuf.len = (ULONG)sz;
//    wsabuf.buf = const_cast<char*>(reinterpret_cast<char const*>(buff));
//
//    DWORD sentBytes;
//    int rc = WSASend((SOCKET)handle, &wsabuf, 1, &sentBytes, 0, NULL, NULL);
//    if (rc == SOCKET_ERROR) { 
//        DWORD err = WSAGetLastError();
//        throw exception("can't send data to socket, error: %1%"_fmt % winapi::error_message(err));
//    }
//    return sentBytes;
//}

tcp_acceptor factory::create_tcp_acceptor(cstring_view address, uint16_t port, tcp_socket_type dt, function<void(tcp_acceptor_factory::connection_factory_t const&)> const& handler)
{
    SOCKET soc;
    LPFN_ACCEPTEX lpfnAcceptEx;
    if (winapi::parse_address(address, port, [this, &soc, &lpfnAcceptEx](ADDRINFOW * r)->bool {
        if (r->ai_family != AF_INET || r->ai_socktype != SOCK_STREAM || r->ai_protocol != IPPROTO_TCP)
            return false;

        soc = winapi::create_socket(r->ai_family, r->ai_socktype, r->ai_protocol);
        SOCKET pinsock = soc;
        SCOPE_EXIT([&pinsock]() { if (pinsock != INVALID_SOCKET) closesocket(pinsock); });

        int iResult = bind(pinsock, r->ai_addr, (int)r->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            DWORD err = WSAGetLastError();
            throw exception("can't bind socket, error: %1%"_fmt % winapi::error_message(err));
        }

        iResult = listen(pinsock, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            DWORD err = WSAGetLastError();
            throw exception("can't listen to socket, error: %1%"_fmt % winapi::error_message(err));
        }

        lpfnAcceptEx = winapi::get_accept_function(pinsock);

        winapi::assign_completion_port((HANDLE)pinsock, dataptr->iocp_, (ULONG_PTR)completion_port::acceptor_callback_key);

        pinsock = INVALID_SOCKET; // detach
        return true;
        }))
    {
        acceptor_handle * ah = new acceptor_handle(logger(), soc, lpfnAcceptEx);
        acceptor_handle * pin = ah;
        SCOPE_EXIT([&pin]() { delete pin; });

        for (size_t i = 0, tc = thread_count(); i < tc; ++i) {
            dataptr->new_acceptor_callback(ah, dataptr->iocp_, handler);
        }

        pin = nullptr; // detach

        return tcp_acceptor_access::create_tcp_acceptor(shared_from_this(), (void*)ah);
    }

    throw exception("can't create acceptor at the address: '%1%' and port: '%2%'"_fmt % address % port);
}

//void factory::tcp_acceptor_async_accept_and_read_some(void * handle, void * buff, size_t sz, acceptor_functor const& func)
//{
//    acceptor_handle * ah = reinterpret_cast<acceptor_handle*>(handle);
//
//    SOCKET accept_sock = winapi::create_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    SCOPE_EXIT([&accept_sock]() { if (accept_sock != INVALID_SOCKET) closesocket(accept_sock); });
//
//    acceptor_callback * cb = dataptr->new_acceptor_callback(ah, func, accept_sock);
//    SCOPE_EXIT([&cb, this]() { if (cb) dataptr->delete_acceptor_callback(cb); });
//
//    winapi::assign_completion_port((HANDLE)accept_sock, dataptr->iocp_, (ULONG_PTR)completion_port::socket_callback_key);
//
//    cb->accept(buff, sz);
//    
//    //winapi::assign_completion_port((HANDLE)ah->socket, dataptr->iocp_, (ULONG_PTR)completion_port::acceptor_callback_key);
//
//    cb = nullptr; // detach
//    accept_sock = INVALID_SOCKET; // detach
//}

void factory::tcp_acceptor_close(void * handle) noexcept
{
    acceptor_handle * ah = reinterpret_cast<acceptor_handle*>(handle);
    ah->close();
    ah->release();
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
    on_add_callback();
    SCOPE_EXIT([this]() { on_release_callback(); });

    file_callback cb{fileoffset};
    SetLastError(0);

    DWORD sz2read = dest.size() > 0xffffFFFF ? 0xffffFFFF : (DWORD)dest.size();
    DWORD outrsz;
    if (!ReadFile(handle, dest.begin(), sz2read, &outrsz, reinterpret_cast<LPOVERLAPPED>(&cb))) {
        DWORD err = GetLastError();
        if (ERROR_IO_PENDING != err) {
            throw exception("read file error : %1%"_fmt % winapi::error_message(err));
        }
    }

    return cb.wait();
}

size_t factory::file_write(void * handle, uint64_t fileoffset, array_view<const char> src)
{
    on_add_callback();
    SCOPE_EXIT([this]() { on_release_callback(); });

    file_callback cb{fileoffset};
    SetLastError(0);

    DWORD sz2write = src.size() > 0xffffFFFF ? 0xffffFFFF : (DWORD)src.size();
    DWORD wrsz;
    if (!WriteFile((HANDLE)handle, src.cbegin(), sz2write, &wrsz, reinterpret_cast<LPOVERLAPPED>(&cb))) {
        DWORD err = GetLastError();
        if (ERROR_IO_PENDING != err) {
            throw exception("write file error : %1%"_fmt % winapi::error_message(err));
        }
    }

    return cb.wait();
}

}}
