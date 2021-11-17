// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_COMMON_REFERENCE

#include "agnostic/std/type_traits/void.hpp"
#include "agnostic/std/type_traits/remove_cvref.hpp"
#include "agnostic/std/type_traits/common_type.hpp"
//#include "agnostic/std/type_traits/enable_if.hpp"

#include "agnostic/stdext/copy_cv.hpp"
#include "agnostic/stdext/cond_res.hpp"
#include "agnostic/stdext/add_qualifiers.hpp"

namespace std {
    
template <class T, class U, template<class> class TQual, template<class> class UQual>
struct basic_common_reference { };

namespace common_reference_detail {

template<typename, typename, typename = void> struct common_ref_impl {};

// If A and B are both lvalue reference types, ...
template <typename X, typename Y>
struct common_ref_impl<X&, Y&, void_t<stdext::cond_res<stdext::copy_cv<X, Y>&, stdext::copy_cv<Y, X>&>>>
{
    using type = stdext::cond_res<stdext::copy_cv<X, Y>&, stdext::copy_cv<Y, X>&>;
};

template <typename X, typename Y>
using common_ref = typename common_ref_impl<X, Y>::type;


template<typename T1, typename T2>
using basic_common_ref = typename basic_common_reference<
    remove_cvref_t<T1>,
    remove_cvref_t<T2>,
    stdext::add_qualifiers<T1>::template apply,
    stdext::add_qualifiers<T2>::template apply>::type;

template <typename T1, typename T2, class = void>
struct cr2c : common_type<T1, T2> {};

template<typename T1, typename T2>
struct cr2c<T1, T2, void_t<stdext::cond_res<T1, T2>>>
{
    using type = stdext::cond_res<T1, T2>;
};

///
template <typename T1, typename T2, class = void>
struct cr2b : cr2c<T1, T2> {};

template<typename T1, typename T2>
struct cr2b<T1, T2, void_t<basic_common_ref<T1, T2>>>
{
    using type = basic_common_ref<T1, T2>;
};
///
template <typename T1, typename T2, class = void>
struct cr2a : cr2b<T1, T2> {};

template <typename T1, typename T2>
struct cr2a<T1&, T2&, void_t<common_ref<T1&, T2&>>> { using type = common_ref<T1&, T2&>; };

template <typename T1, typename T2>
struct cr2a<T1&&, T2&&, void_t<common_ref<T1&&, T2&&>>> { using type = common_ref<T1&&, T2&&>; };

template <typename T1, typename T2>
struct cr2a<T1&, T2&&, void_t<common_ref<T1&, T2&&>>> { using type = common_ref<T1&, T2&&>; };

template <typename T1, typename T2>
struct cr2a<T1&&, T2&, void_t<common_ref<T1&&, T2&>>> { using type = common_ref<T1&&, T2&>; };


} // namespace common_reference_detail


template <typename...>
struct common_reference;

template <typename... T>
using common_reference_t = typename common_reference<T...>::type;

template <> struct common_reference<> {};

template <typename T> struct common_reference<T> { using type = T; };

template <typename T1, typename T2>
struct common_reference<T1, T2> : common_reference_detail::cr2a<T1, T2> {};

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
