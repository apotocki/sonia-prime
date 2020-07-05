// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_INPUT_ITERATOR_CONCEPT

#include "agnostic/std/iterator/tags.hpp"
#include "agnostic/std/iterator/concepts/input_or_output_iterator.hpp"
#include "agnostic/std/concepts/derived_from.hpp"
#include "agnostic/stdext/iter_concept.hpp"

namespace std {

template<class I>
concept input_iterator =
    input_or_output_iterator<I> &&
    indirectly_readable<I> &&
    requires { typename stdext::iter_concept<I>; } &&
    derived_from<stdext::iter_concept<I>, input_iterator_tag>;

}


#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <xutility>
#endif
