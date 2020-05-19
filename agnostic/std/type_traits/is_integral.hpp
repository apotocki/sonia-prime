// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_INTEGRAL_HPP
#define AGNOSTIC_STD_IS_INTEGRAL_HPP

#pragma once

namespace std {

template <typename T> struct is_integral : bool_constant<
    is_any_of_v<
        remove_cv_t<T>, 
        bool, char, signed char, unsigned char, wchar_t,
#if defined(__cpp_char8_t) || !defined (_MSC_VER)
    char8_t,
#endif
        char16_t, char32_t, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long
    >>{};

template <typename T> constexpr bool is_integral_v = is_integral<T>::value;

}

#endif // AGNOSTIC_STD_IS_INTEGRAL_HPP
