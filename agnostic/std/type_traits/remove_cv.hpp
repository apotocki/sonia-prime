// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_CV

#include "remove_const.hpp"
#include "remove_volatile.hpp"

namespace std {

template <typename T> struct remove_cv { using type = remove_volatile_t<remove_const_t<T>>; };
template <typename T> using remove_cv_t = typename remove_cv<T>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
