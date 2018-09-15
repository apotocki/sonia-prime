//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/utility/windows.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/object_pool.hpp"
#include "sonia/exceptions.hpp"
#include "factory.hpp"

#include <memory>
#include <iostream>

#include <boost/integer_traits.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/fiber/condition_variable.hpp>

#pragma comment(lib, "ws2_32.lib")

namespace sonia { namespace io {

//using namespace sonia::windows;

namespace winapi = sonia::windows;

enum class completion_port {
    socket_callback_key = 0,
    acceptor_callback_key,
    close_key
};

struct callback {
    WSAOVERLAPPED overlapped;
    function<void(std::error_code const&, size_t)> proc;

    explicit callback(function<void(std::error_code const&, size_t)> const& p)
        : proc(p)
    {
        SecureZeroMemory((PVOID)&overlapped, sizeof (WSAOVERLAPPED));
    }
};

struct acceptor_handle : public loggable {
    SOCKET socket;
    LPFN_ACCEPTEX lpfnAcceptEx;
    std::atomic<long> refs_;

    acceptor_handle(logger::logger_ptr lgp, SOCKET soc, LPFN_ACCEPTEX fn) 
        : loggable(std::move(lgp)), socket(soc), lpfnAcceptEx(fn), refs_(1)
    {}

    acceptor_handle(acceptor_handle const&) = delete;
    acceptor_handle(acceptor_handle &&) = delete;
    acceptor_handle& operator= (acceptor_handle const&) = delete;
    acceptor_handle& operator= (acceptor_handle &&) = delete;

    void add_ref() {
        ++refs_;
    }

    void release() {
        if (--refs_ == 0) {
            delete this;
        }
    }

    ~acceptor_handle() {
        close();
    }

    void close() noexcept {
        if (socket != INVALID_SOCKET && closesocket(socket) == SOCKET_ERROR) {
            DWORD err = WSAGetLastError();
            LOG_ERROR(logger()) << "errror while closing acceptor socket: " << winapi::error_message(err);
        }
        socket = INVALID_SOCKET;
    }
};

struct acceptor_callback {
    WSAOVERLAPPED overlapped;
    char buff_[2 * sizeof(sockaddr_in) + 32];
    tcp_acceptor::acceptor_functor proc;
    SOCKET socket;
    acceptor_handle * ah_;
    acceptor_callback(acceptor_handle * ah, tcp_acceptor::acceptor_functor const& p, SOCKET soc)
        : proc(p), socket(soc), ah_(ah)
    {
        SecureZeroMemory((PVOID)&overlapped, sizeof (WSAOVERLAPPED));
        ah_->add_ref();
    }

    ~acceptor_callback() {
        ah_->release();
    }

    void accept(void * buff, size_t sz) {
        if (sz < 2 * (sizeof(sockaddr_in) + 16)) {
            buff = buff_;
            sz = 0;
        } else {
            sz -= 2 * (sizeof(sockaddr_in) + 16);
        }

        if (!ah_->lpfnAcceptEx(ah_->socket, socket, buff, (DWORD)sz, sizeof (sockaddr_in) + 16, sizeof (sockaddr_in) + 16, NULL, &overlapped)) {
            DWORD err = WSAGetLastError();
            if (err != WSA_IO_PENDING) {
                throw exception("can't accept socket, error: %1%"_fmt % winapi::error_message(err));
            }
        }
    }
};

struct win_impl_data {
    friend class factory;

    static const long qsz_min_value = boost::integer_traits<long>::const_min;

    winapi::wsa_scope ws_;
    HANDLE iocp_;
    shared_ptr<factory> pin_;
    uint32_t thr_cnt_;
    
    boost::fibers::mutex close_mtx_;
    boost::fibers::condition_variable close_cond_;

    win_impl_data(shared_ptr<factory> itself, uint32_t thr_cnt)
        : pin_(std::move(itself)), thr_cnt_(thr_cnt), socket_callbacks_(256), acceptor_callbacks_(16), qsz_(0)
    {
        iocp_ = winapi::create_completion_port(thr_cnt);
    }

    void run() {
        threads_.reserve(thr_cnt_);

        for (size_t i = 0; i < thr_cnt_; ++i) {
            threads_.push_back(thread([this] { pin_->thread_proc(); }));
            winapi::set_thread_name(threads_.back().get_id(), ("%1% factory thread #%2%"_fmt % pin_->name() % i).str().c_str());
        }
    }

