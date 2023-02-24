//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <iosfwd>
#include <time.h>

#include "sonia/string.hpp"

#include "status_codes.hpp"
#include "headers.hpp"

namespace sonia::http {

#define SONIA_HTTP_METHODS_SEQ \
    ((GET,  "GET"))            \
    ((HEAD, "HEAD"))           \
    ((POST, "POST"))           \
    ((PUT, "PUT"))             \
    ((DELETE_, "DELETE"))      \
    ((CONNECT, "CONNECT"))     \
    ((OPTIONS, "OPTIONS"))     \
    ((TRACE_, "TRACE"))        \
    ((PATCH, "PATCH"))

#define SONIA_PRINT_TYPE_ENUM(r, data, i, elem) BOOST_PP_TUPLE_ELEM(2, 0, elem),

enum class method_verb
{
    UNKNOWN = 0,
    BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_TYPE_ENUM, _, SONIA_HTTP_METHODS_SEQ)
    UPPER_BOUND_VALUE
};

std::string const& to_string(method_verb);
method_verb to_method(string_view);

template <typename CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, method_verb mv)
{
    return os << to_string(mv);
}

std::string http_date(tm* tm);

#undef SONIA_PRINT_TYPE_ENUM

}
