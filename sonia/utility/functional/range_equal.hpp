//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>
#include <cstring>
#include <algorithm>

#include "sonia/type_traits.hpp"
#include "sonia/iterator_traits.hpp"
#include "equal.hpp"

namespace sonia {

struct range_equal
{
    template <typename LR, typename RR>
    constexpr bool operator()(LR && l, RR && r) const
    {
        using std::begin;
        using std::end;
        if constexpr (is_same_v<LR, RR>) {
            return std::addressof(l) == std::addressof(r) ||
                this->operator()(begin(l), end(l), begin(r), end(r));
        } else {
            return this->operator()(begin(l), end(l), begin(r), end(r));
        }
    }

    template <typename LForwardIteratorT, typename RForwardIteratorT>
    constexpr bool operator()(LForwardIteratorT lb, LForwardIteratorT le, RForwardIteratorT rb, RForwardIteratorT re) const
    {
        auto pair = std::mismatch(lb, le, rb, re, equal_to{});
        return pair.first == le && pair.second == re;
    }

    template <typename T>
    constexpr bool operator()(T const* lb, T const* le, T const* rb, T const* re) const
    {
        const size_t sz = le - lb;
        if (sz != size_t(re - rb)) return false;
        if (lb == rb) return true;
        if constexpr (is_integral_v<T>) {
            return !std::memcmp(lb, rb, sz * sizeof(T));
        } else {
            return std::equal(lb, le, rb);
        }
    }
};

template <typename LT, typename RT>
requires(has_range_iterator_v<LT> && has_range_iterator_v<RT>)
struct equal<LT, RT>
{
    template <typename LArgT, typename RArgT>
    constexpr bool operator()(LArgT && l, RArgT && r) const
    {
        return range_equal()(std::forward<LArgT>(l), std::forward<RArgT>(r));
    }
};

}
