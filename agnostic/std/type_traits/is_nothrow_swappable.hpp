// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_NOTHROW_SWAPPABLE_HPP
#define AGNOSTIC_STD_IS_NOTHROW_SWAPPABLE_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ADD_LVALUE_REFERENCE
#   include "add_lvalue_reference.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_SWAPPABLE_WITH
#   include "type_traits/is_nothrow_swappable_with.hpp"
#endif

namespace std {

template <class T>
struct is_nothrow_swappable : detail::is_nothrow_swappable_with_impl<add_lvalue_reference_t<T>, add_lvalue_reference_t<T>>::type {};

template <typename T> constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;

}

#endif // AGNOSTIC_STD_IS_NOTHROW_SWAPPABLE_HPP
