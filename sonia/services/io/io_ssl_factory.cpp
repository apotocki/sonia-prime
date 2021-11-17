//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "ssl_factory.hpp"

#include <sstream>
#include <fstream>

#include "sonia/singleton.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/filesystem.hpp"
#include "sonia/utility/scope_exit.hpp"

#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")

namespace sonia::io {

int ssl_errstr_cb(const char *str, size_t len, void *u)
{
    auto * ss = reinterpret_cast<std::ostringstream*>(u);
    (*ss) << string_view(str, len);
    return 1;
}

std::string get_ssl_error()
{
    std::ostringstream errss;
    ERR_print_errors_cb(&ssl_errstr_cb, &errss);
    return std::move(errss.str());
}

openssl_context::openssl_context(const SSL_METHOD * method)
{
    ctx = SSL_CTX_new(method);
    if (!ctx) {
        auto errstr = get_ssl_error();
        throw exception("can't create ssl context, error: %1%"_fmt % errstr);
    }

    SSL_CTX_set_ecdh_auto(ctx, 1);
}

openssl_context::~openssl_context()
{
    SSL_CTX_free(ctx);
}

void openssl_context::use(shared_ptr<openssl_x509> cert)
{
    if (SSL_CTX_use_certificate(ctx, cert->certificate) <= 0) {
        auto errstr = get_ssl_error();
        throw exception("can't use certificate on context, error: %1%"_fmt % errstr);
    }
}

void openssl_context::use(shared_ptr<openssl_key> pk)
{
    auto errh = [] {
        const auto errstr = get_ssl_error();
        throw exception("can't use pk on context, error: %1%"_fmt % errstr);
    };

    EVP_PKEY** pkval = boost::get<EVP_PKEY*>(&pk->key);
    if (pkval) {
        if (SSL_CTX_use_PrivateKey(ctx, *pkval) == 1) return;
        errh();
    }
    
    RSA** rsapkval = boost::get<RSA*>(&pk->key);
    if (rsapkval) {
        if (SSL_CTX_use_RSAPrivateKey(ctx, *rsapkval) == 1) return;
        errh();
    }
    
    THROW_INTERNAL_ERROR("unknown private key type");
}

openssl_x509::openssl_x509(array_view<char> data, x509_format_type ft)
    : bio(BIO_new(BIO_s_mem()))
{
    BIO *biopin = bio;
    SCOPE_EXIT([&biopin]{if (biopin) BIO_free_all(biopin); });

    int r = BIO_write(bio, data.begin(), static_cast<int>(data.size()));
    if (!r) {
        const auto errstr = get_ssl_error();
        throw exception("can't load x509, error: %1%"_fmt % errstr);
    }
    switch (ft) {
        case x509_format_type::DERFORMAT:
            certificate = d2i_X509_bio(bio, NULL);
        break;
        case  x509_format_type::PEMFORMAT:
            certificate = PEM_read_bio_X509(bio, NULL, 0, NULL);
        break;
        default:
            THROW_INTERNAL_ERROR("undefined certificate format type: %1%"_fmt % (int)ft);
    }
    if (!certificate) {
        const auto errstr = get_ssl_error();
        throw exception("can't read x509, error: %1%"_fmt % errstr);
    }

    // unpin
    biopin = nullptr;
}


openssl_x509::~openssl_x509()
{
    X509_free(certificate);
    BIO_free_all(bio);
}

struct key_error_visitor
{
    template <typename T>
    void operator()(T* p) const
    {
        if (!p) {
            const auto errstr = get_ssl_error();
            throw exception("can't read a key, error: %1%"_fmt % errstr);
        }
    }
};

struct key_free_visitor
{
    void operator()(RSA* p) const
    {
        RSA_free(p);
    }

