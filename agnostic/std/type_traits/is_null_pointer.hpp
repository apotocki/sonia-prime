// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_NULL_POINTER_HPP
#define AGNOSTIC_STD_IS_NULL_POINTER_HPP

#pragma once

namespace std {

template <typename T> struct is_null_pointer : bool_constant<is_same_v<nullptr_t, remove_cv_t<T>>> {};
template <typename T> constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

}

#endif // AGNOSTIC_STD_IS_NULL_POINTER_HPP
