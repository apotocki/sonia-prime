// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_FORWARD_ITERATOR_CONCEPT

#include "agnostic/std/iterator/concepts/input_iterator.hpp"

namespace std {

template<class I>
concept forward_iterator =
    input_iterator<I> &&
    derived_from<ITER_CONCEPT(I), forward_iterator_tag> &&
    incrementable<I> &&
    sentinel_for<I, I>;

}


#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <iterator>
#endif
