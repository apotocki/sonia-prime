// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_INPUT_OR_OUTPUT_ITERATOR_CONCEPT

#include "agnostic/stdext/can_reference.hpp"
#include "agnostic/std/iterator/concepts/weakly_incrementable.hpp"

namespace std {

template <class I>
concept input_or_output_iterator =
    requires(I i) {
        { *i } -> stdext::can_reference;
    }
    && weakly_incrementable<I>;
}


#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <iterator>
#endif
