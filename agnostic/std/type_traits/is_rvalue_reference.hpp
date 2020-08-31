// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_RVALUE_REFERENCE

namespace std {

template <typename T> struct is_rvalue_reference : false_type {};
template <typename T> struct is_rvalue_reference<T&&> : true_type {};

template <typename T> constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif