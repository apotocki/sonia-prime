//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_RANGE_EQUAL_HPP
#define SONIA_FUNCTIONAL_RANGE_EQUAL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <cstring>
#include <algorithm>
#include <boost/utility/addressof.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace sonia {

struct range_equal {
    template <typename LR, typename RR>
    bool operator()(LR const& l, RR const& r) const {
        return boost::addressof(l) == boost::addressof(r) ||
            this->operator()(boost::begin(l), boost::end(l), boost::begin(r), boost::end(r));
    }

    template <typename LForwardIteratorT, typename RForwardIteratorT>
    bool operator()(LForwardIteratorT lb, LForwardIteratorT le, RForwardIteratorT rb, RForwardIteratorT re) const {
        auto pair = std::mismatch(lb, le, rb, re);
        return pair.first == le && pair.second == re;
    }

    template <typename T>
    bool operator()(T const* lb, T const* le, T const* rb, T const* re) const {
        const size_t sz = le - lb;
        if (sz != (re - rb)) return false;
        if (lb == rb) return true;
        if constexpr (is_integral_v<T>) {
            return !std::memcmp(lb, rb, sz * sizeof(T));
        } else {
            return std::equal(lb, le, rb);
        }
    }
};

}

#endif // SONIA_FUNCTIONAL_RANGE_EQUAL_HPP