    void stop() noexcept {
        std::unique_lock<boost::fibers::mutex> lk(close_mtx_);
        if (qsz_.load() >= 0) {
            if (0 == qsz_.fetch_add(qsz_min_value)) {
                park_threads();
            }
        }
        while (!threads_.empty()) {
            close_cond_.wait(lk);
        }
    }

    ~win_impl_data() {
        CloseHandle(iocp_);
    }

    template <typename ArgT>
    callback * new_socket_callback(ArgT && arg) {
        on_add_callback();
        try {
            return socket_callbacks_.new_object(std::forward<ArgT>(arg));
        } catch (...) {
            on_release_callback();
            throw;
        }
    }

    template <typename ... ArgsT>
    acceptor_callback * new_acceptor_callback(ArgsT&& ... args) {
        on_add_callback();
        try {
            return acceptor_callbacks_.new_object(std::forward<ArgsT>(args) ...);
        } catch (...) {
            on_release_callback();
            throw;
        }
    }

    void delete_socket_callback(callback * cb) noexcept {
        socket_callbacks_.delete_object(cb);
        on_release_callback();
    }

    void delete_acceptor_callback(acceptor_callback * cb) noexcept {
        acceptor_callbacks_.delete_object(cb);
        on_release_callback();
    }

    void on_add_callback() {
        long qszval = qsz_.fetch_add(1);
        if (qszval < 0) {
            if (qsz_min_value != qszval) {
                on_release_callback();
            } else {
                // if qsz_min_value == qszval then initiate_thread_parking was already called, 
                // so we just keep it actual
                --qsz_;
            }
            throw closed_exception("io::factory");
        }
    }

    void on_release_callback() noexcept {
        if ((qsz_min_value + 1) == qsz_.fetch_sub(1)) {
            park_threads();
        }
    }

