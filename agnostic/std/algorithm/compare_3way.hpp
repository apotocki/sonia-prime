// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_COMPARE_3WAY

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "agnostic/std/type_traits/void.hpp"
#include "agnostic/std/utility/declval.hpp"
#include "agnostic/std/compare/ordering.hpp"
#include "agnostic/std/compare/equality.hpp"

#include "agnostic/utility/dependent_bool.hpp"

namespace std::compare_3way_detail {

template <typename L, typename R, typename Enabler = void>
struct has_spaceship_operator : false_type {};

template <typename L, typename R>
struct has_spaceship_operator<L, R, void_t<decltype(declval<const L&>() <=> declval<const R&>())>> : true_type {};

template <typename L, typename R, typename Enabler = void>
struct has_equal_operator : false_type {};

template <typename L, typename R>
struct has_equal_operator<L, R, void_t<decltype(declval<const L&>() == declval<const R&>())>> : true_type {};

template <typename L, typename R, typename Enabler = void>
struct has_less_and_equal_operators : has_equal_operator<L, R> {};

template <typename L, typename R>
struct has_less_and_equal_operators<L, R, void_t<decltype(declval<const L&>() < declval<const R&>())>> : true_type {};

}

namespace std {

template <typename L, typename R>
auto compare_3way(L const& l, R const& r)
{
    if constexpr (compare_3way_detail::has_spaceship_operator<L, R>::value) {
        return l <=> r;
    } else if constexpr (compare_3way_detail::has_less_and_equal_operators<L, R>::value) {
        return l == r ? strong_ordering::equal : (l < r ? strong_ordering::less : strong_ordering::greater);
    } else if constexpr (compare_3way_detail::has_equal_operator<L, R>::value) {
        return l == r ? strong_equality::equal : strong_equality::unequal;
    } else {
        static_assert(agnostic::dependent_false<L>);
    }
};

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <compare>
#endif
