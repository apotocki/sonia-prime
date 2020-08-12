// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE_HPP
#define AGNOSTIC_STD_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE_HPP

#pragma once

namespace std {

template <typename T> struct is_nothrow_default_constructible : is_nothrow_constructible<T> {};

template <typename T> constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<T>::value;

}

#endif // AGNOSTIC_STD_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE_HPP
