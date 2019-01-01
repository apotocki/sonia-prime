//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_HASH_ARRAY_VIEW_HPP
#define SONIA_FUNCTIONAL_HASH_ARRAY_VIEW_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/utility/functional/hash.hpp"
#include "sonia/array_view.hpp"

namespace sonia {

template <typename T>
struct hash<array_view<T>> {
    static constexpr size_t init_value() {
        if constexpr (sizeof(size_t) == 4) {
            return 2166136261U;
        } else {
            return 14695981039346656037ULL;
        }
    }

    static constexpr size_t prime_value() {
        if constexpr (sizeof(size_t) == 4) {
            return 16777619U;
        } else {
            return 1099511628211ULL;
        }
    }

    size_t operator()(array_view<T> arr) const noexcept {
        size_t hash_value = init_value();
        if constexpr (sizeof(T) == 1 && is_integral_v<T>) {
            const uint8_t * it = (uint8_t const*)(arr.begin());
            const uint8_t * eit = (uint8_t const*)(arr.end());
            for (; it != eit; ++it) {
                hash_value ^= static_cast<size_t>(*it);
                hash_value *= prime_value();
            }
        } else {
            for (T const& item : arr) {
                hash_combine(hash_value, item);
            }
        }
        return hash_value;
    }
};

}

#endif // SONIA_FUNCTIONAL_HASH_ARRAY_VIEW_HPP
