// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_NULL_POINTER

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "agnostic/std/type_traits/is_same.hpp"
#include "agnostic/std/type_traits/remove_cv.hpp"

namespace std {

template <typename T> struct is_null_pointer : bool_constant<is_same_v<nullptr_t, remove_cv_t<T>>> {};
template <typename T> constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
