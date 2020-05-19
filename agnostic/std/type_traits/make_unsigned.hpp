// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_MAKE_UNSIGNED_HPP
#define AGNOSTIC_STD_MAKE_UNSIGNED_HPP

#pragma once

namespace std {

namespace make_unsigned_detail {

template <typename T> struct make_unsigned_impl { using type = T; };
template <> struct make_unsigned_impl<char> { using type = unsigned char; };
template <> struct make_unsigned_impl<signed char> { using type = unsigned char; };
template <> struct make_unsigned_impl<short> { using type = unsigned short; };
template <> struct make_unsigned_impl<int> { using type = unsigned int; };
template <> struct make_unsigned_impl<long> { using type = unsigned long; };
template <> struct make_unsigned_impl<long long> { using type = unsigned long long; };

}

template <typename T> struct make_unsigned {
    static_assert((is_integral_v<T> && !is_same_v<bool, remove_cv_t<T>>) || is_enum_v<_Ty>,
        "make_unsigned<T> requires that T shall be a (possibly cv-qualified) "
        "integral type or enumeration but not a bool type.");

    using type = typename make_unsigned_detail::make_unsigned_impl<T>::type;
};

template <typename T> using make_unsigned_t = typename make_unsigned<T>::type;

}

#endif // AGNOSTIC_STD_MAKE_UNSIGNED_HPP
