// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_COPY_CONSTRUCTIBLE

#include "agnostic/std/type_traits/add_const.hpp"
#include "agnostic/std/type_traits/add_lvalue_reference.hpp"
#include "agnostic/std/type_traits/is_constructible.hpp"

namespace std {

template <typename T> struct is_copy_constructible
    : is_constructible<T, add_lvalue_reference_t<add_const_t<T>>> {};
    
template <typename T> constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
