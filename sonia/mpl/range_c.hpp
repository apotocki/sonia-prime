//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_RANGE_C_HPP
#define SONIA_MPL_RANGE_C_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"
#include "sonia/mpl/sequence.hpp"

namespace sonia::mpl {

template <typename T, T StartV, T FinishV>
struct range_c
{
    using start = integral_constant<T, StartV>;
    using finish = integral_constant<T, FinishV>;
    static_assert (FinishV >= StartV);
};

struct range_tag{};
template <typename T, T StartV, T FinishV> struct tag_of<range_c<T, StartV, FinishV>> { using type = range_tag; };

template <typename T, T StartV, T FinishV> struct size<range_c<T, StartV, FinishV>> : integral_constant<size_t, FinishV - StartV> {};
template <typename T, T StartV, T FinishV, class IdxT> struct at<range_c<T, StartV, FinishV>, IdxT> { using type = integral_constant<T, StartV + IdxT::value>; };
template <typename T, T StartV, T FinishV, size_t IdxV> struct at_c<range_c<T, StartV, FinishV>, IdxV> { using type = integral_constant<T, StartV + IdxV>; };
template <typename T, T StartV, T FinishV> struct begin<range_c<T, StartV, FinishV>> { using type = iterator<0, range_c<T, StartV, FinishV>>; };
template <typename T, T StartV, T FinishV> struct end<range_c<T, StartV, FinishV>> { using type = iterator<FinishV, range_c<T, StartV, FinishV>>; };

}

namespace sonia {

template <typename T, T StartV, T FinishV>
struct is_mpl_sequence<mpl::range_c<T, StartV, FinishV>> : true_type {};

}

#endif // SONIA_MPL_RANGE_C_HPP
