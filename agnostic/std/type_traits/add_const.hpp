// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ADD_CONST

namespace std {

template <typename T> struct add_const { using type = const T; };
template <typename T> using add_const_t = typename add_const<T>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
