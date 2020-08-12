// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_BIDIRECTIONAL_ITERATOR_CONCEPT

#include "agnostic/std/concepts/same_as.hpp"
#include "agnostic/std/iterator/concepts/forward_iterator.hpp"

namespace std {

template<class I>
concept bidirectional_iterator =
    forward_iterator<I> &&
    derived_from<ITER_CONCEPT(I), bidirectional_iterator_tag> &&
    requires(I i) {
        { --i } -> same_as<I&>;
        { i-- } -> same_as<I>;
};

}


#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <xutility>
#endif
