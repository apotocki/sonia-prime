// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ADD_LVALUE_REFERENCE

#include "agnostic/std/type_traits/type_identity.hpp"

namespace std { namespace add_lvalue_reference_detail {

template <typename T> auto try_add_lvalue_reference(int)->type_identity<T&>;
template <typename T> auto try_add_lvalue_reference(...)->type_identity<T>;

}

template <typename T>
struct add_lvalue_reference : decltype(add_lvalue_reference_detail::try_add_lvalue_reference<T>(0)) {};

template <typename T> using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
