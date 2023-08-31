//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <cstring>
#include <algorithm>
#include <memory>

#include "sonia/type_traits.hpp"
#include "sonia/iterator_traits.hpp"
#include "compare.hpp"

namespace sonia {

struct range_compare
{
    template <typename LR, typename RR>
    constexpr int operator()(LR && l, RR && r) const
    {
        if constexpr (is_same_v<remove_cvref_t<LR>, remove_cvref_t<RR>>) {
            if (BOOST_UNLIKELY(std::addressof(l) == std::addressof(r))) return 0;
        }
        return this->operator()(std::begin(l), std::end(l), std::begin(r), std::end(r));
    }

    template <typename LForwardIteratorT, typename RForwardIteratorT>
    constexpr int operator()(LForwardIteratorT lb, LForwardIteratorT le, RForwardIteratorT rb, RForwardIteratorT re) const
    {
        auto pair = std::mismatch(lb, le, rb, re);
        if (pair.first == le) {
            return pair.second == re ? 0 : -1;
        } else {
            return pair.second == re ? 0 : compare_f(*pair.first, *pair.second);
        }
    }

    template <typename T>
    constexpr int operator()(T const* lb, T const* le, T const* rb, T const* re) const
    {
        const size_t lsz = le - lb;
        const size_t rsz = re - rb;
        if constexpr (is_integral_v<T> && 1 == sizeof(T)) {
            return std::memcmp(lb, rb, (std::min)(lsz, rsz)); // bytes are interpreted as unsigned chars
        } else {
            return std::lexicographical_compare(lb, le, rb, re);
        }
    }
};

template <typename LT, typename RT>
struct compare<LT, RT, enable_if_t<has_range_iterator_v<LT> && has_range_iterator_v<RT>>>
{
    template <typename LArgT, typename RArgT>
    constexpr int operator()(LArgT && l, RArgT && r) const
    {
        return range_compare()(std::forward<LArgT>(l), std::forward<RArgT>(r));
    }
};

}
