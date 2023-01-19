//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/iterator_traits.hpp"
#include "sonia/array_view.hpp"
#include "sonia/span.hpp"
#include <boost/range/iterator_range.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace sonia {

template <class RangeT>
struct range_reference
{
    using iterator = conditional_t<is_const_v<RangeT>, range_const_iterator_t<RangeT>, range_iterator_t<RangeT>>;
    using type = boost::iterator_range<iterator>;

    static type make(RangeT & e) { return type(boost::begin(e), boost::end(e)); }
    static type make(iterator b, iterator e) { return type(std::move(b), std::move(e)); }
};

template <class T>
struct range_reference<array_view<T>>
{
    using iterator = T*;
    using type = array_view<T>;

    static type make(array_view<T> rng) { return rng; }
    static type make(iterator b, iterator e) { return array_view<T>(b, e); }
};

template <class T>
struct range_reference<std::span<T>>
{
    using iterator = typename std::span<T>::iterator;
    using type = std::span<T>;

    static type make(std::span<T> rng) { return rng; }
    static type make(iterator b, iterator e) { return std::span<T>(b, e); }
};

template <class RangeT> using range_reference_t = typename range_reference<RangeT>::type;

}
