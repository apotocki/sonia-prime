// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_COPY_CONSTRUCTIBLE_HPP
#define AGNOSTIC_STD_IS_COPY_CONSTRUCTIBLE_HPP

#pragma once

namespace std {

template <typename T> struct is_copy_constructible
    : is_constructible<T, add_lvalue_reference_t<add_const_t<T>>> {};
    
template <typename T> constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

}

#endif // AGNOSTIC_STD_IS_COPY_CONSTRUCTIBLE_HPP
