// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_MAKE_SIGNED

#include "agnostic/std/type_traits/is_integral.hpp"
#include "agnostic/std/type_traits/is_same.hpp"
#include "agnostic/std/type_traits/remove_cv.hpp"
#include "agnostic/std/type_traits/is_enum.hpp"

namespace std {

namespace make_signed_detail {

template <typename T> struct make_signed_impl { using type = T; };
template <> struct make_signed_impl<char> { using type = signed char; };
template <> struct make_signed_impl<unsigned char> { using type = signed char; };
template <> struct make_signed_impl<unsigned short> { using type = signed short; };
template <> struct make_signed_impl<unsigned int> { using type = signed int; };
template <> struct make_signed_impl<unsigned long> { using type = signed long; };
template <> struct make_signed_impl<unsigned long long> { using type = signed long long; };

}

template <typename T> struct make_signed {
    static_assert((is_integral_v<T> && !is_same_v<bool, remove_cv_t<T>>) || is_enum_v<_Ty>,
        "make_signed<T> requires that T shall be a (possibly cv-qualified) "
        "integral type or enumeration but not a bool type.");

    using type = typename make_signed_detail::make_signed_impl<T>::type;
};

template <typename T> using make_signed_t = typename make_signed<remove_cv_t<T>>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
