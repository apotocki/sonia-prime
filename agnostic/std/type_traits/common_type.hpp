// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_COMMON_TYPE

#include "agnostic/std/type_traits/void.hpp"
#include "agnostic/std/type_traits/decay.hpp"
#include "agnostic/stdext/cond_res.hpp"

namespace std {

template <class...> struct common_type {};

template <class... Ts> using common_type_t = typename common_type<Ts...>::type;

template <class T> struct common_type<T> : common_type<T, T> {};

namespace common_type_detail {

template <class T1, class T2, class = void> struct two_implext {};

template <class T1, class T2>
struct two_implext<T1, T2, void_t<stdext::cond_res<T1 const&, T2 const&>>>
    : decay_t<stdext::cond_res<T1 const&, T2 const&>>
{};

template <class T1, class T2, class = void> struct two_impl : two_implext<T1, T2> {};

template <class T1, class T2>
struct two_impl<T1, T2, void_t<stdext::cond_res<T1, T2>>>
    : decay_t<stdext::cond_res<T1, T2>>
{};

}

template <class T1, class T2>
struct common_type<T1, T2> : common_type_detail::two_impl<decay_t<T1>, decay_t<T2>> {};

namespace common_type_detail {

template <class AlwaysVoid, class T1, class T2, class...R>
struct multi_impl {};

template <class T1, class T2, class...R>
struct multi_impl<void_t<typename common_type<T1, T2>::type>, T1, T2, R...>
    : common_type<common_type_t<T1, T2>, R...> {};

}

template <class T1, class T2, class... R>
struct common_type<T1, T2, R...>
    : common_type_detail::multi_impl<void, T1, T2, R...> {};

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
