// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_OBJECT

#include "agnostic/std/type_traits/is_function.hpp"
#include "agnostic/std/type_traits/is_reference.hpp"
#include "agnostic/std/type_traits/is_void.hpp"
#include "agnostic/std/type_traits/negation.hpp"

namespace std {

template <typename T>
struct is_object : negation<disjunction<is_function<T>, is_reference<T>, is_void<T>>>::type {};

template <typename T> constexpr bool is_object_v = is_object<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
