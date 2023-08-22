//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once
#ifndef SONIA_VARIANT_DECLARED
#   error do not include directly, use "sonia/variant.hpp"
#endif

#include <iosfwd>

namespace sonia {

template <typename CharT, class TraitsT>
struct variant_streaming_visitor : static_visitor<size_t>
{
    std::basic_ostream<CharT, TraitsT>& os_;
    explicit variant_streaming_visitor(std::basic_ostream<CharT, TraitsT>& os) : os_{ os } {}

    template <typename T>
    std::basic_ostream<CharT, TraitsT>& operator()(T const& val) const
    {
        return os_ << val;
    }
};

}

namespace std {

template <typename CharT, class TraitsT, typename ... Ts>
basic_ostream<CharT, TraitsT> & operator<< (basic_ostream<CharT, TraitsT> & os, sonia::variant<Ts...> const& v)
{
    return sonia::apply_visitor(sonia::variant_streaming_visitor(os), v);
}

}
