// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_CLASS_HPP
#define AGNOSTIC_STD_IS_CLASS_HPP

#pragma once

namespace std {

#if 1

template <typename T> struct is_class : bool_constant<__is_class(T)> {};
template <typename T> constexpr bool is_class_v = __is_class(T);

#else

namespace is_class_detail {

template <class T> char test(int T::*);
struct two { char c[2]; };
template <class T> two test(...);

}

template <class T>
struct is_class : bool_constant<sizeof(detail::test<T>(0)) == 1 && !is_union_v<T>> {};
template <typename T> constexpr bool is_class_v = is_class<T>::value;

#endif

}

#endif // AGNOSTIC_STD_IS_CLASS_HPP
