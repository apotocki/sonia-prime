// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_SWAPPABLE_WITH_HPP
#define AGNOSTIC_STD_IS_SWAPPABLE_WITH_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "integral_constant.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_VOID
#   include "void.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_CONJUNCTION
#   include "conjunction.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_DECLVAL
#   include "../utility/declval.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_SWAP
#   include "../algorithm/swap.hpp"
#endif

namespace std {

namespace detail {

template <class T1, class T2, class = void> struct is_swappable_with_helper : false_type {};

template <class T1, class T2>
struct is_swappable_with_helper<T1, T2, void_t<decltype(swap(declval<T1>(), declval<T2>()))>> : true_type {};

template <class T1, class T2>
struct is_swappable_with_impl
    : bool_constant<conjunction_v<is_swappable_with_helper<T1, T2>, is_swappable_with_helper<T2, T1>>>
{};

}

template <class T, class U>
struct is_swappable_with : detail::is_swappable_with_impl<T, U>::type {};

template <class T, class U>
constexpr bool is_swappable_with_v = is_swappable_with<T, U>::value;

}

#endif
