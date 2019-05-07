//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_ALGORITHM_COPY_HPP
#define SONIA_ALGORITHM_COPY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <cstring>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include "sonia/iterator_traits.hpp"

namespace sonia {

template <typename ForwardInputIteratorT, typename ForwardOutputIteratorT>
auto copy_not_more(
    ForwardInputIteratorT b, ForwardInputIteratorT e,
    ForwardOutputIteratorT oit,
    size_t n)
{
    using item_t = iterator_value_t<ForwardInputIteratorT>;

    if constexpr (is_pod_v<item_t> && is_pointer_v<ForwardInputIteratorT> && is_pointer_v<ForwardOutputIteratorT>) {
        static_assert(is_same_v <item_t, iterator_value_t<ForwardOutputIteratorT>>);
        size_t sz = (std::min)(static_cast<size_t>(e - b) / sizeof(item_t), n);
        std::memcpy(oit, b, sz * sizeof(item_t));
        return std::tuple(b + sz, oit + sz);
    } else {
        for (; b != e && n; --n) {
            *oit = *b;
            ++oit; ++b;
        }
        return std::tuple(std::move(b), std::move(oit));
    }
}

template <typename ForwardInputIteratorT, typename ForwardOutputIteratorT>
auto copy(ForwardInputIteratorT ib, ForwardInputIteratorT ie, ForwardOutputIteratorT ob, ForwardOutputIteratorT oe)
{
    for (; ib != ie && ob != oe; ++ib, ++ob) {
        *ob = *ib;
    }
    return std::pair{ib, ob};
}

template <typename SrcRangeT, typename RangeWriteInputIteratorT>
RangeWriteInputIteratorT copy_range(SrcRangeT rng, RangeWriteInputIteratorT it)
{
    auto iit0 = boost::begin(rng);
    for (;;) {
        iterator_value_t<RangeWriteInputIteratorT> orng = *it;
        auto[iit, oit] = copy(iit0, boost::end(rng), boost::begin(orng), boost::end(orng));
        if (iit == boost::end(rng)) {
            if (oit != boost::end(orng)) {
                *it = iterator_value_t<RangeWriteInputIteratorT>{boost::begin(orng), oit};
            }
            ++it;
            return std::move(it);
        }
        ++it;
        iit0 = iit;
    }
    return std::move(it);
}

}

#endif // SONIA_ALGORITHM_COPY_HPP
