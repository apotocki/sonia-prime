// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ITER_DIFFERENCE

#include "agnostic/std/type_traits/conditional.hpp"
#include "agnostic/std/type_traits/remove_cvref.hpp"
#include "agnostic/std/type_traits/is_same.hpp"
#include "agnostic/std/iterator/incrementable_traits.hpp"
#include "agnostic/std/iterator/iterator_traits.hpp"
#include "agnostic/stdext/is_from_primary.hpp"

namespace std {

template <typename T>
using iter_difference_t = typename conditional_t<
    stdext::is_from_primary<iterator_traits<remove_cvref_t<T>>>,
    incrementable_traits<remove_cvref_t<T>>,
    iterator_traits<remove_cvref_t<T>>
>::difference_type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <iterator>
#endif
