// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_INCREMENTABLE_TRAITS

#include "agnostic/std/type_traits/is_object.hpp"
#include "agnostic/std/type_traits/make_signed.hpp"
#include "agnostic/std/utility/declval.hpp"
#include "agnostic/std/concepts/integral.hpp"

namespace std {

template <typename> struct incrementable_traits {};

template <typename T>
requires is_object_v<T>
struct incrementable_traits<T*> { using difference_type = ptrdiff_t; };

template <class T>
struct incrementable_traits<const T> : incrementable_traits<T> {};

template <typename T>
requires requires { typename T::difference_type; }
struct incrementable_traits<T>
{
    using difference_type = typename T::difference_type;
};

template <typename T>
requires (!requires { typename T::difference_type; } &&
    requires(const T& a, const T& b) { { a - b } -> integral; })
struct incrementable_traits<T>
{
    using difference_type = make_signed_t<decltype(declval<T>() - declval<T>())>;
};

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <iterator>
#endif
