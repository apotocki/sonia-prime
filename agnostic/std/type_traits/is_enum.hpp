// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_ENUM_HPP
#define AGNOSTIC_STD_IS_ENUM_HPP

#pragma once

namespace std {

template <typename T> struct is_enum : bool_constant<__is_enum(T)> {};

template <typename T> constexpr bool is_enum_v = __is_enum(T);

}

#endif // AGNOSTIC_STD_IS_ENUM_HPP