    void operator()(EVP_PKEY* p) const
    {
        EVP_PKEY_free(p);
    }
};

openssl_key::openssl_key(array_view<char> data, key_format_type ft, optional<std::string> const& password)
    : bio(BIO_new(BIO_s_mem()))
{
    BIO *biopin = bio;
    SCOPE_EXIT([&biopin]{if (biopin) BIO_free_all(biopin); });

    int r = BIO_write(bio, data.begin(), static_cast<int>(data.size()));
    if (!r) {
        const auto errstr = get_ssl_error();
        throw exception("can't load a key, error: %1%"_fmt % errstr);
    }

    const char dummy_passwd[] = "";
    void* passwd = const_cast<char*>(password? password->c_str() : dummy_passwd);

    switch (ft) {
        case key_format_type::PRIVATE_RSA:
            key = PEM_read_bio_RSAPrivateKey(bio, NULL, 0, passwd);
            break;
        case key_format_type::PRIVATE_EVP:
            key = PEM_read_bio_PrivateKey(bio, NULL, 0, passwd);
            break;
        default:
            THROW_INTERNAL_ERROR("undefined key format type: %1%"_fmt % (int)ft);
    }

    boost::apply_visitor(key_error_visitor(), key);
    // unpin
    biopin = nullptr;
}

openssl_key::~openssl_key()
{
    boost::apply_visitor(key_free_visitor(), key);
    BIO_free_all(bio);
}

struct openssl_support : singleton
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    openssl_support()
    {
        SSL_load_error_strings();
        //ERR_load_crypto_strings();
        OpenSSL_add_ssl_algorithms();

        method = TLS_method();
    }

    shared_ptr<openssl_context> create_context()
    {
        return make_shared<openssl_context>(method);
    }

    ~openssl_support()
    {
        EVP_cleanup();
    }

private:

};

std::vector<char> load_media(std::string const& value, ssl_media_type t)
{
    std::vector<char> certificate;
    if (t == ssl_media_type::FILE) {
        std::ifstream file(value.c_str());
        if (file.fail()) {
            throw exception("can't open the certificate file %1%"_fmt % fs::absolute(value));
        }
        std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(certificate));
    } else if (t == ssl_media_type::STRING) {
        std::copy(value.begin(), value.end(), std::back_inserter(certificate));
    } else {
        THROW_INTERNAL_ERROR("underined ssl_certificate_media_type: %1%"_fmt % (int)t);
    }

    return certificate;
}

void ssl_factory::open(ssl_configuration const& cf)
{
    ctx_ = as_singleton<openssl_support>()->create_context();

    std::vector<char> certificate = load_media(cf.certificate, cf.certificate_media_type);
    auto cert = make_shared<openssl_x509>(to_array_view(certificate), cf.certificate_type);
    ctx_->use(cert);

    std::vector<char> private_key = load_media(cf.private_key, cf.private_key_media_type);
    auto pk = make_shared<openssl_key>(to_array_view(private_key), cf.private_key_type, cf.private_key_password);
    ctx_->use(pk);
}

