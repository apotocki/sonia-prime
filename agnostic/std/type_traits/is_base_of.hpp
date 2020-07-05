// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_BASE_OF

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "agnostic/std/type_traits/is_class.hpp"

#include "agnostic/std/type_traits/remove_cv.hpp"
#include "agnostic/std/type_traits/conditional.hpp"
#include "agnostic/std/type_traits/is_array.hpp"
#include "agnostic/std/type_traits/is_function.hpp"
#include "agnostic/std/type_traits/add_pointer.hpp"


namespace std { namespace is_base_of_detail {
   
template <typename B>
true_type  test_pre_ptr_convertible(const volatile B*);

template <typename>
false_type test_pre_ptr_convertible(const volatile void*);

template <typename, typename>
auto test_pre_is_base_of(...)->true_type;

template <typename B, typename D>
auto test_pre_is_base_of(int) ->
    decltype(test_pre_ptr_convertible<B>(static_cast<D*>(nullptr)));

}

template <typename Base, typename Derived>
struct is_base_of : bool_constant<
    is_class_v<Base> && is_class_v<Derived> &&
    decltype(is_base_of_detail::test_pre_is_base_of<Base, Derived>(0))::value
> {};

template <class B, class D> using is_base_of_t = typename is_base_of<B, D>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
