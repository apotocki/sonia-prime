// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_MEMBER_POINTER_HPP
#define AGNOSTIC_STD_IS_MEMBER_POINTER_HPP

#pragma once

namespace std {

namespace detail {

template <typename T> struct is_member_pointer_impl : false_type {};
template <typename T, typename U> struct is_member_pointer_impl<T U::*> : true_type {};

}

template <typename T> struct is_member_pointer : detail::is_member_pointer_impl<remove_cv_t<T>> {};
template <typename T> constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

}

#endif // AGNOSTIC_STD_IS_MEMBER_POINTER_HPP
