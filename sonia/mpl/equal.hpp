//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_EQUAL_HPP
#define SONIA_MPL_EQUAL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"
#include "sonia/mpl/sequence.hpp"
#include "sonia/mpl/iterator.hpp"
#include "sonia/mpl/lambda.hpp"

namespace sonia::mpl {

template <typename BIT, typename EIT, typename BIT2, typename PredT>
struct equal_impl : conditional_t<
        PredT::template apply_t<deref_t<BIT>, deref_t<BIT2>>::value,
        equal_impl<next_t<BIT>, EIT, next_t<BIT2>, PredT>,
        false_type
    >
{};

template <typename EIT, typename BIT2, typename PredT> struct equal_impl<EIT, EIT, BIT2, PredT> : true_type {};

template <typename SeqT1, typename SeqT2, typename PredT = is_same<_1,_2>>
struct equal : conditional_t<
        size_v<SeqT1> != size_v<SeqT2>,
        false_type,
        equal_impl<begin_t<SeqT1>, end_t<SeqT1>, begin_t<SeqT2>, lambda_t<PredT>>
    >
{};

template <typename SeqT1, typename SeqT2, typename PredT = is_same<_1,_2>> using equal_t = typename equal<SeqT1, SeqT2, PredT>::type;

}

#endif // SONIA_MPL_EQUAL_HPP
