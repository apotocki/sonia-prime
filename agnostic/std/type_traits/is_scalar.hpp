// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_SCALAR

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "agnostic/std/type_traits/is_arithmetic.hpp"
#include "agnostic/std/type_traits/is_enum.hpp"
#include "agnostic/std/type_traits/is_pointer.hpp"
#include "agnostic/std/type_traits/is_member_pointer.hpp"
#include "agnostic/std/type_traits/is_null_pointer.hpp"

namespace std {

template <typename T> struct is_scalar : bool_constant<
    is_arithmetic_v<T> ||
    is_enum_v<T> ||
    is_pointer_v<T> ||
    is_member_pointer_v<T> ||
    is_null_pointer_v<T>
> {};

template <typename T> constexpr bool is_scalar_v = is_scalar<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
