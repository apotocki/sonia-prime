// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_TRIVIALLY_COPY_ASSIGNABLE_HPP
#define AGNOSTIC_STD_IS_TRIVIALLY_COPY_ASSIGNABLE_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_ASSIGNABLE
#   include "is_trivially_assignable.hpp"
#endif

namespace std {

template <class T>
struct is_trivially_copy_assignable : is_trivially_assignable<T&, const T&> {};

template <class T>
constexpr bool is_trivially_copy_assignable_v = is_trivially_copy_assignable<T>::value;

}

#endif // AGNOSTIC_STD_IS_TRIVIALLY_COPY_ASSIGNABLE_HPP
