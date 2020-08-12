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

    if constexpr (std::is_trivial_v<item_t> && is_pointer_v<ForwardInputIteratorT> && is_pointer_v<ForwardOutputIteratorT>) {
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
ForwardOutputIteratorT copy(ForwardInputIteratorT ib, ForwardInputIteratorT ie, ForwardOutputIteratorT ob)
{
    for (; ib != ie; ++ib, ++ob) {
        *ob = *ib;
    }
    return ob;
}

template <typename ForwardInputIteratorT, typename ForwardOutputIteratorT>
auto copy(ForwardInputIteratorT ib, ForwardInputIteratorT ie, ForwardOutputIteratorT ob, ForwardOutputIteratorT oe)
{
    for (; ib != ie && ob != oe; ++ib, ++ob) {
        *ob = *ib;
    }
    return std::pair{ib, ob};
}

template <typename SrcIteratorT, typename RangeWriteInputIteratorT>
RangeWriteInputIteratorT copy_range(SrcIteratorT b, SrcIteratorT e, RangeWriteInputIteratorT it)
{
    for (;;) {
        iterator_value_t<RangeWriteInputIteratorT> orng = *it;
        auto orngb = boost::begin(orng), ornge = boost::end(orng);
        auto[iit, oit] = copy(b, e, orngb, ornge);
        if (iit == e) {
            if (oit != ornge) {
                *it = iterator_value_t<RangeWriteInputIteratorT>{oit, ornge};
            } else {
                ++it;
            }
            return std::move(it);
        }
        ++it;
        b = iit;
    }
    return std::move(it);
}

template <typename SrcRangeT, typename RangeWriteInputIteratorT>
RangeWriteInputIteratorT copy_range(SrcRangeT rng, RangeWriteInputIteratorT it)
{
    return copy_range(boost::begin(rng), boost::end(rng), std::move(it));
}

template <typename ForwardInputIteratorT, typename ForwardOutputIteratorT>
ForwardInputIteratorT copy_ranges_to_ranges(ForwardInputIteratorT irngs, ForwardOutputIteratorT & orngs)
{
    using in_range_type = iterator_value_t<ForwardInputIteratorT>;
    using out_range_type = iterator_value_t<ForwardOutputIteratorT>;
    if (irngs.empty()) return std::move(irngs);

    in_range_type irng = *irngs;
    auto ib = boost::begin(irng);
    auto ie = boost::end(irng);
    
    out_range_type orng = *orngs;
    auto ob = boost::begin(orng);
    auto oe = boost::end(orng);
    auto initial_ob = ob;

    for (;;)
    {
        if (ib == ie) {
            ++irngs;
            if (irngs.empty()) {
                *orngs = out_range_type(initial_ob, ob);
                return std::move(irngs);
            }
            irng = *irngs;
            ib = boost::begin(irng);
            ie = boost::end(irng);
            continue;
        }

        while (ob == oe) {
            ++orngs;
            orng = *orngs;
            initial_ob = ob = boost::begin(orng);
            auto oe = boost::end(orng);
        }

        std::tie(ib, ob) = sonia::copy(ib, ie, ob, oe);
    }
}

}

#endif // SONIA_ALGORITHM_COPY_HPP
