// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_HPP
#define AGNOSTIC_STD_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_HPP

#pragma once

namespace std {

namespace detail {

template <typename T>
void implicitly_default_construct(T const&);

}

template <typename T, typename = void>
struct is_implicitly_default_constructible : false_type {};

template <typename T>
struct is_implicitly_default_constructible<T, void_t<decltype(detail::implicitly_default_construct<T>({}))>> : true_type {};

template <typename T> constexpr bool is_implicitly_default_constructible_v = is_implicitly_default_constructible<T>::value;

}

#endif // AGNOSTIC_STD_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_HPP
