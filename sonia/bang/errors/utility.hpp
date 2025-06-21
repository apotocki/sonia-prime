//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

struct printer_resolver_visitor : static_visitor<std::string>
{
    unit const& u_;
    explicit printer_resolver_visitor(unit const& u) : u_{ u } {}

    inline result_type operator()(nullptr_t const&) const { return {}; }
    //inline result_type operator()(null_t const&) const { return {}; }

    template <typename T>
    inline result_type operator()(T const& val) const
    {
        return u_.print(val);
    }
};

struct string_resolver_visitor : static_visitor<string_view>
{
    string_resolver_visitor() = default;

    inline result_type operator()(std::string const& str) const { return str; }
    inline result_type operator()(string_view str) const { return str; }
};

inline std::ostream& write(std::ostream& s, lex::resource_location const& loc)
{
    return s << loc.resource << '(' << loc.line << ',' << loc.column << ')';
}

}
