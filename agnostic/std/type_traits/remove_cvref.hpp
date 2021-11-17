// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_CVREF

#include "remove_cv.hpp"
#include "remove_reference.hpp"

namespace std {

template <class T> struct remove_cvref : remove_cv<remove_reference_t<T>> {};
template <class T> using remove_cvref_t = typename remove_cvref<T>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
