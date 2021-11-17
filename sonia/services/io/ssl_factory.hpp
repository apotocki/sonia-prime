//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#if OPENSSL_VERSION_NUMBER < 0x10100000L
#   error inappropriate OpenSSL version
#endif

#include <boost/variant.hpp>

#include "sonia/shared_ptr.hpp"
#include "sonia/array_view.hpp"
#include "sonia/logger/loggable.hpp"

#include "sonia/utility/object_pool.hpp"

#include "tcp_socket.hpp"
#include "sockets.hpp"
#include "ssl_configuration.hpp"

namespace sonia::io {

struct openssl_x509
{
    BIO *bio;
    X509 *certificate;

    openssl_x509(array_view<char> data, x509_format_type);
    openssl_x509(openssl_x509 const&) = delete;
    openssl_x509& operator= (openssl_x509 const&) = delete;
    ~openssl_x509();
};

struct openssl_key
{
    BIO *bio;
    boost::variant<EVP_PKEY*, RSA*> key;

    openssl_key(array_view<char> data, key_format_type, optional<std::string> const& password);
    openssl_key(openssl_key const&) = delete;
    openssl_key& operator= (openssl_key const&) = delete;

    ~openssl_key();
};

struct openssl_context
{
    SSL_CTX *ctx;

    explicit openssl_context(const SSL_METHOD * method);

    openssl_context(openssl_context const&) = delete;
    openssl_context& operator=(openssl_context const&) = delete;

    ~openssl_context();

    void use(shared_ptr<openssl_x509> cert);
    void use(shared_ptr<openssl_key> pk);
};

class ssl_factory
    : public tcp_server_socket_factory_type
    , public tcp_server_socket_service_type
    , public tcp_socket_factory_type
    , public tcp_socket_service_type
    , public enable_shared_from_this<ssl_factory>
    , public virtual loggable
{
    using tcp_handle_type = tcp_socket_service_type::tcp_handle_type;

public:
    void open(ssl_configuration const& cf);

    struct ssl_acceptor_descriptor : shared_handle<io::socket_handle_traits>
    {
        tcp_server_socket sock;

        explicit ssl_acceptor_descriptor(tcp_server_socket s) : sock(std::move(s)) {}
    };

    struct ssl_descriptor : shared_handle<io::socket_handle_traits>
    {
        tcp_socket sock;
        SSL * ssl_;
        BIO* net_bio_;
        size_t read_buffsz_{4096};
        size_t write_buffsz_{4096};
        std::vector<char> net_write_buff_; // also the read buffer for sock
        array_view<char> net_write_data_; // actual data to write into net_bio_
        std::vector<char> net_read_buff_;
        fibers::mutex read_mtx_, write_mtx_;
        std::atomic_bool shutdowned_{true};

        ssl_descriptor(SSL_CTX * ctx);
        ~ssl_descriptor() override;

        void handle_async_perform_result(int ssl_error, size_t pending_sz);
        size_t perform(int (ssl_descriptor::* op)(array_view<char>), array_view<char> = {});
        
        void shutdown();

        int do_accept(array_view<char>);
        int do_connect(array_view<char>);
        int do_read(array_view<char>);
        int do_write(array_view<char>);

        void write_from_bio_to_sock(size_t sz);
    };

    // tcp_server_socket_factory
    tcp_server_socket create_server_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt) override;

    // tcp_socket_factory
    tcp_socket create_connected_tcp_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt) override;
    
    // tcp_socket_service_type
    fibers::future<io::tcp_socket> tcp_server_socket_accept(tcp_handle_type) override final;
    size_t tcp_server_socket_accepting_count(tcp_handle_type) const override final;
    void close_handle(identity<tcp_server_socket_service_type>, tcp_handle_type) noexcept override final;
    void release_handle(identity<tcp_server_socket_service_type>, tcp_handle_type h) noexcept override final;
    void free_handle(identity<tcp_server_socket_service_type>, tcp_handle_type) noexcept override final;

    expected<size_t, std::exception_ptr> tcp_socket_read_some(tcp_handle_type, void * buff, size_t sz) noexcept override final;
    expected<size_t, std::exception_ptr> tcp_socket_write_some(tcp_handle_type, void const* buff, size_t sz) noexcept override final;
    void close_handle(identity<tcp_socket_service_type>, tcp_handle_type) noexcept override final;
    void release_handle(identity<tcp_socket_service_type>, tcp_handle_type h) noexcept override final;
    void free_handle(identity<tcp_socket_service_type>, tcp_handle_type) noexcept override final;
    sal::socket_handle system_handle(tcp_handle_type) noexcept override final;

protected:
    shared_ptr<tcp_socket_factory_type> base_socket_factory_;
    shared_ptr<tcp_server_socket_factory_type> base_server_socket_factory_;

private:
    shared_ptr<openssl_context> ctx_;
    object_pool<ssl_descriptor, spin_mutex> pool_{32};
    object_pool<ssl_acceptor_descriptor, spin_mutex> acceptor_pool_{32};
};

}
