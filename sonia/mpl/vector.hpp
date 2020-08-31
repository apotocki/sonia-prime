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

#include "sonia/mpl/sequence.hpp"
#include "sonia/mpl/iterator.hpp"

namespace sonia::mpl {

struct std_tuple_tag{};

template <typename ... Types> struct tag_of<std::tuple<Types...>> { using type = std_tuple_tag; };
template <> struct size_impl<std_tuple_tag>
{
    template <class TupleT> struct apply : std::tuple_size<TupleT> {};
};

template <> struct at_impl<std_tuple_tag>
{
    template <class TupleT, class IdxT> struct apply : std::tuple_element<IdxT::value, TupleT> {};
};

template <> struct at_c_impl<std_tuple_tag>
{
    template <class TupleT, size_t IdxV> struct apply : std::tuple_element<IdxV, TupleT> {};
};

template <> struct begin_impl<std_tuple_tag>
{
    template <class TupleT> struct apply { using type = iterator<0, TupleT>; };
};

template <> struct end_impl<std_tuple_tag>
{
    template <class TupleT> struct apply { using type = iterator<std::tuple_size<TupleT>::value, TupleT>; };
};

template <typename ... ArgsT> using vector = std::tuple<ArgsT ...>;

template <typename T, T ... ValsV>
using vector_c = std::tuple<std::integral_constant<T, ValsV>...>;

}

namespace sonia {

template <size_t I, typename ... Types>
decltype(auto) at_c(std::tuple<Types...> & v) { return std::get<I>(v); }

template <size_t I, typename ... Types>
decltype(auto) at_c(std::tuple<Types...> const& v) { return std::get<I>(v); }

template <typename ... Types>
struct is_mpl_sequence<std::tuple<Types...>> : true_type {};

}

#endif // SONIA_MPL_VECTOR_HPP
