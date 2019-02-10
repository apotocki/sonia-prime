//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_STREAMING_VECTOR_HPP
#define SONIA_UTILITY_STREAMING_VECTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <iosfwd>
#include <tuple>

namespace std {

template <size_t I, class TupleT>
struct tplelem
{
    TupleT const& val;
};

template <typename CharT, class TraitsT, size_t I, class TupleT>
basic_ostream<CharT, TraitsT> & operator<< (basic_ostream<CharT, TraitsT> & os, tplelem<I, TupleT> const& t)
{
    if constexpr (!I) {
        return os << std::get<I>(t.val);
    } else {
        return os << ", " << std::get<I>(t.val);
    }
}

template <typename CharT, class TraitsT, class TupleT, size_t ... I>
basic_ostream<CharT, TraitsT> & print_tuple(basic_ostream<CharT, TraitsT> & os, TupleT const& t, std::index_sequence<I ...>)
{
    return (os << ... << tplelem<I, TupleT>{t});
}

template <typename CharT, class TraitsT, typename ... Ts>
basic_ostream<CharT, TraitsT> & operator<< (basic_ostream<CharT, TraitsT> & os, tuple<Ts...> const& t)
{
    return print_tuple(os << "[", t, std::make_index_sequence<sizeof... (Ts)>()) << "]";
}

}

#endif // SONIA_UTILITY_STREAMING_VECTOR_HPP
