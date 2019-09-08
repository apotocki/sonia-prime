//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_VECTOR_HPP
#define SONIA_MPL_VECTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <tuple>
#include <type_traits>

namespace sonia::mpl {

template <typename ... ArgsT> using vector = std::tuple<ArgsT ...>;

template <typename T, T ... ValsV>
using vector_c = std::tuple<std::integral_constant<T, ValsV>...>;

}

#endif // SONIA_MPL_VECTOR_HPP
