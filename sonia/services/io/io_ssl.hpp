//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_FACTORY_SSL_HPP
#define SONIA_IO_FACTORY_SSL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <openssl/ssl.h>
#include <openssl/bio.h>

#include <boost/variant.hpp>

#include "sonia/shared_ptr.hpp"
#include "sonia/array_view.hpp"
#include "io_ssl_configuration.hpp"

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

class io_ssl
{
public:
    explicit io_ssl(ssl_configuration const& cfg);

private:
    shared_ptr<openssl_context> ctx;
};

}

#endif // SONIA_IO_FACTORY_SSL_HPP
