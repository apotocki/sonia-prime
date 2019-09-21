//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_AUTHENTICATION_HPP
#define SONIA_SERVICES_AUTHENTICATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/string.hpp"
#include "sonia/optional.hpp"

namespace sonia::authentication {

class digest_provider
{
public:
    virtual ~digest_provider() = default;

    virtual optional<std::string> get_digest_for(string_view) const = 0;
    virtual std::string get_digest_for(string_view, string_view password) const = 0;
    virtual string_view get_realm() const = 0;
};

}

#endif // SONIA_SERVICES_AUTHENTICATION_HPP
