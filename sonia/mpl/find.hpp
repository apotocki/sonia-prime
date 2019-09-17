//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_FIND_HPP
#define SONIA_MPL_FIND_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"
#include "sonia/mpl/find_if.hpp"

namespace sonia::mpl {

template <typename SeqT, typename LT> struct find_impl : find_if<SeqT, is_same<_1, LT>> {};

template <typename SeqT, typename T> using find = find_impl<SeqT, lambda_t<T>>;

//template <typename SeqT, typename T> using find = find_if<SeqT, is_same<_1, lambda_t<T>>>;

template <typename SeqT, typename T> using find_t = typename find<SeqT, T>::type;

}

#endif // SONIA_MPL_FIND_HPP
