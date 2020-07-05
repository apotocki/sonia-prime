// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_MEMBER_POINTER

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "agnostic/std/type_traits/remove_cv.hpp"

namespace std {

namespace detail {

template <typename T> struct is_member_pointer_impl : false_type {};
template <typename T, typename U> struct is_member_pointer_impl<T U::*> : true_type {};

}

template <typename T> struct is_member_pointer : detail::is_member_pointer_impl<remove_cv_t<T>> {};
template <typename T> constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
