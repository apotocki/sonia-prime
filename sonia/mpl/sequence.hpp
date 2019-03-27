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
 
template <class SequenceT> struct size;

template <class SequenceT> struct size<const SequenceT> : size<SequenceT> {};

template <typename ... Types>
struct size<std::tuple<Types...>> : std::tuple_size<std::tuple<Types...>> {};

template <class SequenceT>
constexpr size_t size_v = size<SequenceT>::value;

template <class SequenceT, size_t I> struct at : boost::mpl::at_c<SequenceT, I> {};

template <typename ... Types, size_t I>
struct at<std::tuple<Types...>, I> : std::tuple_element<I, std::tuple<Types...>> {};

template <size_t I, typename ... Types>
decltype(auto) at_c(std::tuple<Types...> & v) { return std::get<I>(v); }

template <size_t I, typename ... Types>
decltype(auto) at_c(std::tuple<Types...> const& v) { return std::get<I>(v); }

template <class SequenceT, size_t I> using at_t = typename at<SequenceT, I>::type;

}}

#endif // SONIA_MPL_SEQUENCE_HPP
