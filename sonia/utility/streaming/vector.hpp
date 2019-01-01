//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_STREAMING_VECTOR_HPP
#define SONIA_UTILITY_STREAMING_VECTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <iosfwd>
#include <vector>
#include "array_view.hpp"

namespace std {

template <typename CharT, class TraitsT, typename T, class AllocatorT>
basic_ostream<CharT, TraitsT> & operator<< (basic_ostream<CharT, TraitsT> & os, vector<T, AllocatorT> const& vec)
{
    return os << sonia::to_array_view(vec);
}

}

#endif // SONIA_UTILITY_STREAMING_VECTOR_HPP
