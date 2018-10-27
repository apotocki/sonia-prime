//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_ALGORITHM_CONSTRUCT_HPP
#define SONIA_ALGORITHM_CONSTRUCT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <cstring>
#include <utility>

#include <boost/range/size.hpp>
#include <boost/range/begin.hpp>

#include "sonia/type_traits.hpp"

namespace sonia {

template <typename ElementT, class RngT>
void construct(ElementT * dest, RngT && src, size_t sz) {
    if constexpr (is_pod_v<ElementT> && is_same_v<remove_cv_t<range_value_t<RngT>>, ElementT> && is_pointer_v<range_iterator_t<RngT>>) {
        std::memcpy(dest, boost::begin(src), sizeof(ElementT) * sz);
    } else if constexpr (is_pod_v<ElementT> && is_same_v<remove_cv_t<range_value_t<RngT>>, ElementT>) {
        std::copy_n(boost::begin(src), sz, dest);
    } else {
        for (auto s = boost::begin(src); sz != 0; ++dest, ++s, --sz) {
            new (dest) ElementT(*s);
        }
    }
}

template <typename ElementT>
void construct(ElementT * dest, size_t sz) {
    if constexpr (is_trivially_constructible_v<ElementT>) {
        // do nothing
    } else {
        for (; sz != 0; ++dest, --sz) {
            new (dest) ElementT;
        }
    }
}

template <typename ElementT, class RngT>
void move_construct(ElementT * dest, RngT && src, size_t sz) {
    if constexpr (is_pod_v<ElementT> && is_same_v<remove_cv_t<range_value_t<RngT>>, ElementT> && is_pointer_v<range_iterator_t<RngT>>) {
        std::memcpy(dest, boost::begin(src), sizeof(ElementT) * sz);
    } else if constexpr (is_pod_v<ElementT> && is_same_v<remove_cv_t<range_value_t<RngT>>, ElementT>) {
        std::copy_n(boost::begin(src), sz, dest);
    } else {
        for (auto s = boost::begin(src); sz != 0; ++dest, ++s, --sz) {
            new (dest) ElementT(std::move(*s));
        }
    }
}

template <typename ElementT, class RngT>
void construct(ElementT * dest, RngT && src) {
    construct(dest, std::forward<RngT>(src), boost::size(src));
}

}

#endif // SONIA_ALGORITHM_CONSTRUCT_HPP
