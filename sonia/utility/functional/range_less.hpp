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
#include <boost/utility/addressof.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace sonia {

struct range_less {
    template <typename LR, typename RR>
    bool operator()(LR const& l, RR const& r) const {
        return boost::addressof(l) != boost::addressof(r) &&
            this->operator()(boost::begin(l), boost::end(l), boost::begin(r), boost::end(r));
    }

    template <typename LForwardIteratorT, typename RForwardIteratorT>
    bool operator()(LForwardIteratorT lb, LForwardIteratorT le, RForwardIteratorT rb, RForwardIteratorT re) const {
        return std::lexicographical_compare(std::move(lb), std::move(le), std::move(rb), std::move(re));
    }

    template <typename T>
    bool operator()(T const* lb, T const* le, T const* rb, T const* re) const {
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

}

#endif // SONIA_FUNCTIONAL_RANGE_LESS_HPP
