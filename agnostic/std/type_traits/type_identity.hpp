// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_TYPE_IDENTITY

namespace std {

template <typename T>
struct type_identity { using type = T; };

template< class T >
using type_identity_t = typename type_identity<T>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
