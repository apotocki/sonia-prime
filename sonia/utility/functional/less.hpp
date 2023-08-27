//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <functional>

#include "sonia/type_traits.hpp"

namespace sonia {

template <typename LT, typename RT>
struct less_unrelated
{

};

template <typename LT, typename RT>
struct less
{
    template <typename LArgT, typename RArgT>
    inline bool operator()(LArgT && l, RArgT && r) const
    {
        if constexpr (is_same_v<LT, RT>) {
            return std::less<LT>()(std::forward<LArgT>(l), std::forward<RArgT>(r));
        } else if constexpr (requires{ !std::less<>()(std::forward<LArgT>(l), std::forward<RArgT>(r)); }) {
            return std::less<>()(std::forward<LArgT>(l), std::forward<RArgT>(r));
        } else {
            // compare unrelated types
            return less_unrelated<LT, RT>::value;
        }
    }
};

template <typename LT, typename RT>
requires(is_integral_v<LT> && is_integral_v<RT>)
struct less<LT, RT>
{
    constexpr bool operator()(LT l, RT r) const noexcept
    {
        if constexpr (is_signed_v<LT>) {
            if constexpr (is_signed_v<RT>) {
                return l < r;
            } else {
                return l < 0 || static_cast<uint64_t>(l) < r;
            }
        } else if constexpr (is_unsigned_v<RT>) {
            return l < r;
        } else {
            return r > 0 && l < static_cast<uint64_t>(r);
        }
    }
};

template <typename LT, typename RT>
constexpr inline bool less_f(LT && l, RT && r)
{
    return less<remove_cvref_t<LT>, remove_cvref_t<RT>>()
        (std::forward<LT>(l), std::forward<RT>(r));
}

template <typename LT, typename RT>
constexpr inline bool less_equal_f(LT && l, RT && r)
{
    return !less<remove_cvref_t<RT>, remove_cvref_t<LT>>()
        (std::forward<RT>(r), std::forward<LT>(l));
}

template <typename LT, typename RT>
constexpr inline bool greater_equal_f(LT && l, RT && r)
{
    return !less<remove_cvref_t<LT>, remove_cvref_t<RT>>()
        (std::forward<LT>(l), std::forward<RT>(r));
}

}
