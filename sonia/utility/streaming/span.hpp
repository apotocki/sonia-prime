//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#ifndef SONIA_SPAN_DECLARED
#   error do not include directly, use "sonia/span.hpp"
#endif

#include <ios>
#include <type_traits>

namespace std {

template <typename CharT, class TraitsT, typename T, size_t ET>
std::basic_ostream<CharT, TraitsT> & operator<< (basic_ostream<CharT, TraitsT> & os, sonia::span<T, ET> arr)
{
    os << '[';
    if constexpr (is_integral_v<T>) {
        os << hex;
    }
    bool first = true;
    for (T const& val : arr) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }
        if constexpr (is_integral_v<T> && sizeof(T) == 1) {
            os << (int)val;
        } else {
            os << val;
        }
    }
    return os << ']';
}

template <typename CharT, class TraitsT, typename T>
requires(requires{ std::span{ std::declval<const T>() }; })
std::basic_ostream<CharT, TraitsT>& operator<< (std::basic_ostream<CharT, TraitsT>& os, T const& vec)
{
    return os << std::span{ vec };
}

template <typename CharT, class TraitsT, typename T>
requires(
    !requires{ std::span{ std::declval<const T>() }; }&&
    requires{ to_span(std::declval<const T>()); }
)
std::basic_ostream<CharT, TraitsT>& operator<< (std::basic_ostream<CharT, TraitsT>& os, T const& vec)
{
    return os << to_span(vec);
}

}
