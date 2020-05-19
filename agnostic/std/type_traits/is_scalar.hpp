// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_SCALAR_HPP
#define AGNOSTIC_STD_IS_SCALAR_HPP

#pragma once

namespace std {

template <typename T> struct is_scalar : bool_constant<
    is_arithmetic_v<T> ||
    is_enum_v<T> ||
    is_pointer_v<T> ||
    is_member_pointer_v<T> ||
    is_null_pointer_v<T>
> {};

template <typename T> constexpr bool is_scalar_v = is_scalar<T>::value;

}

#endif // AGNOSTIC_STD_IS_SCALAR_HPP
