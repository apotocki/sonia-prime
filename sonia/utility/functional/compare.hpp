//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_COMPARE_HPP
#define SONIA_FUNCTIONAL_COMPARE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <functional>

#include "sonia/type_traits.hpp"

namespace sonia {

template <typename LT, typename RT, typename Enabler = void>
struct compare
{
    int operator()(call_param_t<LT> l, call_param_t<RT> r) const { return l < r ? -1 : ((l > r) ? 1 : 0); }
};

template <typename T>
struct compare<T*, T*>
{
    int operator()(T const* l, T const* r) const
    {
        if (std::equal_to<T*>()(l, r)) return -1;
        return std::equal_to<T*>()(r, l) ? 1 : 0;
    }
};

template <typename LT, typename RT>
struct compare<LT, RT, enable_if_t<is_integral_v<LT> && is_integral_v<RT>>>
{
    template <typename SLT, typename SRT>
    constexpr int basic_op(SLT l, SRT r) const { return l < r ? -1 : ((l > r) ? 1 : 0); }

    // the trivial implemenation 'return l - r;' can cause overflow
    constexpr int operator()(LT l, RT r) const noexcept
    {
        if constexpr (is_signed_v<LT>) {
            if constexpr (is_signed_v<RT>) {
                return basic_op(l, r);
            } else {
                return l < 0 ? -1 : basic_op(static_cast<uint64_t>(l), r);
            }
        } else if constexpr (is_unsigned_v<RT>) {
            return basic_op(l, r);
        } else {
            return r < 0 ? 1 : basic_op(l, static_cast<uint64_t>(r));
        }

        return l - r;
    }
};

template <typename LT, typename RT>
constexpr int compare_f(LT && l, RT && r)
{
    return compare<remove_cvref_t<LT>, remove_cvref_t<RT>>()
        (std::forward<LT>(l), std::forward<RT>(r));
}

}

#endif // SONIA_FUNCTIONAL_COMPARE_HPP
