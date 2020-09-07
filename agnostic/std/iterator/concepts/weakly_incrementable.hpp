// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_WEAKLY_INCREMENTABLE_CONCEPT

#include "agnostic/std/concepts/default_initializable.hpp"
#include "agnostic/std/concepts/movable.hpp"
#include "agnostic/std/concepts/same_as.hpp"
#include "agnostic/std/concepts/signed_integral.hpp"

namespace std {

template<class I>
concept weakly_incrementable =
    default_initializable<I> && movable<I> &&
    requires(I i) {
        typename iter_difference_t<I>;
        requires signed_integral<iter_difference_t<I>>;
        { ++i } -> same_as<I&>;   // not required to be equality-preserving
        i++;                      // not required to be equality-preserving
    };
}


#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <iterator>
#endif
