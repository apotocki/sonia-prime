// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_NOTHROW_SWAPPABLE_WITH_HPP
#define AGNOSTIC_STD_IS_NOTHROW_SWAPPABLE_WITH_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "integral_constant.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_CONJUNCTION
#   include "conjunction.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_SWAPPABLE_WITH
#   include "is_swappable_with.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_DECLVAL
#   include "../utility/declval.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_SWAP
#   include "../algorithm/swap.hpp"
#endif

namespace std {

namespace detail {

template <class T, class U>
struct swap_cannot_throw : bool_constant<
       noexcept(swap(declval<T>(), declval<U>()))
    && noexcept(swap(declval<U>(), declval<T>()))
> {};

template <typename T, typename U>
struct is_nothrow_swappable_with_impl
    : bool_constant<conjunction_v<is_swappable_with_impl<T, U>, swap_cannot_throw<T, U>>> {};

}

template <typename T, typename U>
struct is_nothrow_swappable_with : detail::is_nothrow_swappable_with_impl<T, U>::type {};


template <typename T, typename U>
constexpr bool is_nothrow_swappable_with_v = is_nothrow_swappable_with<T, U>::value;

}

#endif // AGNOSTIC_STD_IS_NOTHROW_SWAPPABLE_WITH_HPP
