//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <span>

namespace sonia::serialization {

template <typename TagT, typename T, size_t EV>
class coder<TagT, std::span<T, EV>>
{
    using type = std::span<T, EV>;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(type value, OutputIteratorT oi) const
    {
        if constexpr (EV == std::dynamic_extent) {
            oi = sonia::encode<TagT, typename type::size_type>(value.size(), std::move(oi));
        }
        if constexpr (is_trivial_v<T> && sizeof(T) == 1) {
            return sonia::copy(value.begin(), value.end(), std::move(oi));
        } else {
            coder<TagT, remove_cv_t<T>> enc;
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

}
