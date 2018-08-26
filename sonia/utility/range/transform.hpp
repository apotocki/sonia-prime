//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_RANGE_TRANSFORM_HPP
#define SONIA_UTILITY_RANGE_TRANSFORM_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <tuple>

#include "sonia/type_traits.hpp"

namespace sonia { namespace range {

template <typename RangeT, typename TransformerT>
class transformed_range {
public:
    template <typename RArgT>
    transformed_range(RArgT && rarg, TransformerT const& f) : rng_(std::forward<RArgT>(rarg)), func_(f) {}
private:
    RangeT rng_;
    TransformerT func_;
};

template <typename RangeT, typename TransformerT>
transformed_range<RangeT, TransformerT> transform(RangeT&& rng, TransformerT const& ftor) 
{
    return transformed_range<remove_reference_t<RangeT>, TransformerT>(std::forward<RangeT>(rng, ftor));
}

}

#endif // SONIA_UTILITY_RANGE_COMBINE_HPP
