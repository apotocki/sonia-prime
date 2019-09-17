//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_SEQUENCE_HPP
#define SONIA_MPL_SEQUENCE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <tuple>
#include "sonia/type_traits.hpp"

namespace sonia {

template <class T> struct is_mpl_sequence : false_type {};

template <class T>
constexpr bool is_mpl_sequence_v = is_mpl_sequence<T>::value;

namespace mpl {
 
template <typename T> struct tag_of;
template <typename T> struct tag_of<const T> : tag_of<T> {};
template <typename T> using tag_of_t = typename tag_of<T>::type;

/// size
template <typename TagT> struct size_impl;
template <class SequenceT> struct size : size_impl<tag_of_t<SequenceT>>::template apply<SequenceT>{};
template <class SequenceT> struct size<const SequenceT> : size<SequenceT> {};
template <class SequenceT> constexpr size_t size_v = size<SequenceT>::value;

/// at
template <typename TagT> struct at_impl;
template <class SequenceT, class IdxT> struct at : at_impl<tag_of_t<SequenceT>>::template apply<SequenceT, IdxT>{};
template <class SequenceT, class IdxT> using at_t = typename at<SequenceT, IdxT>::type;

template <typename TagT> struct at_c_impl;
template <class SequenceT, size_t IdxV> struct at_c : at_c_impl<tag_of_t<SequenceT>>::template apply<SequenceT, IdxV>{};
template <class SequenceT, size_t IdxV> using at_c_t = typename at_c<SequenceT, IdxV>::type;

/// begin
template <typename TagT> struct begin_impl { using type = void; };
template <class SequenceT> struct begin : begin_impl<tag_of_t<SequenceT>>::template apply<SequenceT>{};
template <class SequenceT> using begin_t = typename begin<SequenceT>::type;

/// end
template <typename TagT> struct end_impl { using type = void; };
template <class SequenceT> struct end : end_impl<tag_of_t<SequenceT>>::template apply<SequenceT>{};
template <class SequenceT> using end_t = typename end<SequenceT>::type;

// empty
template <class SequenceT> using empty = std::bool_constant<size_v<SequenceT> == 0>;
template <class SequenceT> static constexpr bool empty_v = size_v<SequenceT> == 0;

// front
template <class SequenceT> using front = at_c<SequenceT, 0>;
template <class SequenceT> using front_t = at_c_t<SequenceT, 0>;

// front
template <class SequenceT> using back = at_c<SequenceT, size_v<SequenceT> - 1>;
template <class SequenceT> using back_t = at_c_t<SequenceT, size_v<SequenceT> - 1>;

}}

#endif // SONIA_MPL_SEQUENCE_HPP
