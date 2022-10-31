//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/string.hpp"
#include "sonia/optional.hpp"

namespace sonia::authentication {

class authenticator
{
public:
    virtual ~authenticator() = default;
    virtual std::string get_digest_for(string_view, string_view password) const = 0;
    virtual string_view get_realm() const = 0;
};

class digest_provider
{
public:
    virtual ~digest_provider() = default;

    virtual optional<std::string> get_digest_for(string_view) const = 0;
};

}
