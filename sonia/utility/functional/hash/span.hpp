//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once
#ifndef SONIA_SPAN_DECLARED
#   error do not include directly, use "sonia/span.hpp"
#endif
#include "sonia/utility/functional/hash.hpp"

namespace sonia {

template <typename T, size_t ET>
struct hash<span<T, ET>>
{
    size_t operator()(span<T, ET> const& sp) const noexcept
    {
        size_t hash_value = hash_init_value();
        if constexpr (sizeof(T) == 1 && is_integral_v<T>)
        {
            const uint8_t* it = (uint8_t const*)(sp.data());
            const uint8_t* eit = it + sp.size();
            for (; it != eit; ++it) {
                hash_value ^= static_cast<size_t>(*it);
                hash_value *= hash_prime_value();
            }
        }
        else {
            for (T const& item : sp) {
                sonia::hash_combine(hash_value, item);
            }
        }
        return hash_value;
    }
};

template <typename T>
requires(requires{ span{ std::declval<const T>()}; })
struct hash<T>
{
    size_t operator()(T const& vec) const noexcept
    {
        return hasher()(span{ vec });
    }
};

template <typename T>
requires(
    !requires{ span{ std::declval<const T>() }; } &&
    requires{ to_span(std::declval<const T>()); }
)
struct hash<T>
{
    size_t operator()(T const& vec) const noexcept
    {
        return hasher()(to_span(vec));
    }
};

}