    void park_threads() noexcept {
        for (uint32_t c = 0; c < thr_cnt_; ++c) {
            try {
                winapi::post_completion_port(iocp_, 0, (ULONG_PTR)completion_port::close_key);
            } catch (std::exception const& e) {
                GLOBAL_LOG_ERROR() << e.what();
            }
        }
        
        thread t([this]() {
            for (thread & t : threads_) {
                t.join();
            }
            unique_lock<boost::fibers::mutex > lk(this->close_mtx_);
            threads_.clear();
            pin_.reset();
            close_cond_.notify_one();
        });
    }

private:
    object_pool<callback, mutex> socket_callbacks_;
    object_pool<acceptor_callback, mutex> acceptor_callbacks_;
    std::atomic<long> qsz_;
    std::vector<thread> threads_;
};

#define dataptr reinterpret_cast<win_impl_data*>(impl_data_)

factory::factory() 
    : impl_data_(nullptr)
{
    
}

factory::~factory() {
    delete dataptr;
}

void factory::open(uint32_t thr_cnt) {
    if (impl_data_) {
        throw internal_error("io factory is already initialized");
    }
    
    impl_data_ = new win_impl_data(shared_from_this(), thr_cnt);
    dataptr->run();
}

void factory::close() {
    if (impl_data_) {
        dataptr->stop();
    }
}

std::string factory::name() const {
    return std::string("windows");
}

void factory::thread_proc() {
    for (;;)
    {
        DWORD bytes;
        ULONG_PTR key = 0;
        LPOVERLAPPED overlapped = nullptr;

        BOOL r = GetQueuedCompletionStatus(dataptr->iocp_, &bytes, &key, &overlapped, INFINITE);
        std::error_code err(r ? 0 : GetLastError(), std::system_category());

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
                    cb->proc(err, (size_t)bytes, tcp_socket_access::create_tcp_socket(shared_from_this(), (void*)cb->socket), [&cb](void * buff, size_t sz) {
                        SOCKET accept_sock = winapi::create_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                        SCOPE_EXIT([&accept_sock]() { if (accept_sock != INVALID_SOCKET) closesocket(accept_sock); });
                        cb->socket = accept_sock;
                        cb->accept(buff, sz);
                        accept_sock = INVALID_SOCKET; // detach
                        cb = nullptr; // detach
                    });
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
    }
}

// tcp_socket_factory
tcp_socket factory::create_tcp_socket(string_view address, uint16_t port, tcp_socket_type dt) {
    SOCKET sock;
    if (winapi::parse_address(address, port, [this, &sock](ADDRINFOW * r)->bool {
        if (r->ai_family != AF_INET || r->ai_socktype != SOCK_STREAM || r->ai_protocol != IPPROTO_TCP)
            return false;

        sock = winapi::create_socket(r->ai_family, r->ai_socktype, r->ai_protocol);
        SOCKET pinsock = sock;
        SCOPE_EXIT([&pinsock]() { if (pinsock) closesocket(pinsock); });

        DWORD iResult = connect(sock, r->ai_addr, (int)r->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            DWORD err = WSAGetLastError();
            if (WSAECONNREFUSED == err) {
                return false;
            }
            throw exception("can't connect socket, error: %1%"_fmt % winapi::error_message(err));
        }

        winapi::assign_completion_port((HANDLE)sock, dataptr->iocp_, (ULONG_PTR)completion_port::socket_callback_key);
        pinsock = 0; // detach
        return true;
    })) {
        return tcp_socket_access::create_tcp_socket(shared_from_this(), (void*)sock);
    }

    throw exception("can't create socket with address: '%1%' and port: '%2%'"_fmt % address % port);
}

size_t factory::tcp_socket_count(tcp_socket_type) const {
    throw not_implemented_error("tcp_socket_count");
}

// tcp_socket_service
void factory::tcp_socket_close(void * handle) {
    if (closesocket((SOCKET)handle) == SOCKET_ERROR) {
        DWORD err = WSAGetLastError();
        LOG_ERROR(logger()) << "errror while closing socket: " << winapi::error_message(err);
    }
}

size_t factory::tcp_socket_read_some(void * handle, void * buff, size_t sz) {
    WSABUF wsabuf;
    wsabuf.len = (ULONG)sz;
    wsabuf.buf = reinterpret_cast<char*>(buff);


    DWORD recvBytes, flags = 0;
    int rc = WSARecv((SOCKET)handle, &wsabuf, 1, &recvBytes, &flags, NULL, NULL);
    if (rc == SOCKET_ERROR) { 
        DWORD err = WSAGetLastError();
        throw exception("can't receive data from socket, error: %1%"_fmt % winapi::error_message(err));
    }
    return recvBytes;
}

void factory::tcp_socket_async_read_some(void * soc, void * buff, size_t sz, function<void(std::error_code const&, size_t)> const& ftor) {
    callback * cb = dataptr->new_socket_callback(ftor);
    SCOPE_EXIT([&cb, this]() { if (cb) dataptr->delete_socket_callback(cb); });

    winapi::async_recv((SOCKET)soc, buff, sz, reinterpret_cast<WSAOVERLAPPED*>(cb));
    cb = nullptr; // detach
}

size_t factory::tcp_socket_write_some(void * handle, void const* buff, size_t sz) {
    WSABUF wsabuf;
    wsabuf.len = (ULONG)sz;
    wsabuf.buf = const_cast<char*>(reinterpret_cast<char const*>(buff));

    DWORD sentBytes;
    int rc = WSASend((SOCKET)handle, &wsabuf, 1, &sentBytes, 0, NULL, NULL);
    if (rc == SOCKET_ERROR) { 
        DWORD err = WSAGetLastError();
        throw exception("can't send data to socket, error: %1%"_fmt % winapi::error_message(err));
    }
    return sentBytes;
}

tcp_acceptor factory::create_tcp_acceptor(string_view address, uint16_t port, tcp_socket_type dt) {
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
    })) {
        return tcp_acceptor_access::create_tcp_acceptor(shared_from_this(), (void*)new acceptor_handle(logger(), soc, lpfnAcceptEx));
    }

    throw exception("can't create acceptor at the address: '%1%' and port: '%2%'"_fmt % address % port);
}

void factory::tcp_acceptor_async_accept_and_read_some(void * handle, void * buff, size_t sz, acceptor_functor const& func) {
    acceptor_handle * ah = reinterpret_cast<acceptor_handle*>(handle);

    SOCKET accept_sock = winapi::create_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SCOPE_EXIT([&accept_sock]() { if (accept_sock != INVALID_SOCKET) closesocket(accept_sock); });

    acceptor_callback * cb = dataptr->new_acceptor_callback(ah, func, accept_sock);
    SCOPE_EXIT([&cb, this]() { if (cb) dataptr->delete_acceptor_callback(cb); });

    cb->accept(buff, sz);
    
    //winapi::assign_completion_port((HANDLE)ah->socket, dataptr->iocp_, (ULONG_PTR)completion_port::acceptor_callback_key);

    cb = nullptr; // detach
    accept_sock = INVALID_SOCKET; // detach
}

void factory::tcp_acceptor_close(void * handle) {
    acceptor_handle * ah = reinterpret_cast<acceptor_handle*>(handle);
    ah->close();
    ah->release();
}

}}
