//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_AS_VECTOR_HPP
#define SONIA_MPL_AS_VECTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/mpl/vector.hpp"
#include "sonia/mpl/fold.hpp"
#include "sonia/mpl/push_back.hpp"

namespace sonia::mpl {

template <typename SeqT> struct as_vector : fold<SeqT, vector<>, push_back<_1, _2>> {};
template <typename ... Types> struct as_vector<vector<Types...>> { using type = vector<Types...>; };
template <typename SeqT> using as_vector_t = typename as_vector<SeqT>::type;

}

#endif // SONIA_MPL_AS_VECTOR_HPP
