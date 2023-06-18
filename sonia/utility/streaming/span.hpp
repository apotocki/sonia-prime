//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <iosfwd>
#include <span>

namespace std {

template <typename CharT, class TraitsT, typename T, size_t ET>
std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, std::span<T, ET> arr)
{
    os << '[';
    if constexpr (is_integral_v<T>) {
        os << std::hex;
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

}