ssl_factory::ssl_descriptor::ssl_descriptor(SSL_CTX * ctx)
{
    net_write_buff_.resize(write_buffsz_);
    net_read_buff_.reserve(read_buffsz_);
    ssl_ = SSL_new(ctx);
    if (!ssl_) {
        throw exception("can't create SSL object, error: %1%"_fmt % get_ssl_error());
    }
    SSL_set_mode(ssl_, SSL_MODE_ENABLE_PARTIAL_WRITE);
    SSL_set_mode(ssl_, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
    BIO* int_bio = 0;
    if (!BIO_new_bio_pair(&int_bio, 0, &net_bio_, 0)) {
        throw exception("can't create new BIO pair, error: %1%"_fmt % get_ssl_error());
    }
    SSL_set_bio(ssl_, int_bio, int_bio);
}

ssl_factory::ssl_descriptor::~ssl_descriptor()
{
    SSL_free(ssl_);
    BIO_free(net_bio_);
}

void ssl_factory::ssl_descriptor::write_from_bio_to_sock(size_t sz)
{
    unique_lock lck{write_mtx_, std::try_to_lock};
    if (lck) {
        net_read_buff_.resize(sz);
        int rdsz = BIO_read(net_bio_, &net_read_buff_.front(), (int)net_read_buff_.size());
        if (rdsz <= 0) throw exception("can't read from BIO object, error: %1%"_fmt % get_ssl_error());
        net_read_buff_.resize((size_t)rdsz);
        auto net_read_data = to_array_view(net_read_buff_);
        while (!net_read_data.empty()) {
            auto ressz = sock.write_some(net_read_data);
            if (!ressz.has_value() || ressz.value() == 0) {
                throw eof_exception("can't write ssl socket");
            }
            net_read_data.advance_front(ressz.value());
        }
    } else {
        lck.lock();
    }
}

void ssl_factory::ssl_descriptor::handle_async_perform_result(int ssl_error, size_t pending_sz)
{
    if (ssl_error == SSL_ERROR_SSL) {
        throw eof_exception("ssl error: %1%"_fmt % get_ssl_error());
    } else if (ssl_error == SSL_ERROR_SYSCALL) {
        shutdowned_ = true;
        throw exception("ssl sys error: %1%"_fmt % get_ssl_error());
    }

    if (pending_sz) {
        write_from_bio_to_sock(pending_sz);
    }

    if (!ssl_error) return;

    if (ssl_error == SSL_ERROR_WANT_READ) {
        unique_lock lck{read_mtx_, std::try_to_lock};
        if (lck) {
            if (net_write_data_.empty()) {
                auto ressz = sock.read_some(&net_write_buff_.front(), net_write_buff_.size());
                if (!ressz.has_value()) {
                    try { std::rethrow_exception(ressz.error()); }
                    catch (...) { std::throw_with_nested(eof_exception("can't read ssl socket")); }
                }
                if (ressz.value() == 0) {
                    throw eof_exception("can't read ssl socket");
                }
                net_write_data_ = array_view(&net_write_buff_.front(), ressz.value());
            }

            int wrsz = BIO_write(net_bio_, net_write_data_.begin(), (int)net_write_data_.size());
            if (wrsz <= 0) throw exception("can't write into BIO object, error: %1%"_fmt % get_ssl_error());
            net_write_data_.advance_front(wrsz);
        } else {
            lck.lock();
        }
    } else if (ssl_error == SSL_ERROR_WANT_WRITE) {
        write_from_bio_to_sock(read_buffsz_);
    } else if (ssl_error == SSL_ERROR_ZERO_RETURN) {
        throw eof_exception("SSL_ERROR_ZERO_RETURN");
    } else if (ssl_error) {
        throw exception("ssl error code: %1%, what: %2%"_fmt % ssl_error % get_ssl_error());
    }
}

size_t ssl_factory::ssl_descriptor::perform(int (ssl_factory::ssl_descriptor::* op)(array_view<char>), array_view<char> data)
{
    for (;;) try {
        ERR_clear_error();
        int result = (this->*op)(data);
        int ssl_error = SSL_get_error(ssl_, result);
        size_t pending_sz = BIO_ctrl_pending(net_bio_);
        handle_async_perform_result(ssl_error, pending_sz);

        if (result > 0) {
            BOOST_ASSERT (!ssl_error);
            return (size_t)result;
        }
    } catch (eof_exception const&) {
        // GLOBAL_LOG_TRACE() << "eof_exception in perform: " << e.what();
        shutdowned_ = true;
        throw;
    }
}

void ssl_factory::ssl_descriptor::shutdown()
{
    if (!shutdowned_) {
        try {
            for (;;) {
                int result = SSL_shutdown(ssl_);
                if (1 == result) return;
                size_t pending_sz = BIO_ctrl_pending(net_bio_);
                if (0 == result) {
                    if (pending_sz) {
                        write_from_bio_to_sock(pending_sz);
                        continue;
                    }
                }
                break;
            }
        } catch (...) {}
        shutdowned_ = true;
    }
    if (sock) sock.close();
}

int ssl_factory::ssl_descriptor::do_accept(array_view<char>)
{
    return SSL_accept(ssl_);
}

int ssl_factory::ssl_descriptor::do_connect(array_view<char>)
{
    return SSL_connect(ssl_);
}

int ssl_factory::ssl_descriptor::do_read(array_view<char> data)
{
    return SSL_read(ssl_, data.begin(), static_cast<int>(data.size()));
}

int ssl_factory::ssl_descriptor::do_write(array_view<char> data)
{
    return SSL_write(ssl_, data.begin(), static_cast<int>(data.size()));
}

tcp_socket ssl_factory::create_connected_tcp_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt)
{
    tcp_socket sock = base_socket_factory_->create_connected_tcp_socket(address, port, dt);
    ssl_descriptor * sd = pool_.new_object(ctx_->ctx);
    sd->sock = std::move(sock);
    ssl_descriptor * sd_pin = sd; // pin
    SCOPE_EXIT([&sd_pin, this]{ if (sd_pin) pool_.delete_object(sd_pin); });

    sd->perform(&ssl_descriptor::do_connect);
    sd->shutdowned_ = false;
    sd_pin = nullptr; // unpin

    return tcp_socket_access::create_tcp_socket<io::socket_traits>(shared_from_this(), sd);
}

