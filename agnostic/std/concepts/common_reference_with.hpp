// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_COMMON_REFERENCE_WITH_CONCEPT

#include "agnostic/std/concepts/same_as.hpp"
#include "agnostic/std/concepts/convertible_to.hpp"
#include "agnostic/std/type_traits/common_reference.hpp"

namespace std {

template <class T, class U>
concept common_reference_with =
    same_as<std::common_reference_t<T, U>, common_reference_t<U, T>> &&
    convertible_to<T, common_reference_t<T, U>> &&
    convertible_to<U, common_reference_t<T, U>>;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
