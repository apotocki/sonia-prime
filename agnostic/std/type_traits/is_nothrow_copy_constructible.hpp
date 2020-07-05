// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_COPY_CONSTRUCTIBLE

#include "agnostic/std/type_traits/add_rvalue_reference.hpp"
#include "agnostic/std/type_traits/add_const.hpp"

namespace std {

template <typename T> struct is_nothrow_copy_constructible : is_nothrow_constructible<T, add_lvalue_reference_t<add_const_t<T>>> {};

template <typename T> constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
