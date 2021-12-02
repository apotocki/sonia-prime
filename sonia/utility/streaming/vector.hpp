//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <iosfwd>
#include <vector>
#include "array_view.hpp"

namespace std {

template <typename CharT, class TraitsT, typename T, class AllocatorT>
basic_ostream<CharT, TraitsT> & operator<< (basic_ostream<CharT, TraitsT> & os, vector<T, AllocatorT> const& vec)
{
    return os << sonia::array_view(vec);
}

}
