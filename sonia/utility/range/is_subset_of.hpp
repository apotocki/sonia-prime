//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_RANGE_IS_SUBSET_OF_HPP
#define SONIA_UTILITY_RANGE_IS_SUBSET_OF_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/array_view.hpp"

namespace sonia::ranges {

template <typename RangeT> struct is_subset_of_impl{};

template <typename T> struct is_subset_of_impl<array_view<T>>
{
    bool operator()(array_view<T> ss, array_view<T> r) const { return ss.is_subset_of(r); }
};

template <typename RangeT>
bool is_subset_of(RangeT const& subset, RangeT const& r)
{
    return is_subset_of_impl<RangeT>()(subset, r);
}

}
#endif // SONIA_UTILITY_RANGE_IS_SUBSET_OF_HPP
