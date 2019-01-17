//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_MARSHALING_STUB_PARAMETER_HPP
#define SONIA_UTILITY_MARSHALING_STUB_PARAMETER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"

namespace sonia {

template <typename T, typename Enabler = void>
struct stub_bound_parameter { using type = remove_cvref_t<T>; };

template <typename T> using stub_bound_parameter_t = typename stub_bound_parameter<T>::type;

}

#endif // SONIA_UTILITY_MARSHALING_STUB_PARAMETER_HPP
