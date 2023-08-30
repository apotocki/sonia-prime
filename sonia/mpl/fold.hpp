//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/mpl/sequence.hpp"
#include "sonia/mpl/iterator.hpp" // deref
#include "sonia/mpl/lambda.hpp"
#include "sonia/mpl/apply_wrap.hpp"

namespace sonia::mpl {

template <typename IterT, typename EndIterT, typename StateT, typename OpT>
struct iter_fold_one
{
    using type = typename iter_fold_one<next_t<IterT>, EndIterT, apply_wrap_t<OpT, StateT, IterT>, OpT>::type;
};

template <typename IterT, typename StateT, typename OpT>
struct iter_fold_one<IterT, IterT, StateT, OpT> { using type = StateT; };

template <typename SeqT, typename StateT, typename ForwardOpT>
struct iter_fold
{
    using type = typename iter_fold_one<begin_t<SeqT>, end_t<SeqT>, StateT, lambda_t<ForwardOpT>>::type;
};

template <typename SeqT, typename StateT, typename ForwardOpT>
using iter_fold_t = typename iter_fold<SeqT, StateT, ForwardOpT>::type;


template <typename SeqT, typename StateT, typename OpT>
struct fold
{
    using type = iter_fold_t<SeqT, StateT, apply_wrap<lambda_t<OpT>, _1, deref<_2>>>;
};

template <typename SeqT, typename StateT, typename OpT>
using fold_t = typename fold<SeqT, StateT, OpT>::type;

}
