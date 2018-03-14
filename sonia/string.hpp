//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_STRING_HPP
#define SONIA_STRING_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <string>
#include "array_view.hpp"

namespace sonia {

using std::string;

template <typename CharT, class TraitsT = std::char_traits<CharT>>
class basic_string_view : public basic_array_view<CharT>
{
public:
    basic_string_view() {}
};

template <typename CharT, class TraitsT = std::char_traits<CharT>>
class basic_cstring_view : public basic_string_view<CharT, TraitsT> {
public:

    const char* c_str() const noexcept { return data(); }
};

typedef basic_string_view<char> string_view;
typedef basic_cstring_view<char> cstring_view;

}

#endif // SONIA_STRING_HPP
