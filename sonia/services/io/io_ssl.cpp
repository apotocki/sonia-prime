//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "io_ssl.hpp"

#include <sstream>
#include <fstream>
#include <boost/filesystem.hpp>

#include "openssl/ssl.h"
#include "openssl/err.h"

#include "sonia/singleton.hpp"
#include "sonia/exceptions/internal_errors.hpp"
#include "sonia/utility/scope_exit.hpp"

#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")

#if OPENSSL_VERSION_NUMBER < 0x10100000L
#   error inappropriate OpenSSL version
#endif

namespace sonia::io {

namespace fs = boost::filesystem;

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

io_ssl::io_ssl(ssl_configuration const& cf)
    : ctx(as_singleton<openssl_support>()->create_context())
{
    std::vector<char> certificate = load_media(cf.certificate, cf.certificate_media_type);
    auto cert = make_shared<openssl_x509>(to_array_view(certificate), cf.certificate_type);
    ctx->use(cert);

    std::vector<char> private_key = load_media(cf.private_key, cf.private_key_media_type);
    auto pk = make_shared<openssl_key>(to_array_view(private_key), cf.private_key_type, cf.private_key_password);
    ctx->use(pk);
}

}
