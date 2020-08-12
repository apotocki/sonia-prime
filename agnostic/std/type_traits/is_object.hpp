// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_OBJECT_HPP
#define AGNOSTIC_STD_IS_OBJECT_HPP

#pragma once

namespace std {

template <typename T>
struct is_object : negation<disjunction<is_function<T>, is_reference<T>, is_void<T>>>::type {};

template <typename T> constexpr bool is_object_v = is_object<T>::value;

}

#endif // AGNOSTIC_STD_IS_OBJECT_HPP
