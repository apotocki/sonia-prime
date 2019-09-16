//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_FIND_IF_HPP
#define SONIA_MPL_FIND_IF_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"
#include "sonia/mpl/iterator.hpp"
#include "sonia/mpl/lambda.hpp"

namespace sonia::mpl {

template <typename BIT, typename EIT, typename PredT>
struct find_if_impl
{
    using type = typename conditional_t<
        PredT::template apply<deref_t<BIT>>::type::value,
        identity<BIT>,
        find_if_impl<next_t<BIT>, EIT, PredT>
    >::type;
};

template <typename EIT, typename PredT> struct find_if_impl<EIT, EIT, PredT>
{
    using type = EIT;
};

template <typename SeqT, typename PredT>
using find_if = find_if_impl<begin_t<SeqT>, end_t<SeqT>, lambda_t<PredT>>;

template <typename SeqT, typename PredT> using find_if_t = typename find_if<SeqT, PredT>::type;

}

#endif // SONIA_MPL_FIND_IF_HPP
