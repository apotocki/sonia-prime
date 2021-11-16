// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_INTEGRAL

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "agnostic/std/type_traits/remove_cv.hpp"
#include "agnostic/stdext/is_any_of.hpp"

namespace std {

template <typename T> struct is_integral : bool_constant<
    stdext::is_any_of_v<
        remove_cv_t<T>, 
        bool, char, signed char, unsigned char, wchar_t,
#if defined(__cpp_char8_t) || !defined (_MSC_VER)
    char8_t,
#endif
        char16_t, char32_t, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long
    >>{};

template <typename T> constexpr bool is_integral_v = is_integral<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