tcp_server_socket ssl_factory::create_server_socket(cstring_view address, uint16_t port, sonia::sal::net_family_type dt)
{
    tcp_server_socket sock = base_server_socket_factory_->create_server_socket(address, port, dt);
    ssl_acceptor_descriptor * sd = acceptor_pool_.new_object(std::move(sock));

    return tcp_socket_access::create_tcp_server_socket<io::socket_traits>(shared_from_this(), sd);
}

////////////////////////
fibers::future<tcp_socket> ssl_factory::tcp_server_socket_accept(tcp_handle_type h)
{
    ssl_descriptor * asd = pool_.new_object(ctx_->ctx);
    ssl_descriptor * asd_pin = asd;
    SCOPE_EXIT([&asd_pin, this]{ if (asd_pin) pool_.delete_object(asd_pin); });

    auto * ad = static_cast<ssl_acceptor_descriptor*>(h);
    
    auto self = shared_from_this();
    fibers::packaged_task<tcp_socket()> pt([ad, asd, f = std::move(self)] {
        tcp_socket result = tcp_socket_access::create_tcp_socket<socket_traits>(f, asd);
        asd->sock = ad->sock.accept().get();
        return std::move(result);
    });
    asd_pin = nullptr; // unpin

    fibers::future<tcp_socket> result = pt.get_future();
    fibers::fiber(std::move(pt)).detach();

    return std::move(result);
}

size_t ssl_factory::tcp_server_socket_accepting_count(tcp_handle_type h) const
{
    auto * ci = static_cast<ssl_acceptor_descriptor*>(h);
    return ci->sock.accepting_count();
}

void ssl_factory::close_handle(identity<tcp_server_socket_service_type>, tcp_handle_type h) noexcept
{
    auto * ci = static_cast<ssl_acceptor_descriptor*>(h);
    ci->sock.close();
}

void ssl_factory::release_handle(identity<tcp_server_socket_service_type>, tcp_handle_type h) noexcept
{
    close_handle(identity<tcp_server_socket_service_type>(), h);
}

void ssl_factory::free_handle(identity<tcp_server_socket_service_type>, tcp_handle_type h) noexcept
{
    auto * ci = static_cast<ssl_acceptor_descriptor*>(h);
    acceptor_pool_.delete_object(ci);
}

expected<size_t, std::exception_ptr> ssl_factory::tcp_socket_read_some(tcp_handle_type h, void * buff, size_t sz) noexcept
{
    try {
        auto * ci = static_cast<ssl_descriptor*>(h);
        if (ci->shutdowned_) {
            ci->perform(&ssl_descriptor::do_accept);
            ci->shutdowned_ = false;
        }
        return ci->perform(&ssl_descriptor::do_read, array_view(reinterpret_cast<char*>(buff), sz));
    } catch (...) {
        return make_unexpected(std::current_exception());
    }
}

expected<size_t, std::exception_ptr> ssl_factory::tcp_socket_write_some(tcp_handle_type h, void const* buff, size_t sz) noexcept
{
    try {
        auto * ci = static_cast<ssl_descriptor*>(h);
        if (ci->shutdowned_) {
            ci->perform(&ssl_descriptor::do_accept);
            ci->shutdowned_ = false;
        }
        return ci->perform(&ssl_descriptor::do_write, array_view(reinterpret_cast<char*>(const_cast<void*>(buff)), sz));
    } catch (...) {
        return make_unexpected(std::current_exception());
    }
}

void ssl_factory::close_handle(identity<io::tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    auto * ci = static_cast<ssl_descriptor*>(h);
    ci->shutdown();
}

void ssl_factory::release_handle(identity<io::tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    close_handle(identity<tcp_socket_service_type>(), h);
}

void ssl_factory::free_handle(identity<io::tcp_socket_service_type>, tcp_handle_type h) noexcept
{
    auto * ci = static_cast<ssl_descriptor*>(h);
    pool_.delete_object(ci);
}

sal::socket_handle ssl_factory::system_handle(tcp_handle_type h) noexcept
{
    auto* ci = static_cast<ssl_descriptor*>(h);
    return ci->sock.system_handle();
}

}
