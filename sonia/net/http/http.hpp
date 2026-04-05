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

#define SONIA_HTTP_METHODS_SEQ  \
    ((GET,  "GET"sv))           \
    ((HEAD, "HEAD"sv))          \
    ((POST, "POST"sv))          \
    ((PUT, "PUT"sv))            \
    ((DELETE_, "DELETE"sv))     \
    ((CONNECT, "CONNECT"sv))    \
    ((OPTIONS, "OPTIONS"sv))    \
    ((TRACE_, "TRACE"sv))       \
    ((PATCH, "PATCH"sv))

#define SONIA_PRINT_TYPE_ENUM(r, data, i, elem) BOOST_PP_TUPLE_ELEM(2, 0, elem),

enum class method_verb
{
    UNKNOWN = 0,
    BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_TYPE_ENUM, _, SONIA_HTTP_METHODS_SEQ)
    UPPER_BOUND_VALUE
};

std::string_view to_string_view(method_verb);
method_verb to_method(std::string_view);

template <typename CharT, class TraitsT>
inline std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, method_verb mv)
{
    return os << to_string_view(mv);
}

std::string http_date(tm* tm);

#undef SONIA_PRINT_TYPE_ENUM

}
