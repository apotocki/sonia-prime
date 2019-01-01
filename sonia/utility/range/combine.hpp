//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_RANGE_COMBINE_HPP
#define SONIA_UTILITY_RANGE_COMBINE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

not implemented yet

#include <tuple>

#include "sonia/type_traits.hpp"

namespace sonia {

template <typename... Ranges>
auto combine(Ranges&&... rngs) ->
combined_range<decltype(std::tuple(boost::begin(rngs)...))>
{
    return combined_range<decltype(std::tuple(boost::begin(rngs)...))>(
        std::tuple(boost::begin(rngs)...),
        std::tuple(boost::end(rngs)...));
}

}

#endif // SONIA_UTILITY_RANGE_COMBINE_HPP
