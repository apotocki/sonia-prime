// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ENABLE_IF

namespace std {

template <bool B, typename T = void> struct enable_if {};

template <typename T> struct enable_if<true, T> { using type = T; };

template <bool B, typename T = void> using enable_if_t = typename enable_if<B, T>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
