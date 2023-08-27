//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <functional>

#include "sonia/type_traits.hpp"

namespace sonia {

template <typename LT, typename RT>
struct equal
{
    template <typename LArgT, typename RArgT>
    inline bool operator()(LArgT && l, RArgT && r) const
    {
        if constexpr (is_same_v<LT, RT>) {
            return std::equal_to<LT>()(std::forward<LArgT>(l), std::forward<RArgT>(r));
        } else if constexpr (requires{ !std::equal_to<>()(std::forward<LArgT>(l), std::forward<RArgT>(r)); }) {
            return std::equal_to<>()(std::forward<LArgT>(l), std::forward<RArgT>(r));
        } else {
            return false;
        }
    }
};

template <typename LT, typename RT>
requires(is_integral_v<LT> && is_integral_v<RT>)
struct equal<LT, RT>
{
    bool operator()(LT l, RT r) const noexcept
    {
        if constexpr (is_signed_v<LT>) {
            if constexpr (is_signed_v<RT>) return l == r;
            return l >= 0 && static_cast<uint64_t>(l) == r;
        } else if constexpr (is_unsigned_v<RT>) {
            return l == r;
        } else {
            return r >= 0 && l == static_cast<uint64_t>(r);
        }
    }
};

template <typename LT, typename RT>
inline bool equal_f(LT && l, RT && r)
{
    return equal<remove_cvref_t<LT>, remove_cvref_t<RT>>()
        (std::forward<LT>(l), std::forward<RT>(r));
}

struct equal_to
{
    template <typename LT, typename RT>
    bool operator()(LT && l, RT && r) const
    {
        return equal<remove_cvref_t<LT>, remove_cvref_t<RT>>()
            (std::forward<LT>(l), std::forward<RT>(r));
    }
};

}
