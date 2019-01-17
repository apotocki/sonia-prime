//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_ARRAY_HPP
#define SONIA_SERIALIZATION_ARRAY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <algorithm>

#include "sonia/array_view.hpp"
#include "sonia/utility/algorithm/pull.hpp"
#include "serialization.hpp"

namespace sonia { namespace serialization {

template <typename TagT, typename T>
class coder<TagT, array_view<T>>
{
    using type = array_view<T>;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(type const& value, OutputIteratorT oi) const
    {
        if constexpr (is_trivial_v<T> && sizeof(T) == 1) {
            return std::copy(value.begin(), value.end(), std::move(oi));
        } else {
            coder<TagT, T> enc;
            for (auto const& e : value) {
                oi = enc.encode(e, std::move(oi));
            }
            return std::move(oi);
        }
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, type value) const
    {
        if constexpr (is_trivial_v<T> && sizeof(T) == 1) {
            return sonia::pull(std::move(ii), value.begin(), value.end());
        } else {
            coder<TagT, T> dec;
            for (auto & e : value) {
                ii = dec.decode(std::move(ii), e);
            }
            return std::move(ii);
        }
    }
};

}}

#endif // SONIA_SERIALIZATION_ARRAY_HPP
