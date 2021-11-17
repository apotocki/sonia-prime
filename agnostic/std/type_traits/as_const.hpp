// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_AS_CONST_HPP
#define AGNOSTIC_STD_AS_CONST_HPP

#pragma once

namespace std {

template <typename T>
constexpr add_const_t<T> & as_const(T& t) noexcept { return t; }
    
template <typename T> void as_const(const T&&) = delete;

}

#endif // AGNOSTIC_STD_AS_CONST_HPP
