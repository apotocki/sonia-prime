//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_RANGE_LESS_HPP
#define SONIA_FUNCTIONAL_RANGE_LESS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <cstring>
#include <algorithm>
#include <memory>

#include <boost/utility/addressof.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/iterator_traits.hpp"
#include "less.hpp"

namespace sonia {

struct range_less
{
    template <typename LR, typename RR>
    constexpr bool operator()(LR && l, RR && r) const
    {
        if constexpr (is_same_v<remove_cvref_t<LR>, remove_cvref_t<RR>>) {
            if (BOOST_UNLIKELY(std::addressof(l) == std::addressof(r))) return false;
        }
        return this->operator()(boost::begin(l), boost::end(l), boost::begin(r), boost::end(r));
    }

    template <typename LForwardIteratorT, typename RForwardIteratorT>
    bool operator()(LForwardIteratorT lb, LForwardIteratorT le, RForwardIteratorT rb, RForwardIteratorT re) const
    {
        return std::lexicographical_compare(std::move(lb), std::move(le), std::move(rb), std::move(re));
    }

    template <typename T>
    bool operator()(T const* lb, T const* le, T const* rb, T const* re) const
    {
        const size_t lsz = le - lb;
        const size_t rsz = re - rb;
        if constexpr (is_integral_v<T> && 1 == sizeof(T)) {
            const int res = std::memcmp(lb, rb, (std::min)(lsz, rsz)); // bytes are interpreted as unsigned chars
            return (!res) ? lsz < rsz : res < 0;
        } else {
            return std::lexicographical_compare(lb, le, rb, re);
        }
    }
};

template <typename LT, typename RT>
struct less<LT, RT, enable_if_t<has_range_iterator_v<LT> && has_range_iterator_v<RT>>>
{
    template <typename LArgT, typename RArgT>
    constexpr bool operator()(LArgT && l, RArgT && r) const
    {
        return range_less()(std::forward<LArgT>(l), std::forward<RArgT>(r));
    }
};

}

#endif // SONIA_FUNCTIONAL_RANGE_LESS_HPP
