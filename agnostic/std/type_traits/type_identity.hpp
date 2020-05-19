// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_TYPE_IDENTITY_HPP
#define AGNOSTIC_STD_TYPE_IDENTITY_HPP

#pragma once

namespace std {

template <typename T>
struct type_identity { using type = T; };

template< class T >
using type_identity_t = typename type_identity<T>::type;

}

#endif // AGNOSTIC_STD_TYPE_IDENTITY_HPP
