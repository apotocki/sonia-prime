//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"

namespace sonia::io {

enum class ssl_media_type
{
    STRING,
    FILE
};

enum class x509_format_type
{
    DERFORMAT,
    PEMFORMAT
};

enum class key_format_type
{
    PRIVATE_RSA,
    PRIVATE_EVP
};

struct ssl_configuration
{
    std::string certificate;
    ssl_media_type certificate_media_type;
    x509_format_type certificate_type;

    std::string private_key;
    ssl_media_type private_key_media_type;
    key_format_type private_key_type;
    optional<std::string> private_key_password;
};

}
