//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_MIN_ELEMENT_HPP
#define SONIA_MPL_MIN_ELEMENT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"
#include "sonia/mpl/less.hpp"
#include "sonia/mpl/fold.hpp"
#include "sonia/mpl/iterator.hpp"
#include "sonia/mpl/lambda.hpp"

namespace sonia::mpl {

template <typename SeqT, typename PredT = less<_1,_2>>
struct min_element : iter_fold<
          SeqT
        , begin_t<SeqT>
        , switchable<typename lambda_t<PredT>::template apply<deref<_1>, deref<_2>>, _1, _2>
    >
{};

template <typename SeqT, typename PredT = less<_1,_2>> using min_element_t = typename min_element<SeqT, PredT>::type;

}

#endif // SONIA_MPL_MIN_ELEMENT_HPP
