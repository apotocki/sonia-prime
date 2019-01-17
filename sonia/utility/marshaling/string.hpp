//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_MARSHALING_STUB_PARAMETER_STRING_HPP
#define SONIA_UTILITY_MARSHALING_STUB_PARAMETER_STRING_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/string.hpp"
#include "stub_parameter.hpp"

namespace sonia {

template <typename CharT, class TraitsT>
struct stub_bound_parameter<basic_string_view<CharT, TraitsT>> { using type = std::basic_string<CharT, TraitsT>; };

}

#endif // SONIA_UTILITY_MARSHALING_STUB_PARAMETER_STRING_HPP
