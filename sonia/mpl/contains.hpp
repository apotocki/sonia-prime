//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_CONTAINS_HPP
#define SONIA_MPL_CONTAINS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"
#include "sonia/mpl/find_if.hpp"
#include "sonia/mpl/iterator.hpp"

namespace sonia::mpl {

template <typename SeqT, typename T>
struct contains : bool_constant<!is_same_v<find_if_t<SeqT, is_same<T, _1>>, end_t<SeqT>>> {};

template <typename SeqT, typename T> using contains_t = typename contains<SeqT, T>::type;

}

#endif // SONIA_MPL_CONTAINS_HPP
