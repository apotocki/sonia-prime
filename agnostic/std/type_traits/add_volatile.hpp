// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ADD_VOLAATILE_HPP
#define AGNOSTIC_STD_ADD_VOLAATILE_HPP

#pragma once

namespace std {

template <typename T> struct add_volatile { using type = volatile T; };
template <typename T> using add_volatile_t = typename add_volatile<T>::type;

}

#endif // AGNOSTIC_STD_ADD_VOLAATILE_HPP
