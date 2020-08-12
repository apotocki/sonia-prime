// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_RANDOM_ACCESS_ITERATOR_CONCEPT

#include "agnostic/std/concepts/same_as.hpp"
#include "agnostic/std/iterator/concepts/bidirectional_iterator.hpp"

namespace std {

template <class I>
concept random_access_iterator =
    bidirectional_iterator<I> &&
    derived_from<ITER_CONCEPT(I), random_access_iterator_tag> &&
    totally_ordered<I> &&
    sized_sentinel_for<I, I> &&
    requires(I i, const I j, const iter_difference_t<I> n) {
        { i += n } -> same_as<I&>;
        { j + n } -> same_as<I>;
        { n + j } -> same_as<I>;
        { i -= n } -> same_as<I&>;
        { j - n } -> same_as<I>;
        {  j[n]  } -> same_as<iter_reference_t<I>>;
    };

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <xutility>
#endif
