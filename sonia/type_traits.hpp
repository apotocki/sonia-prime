//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <iosfwd>
#include <utility>
#include <type_traits>
#include <typeindex>
#include <functional> // is_placeholder, reference_wrapper
#include <boost/is_placeholder.hpp>
#include <boost/mpl/identity.hpp>

#include <boost/call_traits.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/utility/typed_in_place_factory.hpp>

#include "utility/expected.hpp"
#include "utility/variadic.hpp"

#define typeidx(t) std::type_index(typeid(t))

#include "agnostic/std/type_traits/remove_cvref.hpp"
#include "agnostic/utility/ceiling.hpp"
#include "agnostic/utility/dependent_bool.hpp"

namespace sonia {

struct null_t
{
    null_t() = default;

    inline bool operator == (null_t const&) const noexcept { return true; }
    inline bool operator != (null_t const&) const noexcept { return false; }
    inline bool operator < (null_t const&) const noexcept { return false; }
    inline bool operator <= (null_t const&) const noexcept { return true; }
    inline bool operator > (null_t const&) const noexcept { return false; }
    inline bool operator >= (null_t const&) const noexcept { return true; }
};

template <typename CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, null_t const&)
{
    return os << "null";
}

inline size_t hash_value(null_t const&) { return 0; }

constexpr null_t null{};

struct empty_t {};
struct use_default_t {};

using namespace tl;
//template <class T, class E> using expected = tl::expected<T, E>;
//using make_unexpected = tl::make_unexpected;

using agnostic::dependent_true;
using agnostic::dependent_false;

using std::bool_constant;
using std::integral_constant;

using std::true_type;
using std::false_type;

using std::void_t;

using std::enable_if;
using std::enable_if_t;

using std::is_integral;
using std::is_integral_v;

using std::is_signed;
using std::is_signed_v;

using std::is_unsigned;
using std::is_unsigned_v;

using std::make_unsigned;
using std::make_unsigned_t;

using std::make_signed;
using std::make_signed_t;

using std::is_floating_point;
using std::is_floating_point_v;

using std::is_enum;
using std::is_enum_v;

using std::underlying_type;
using std::underlying_type_t;

using std::is_same;
using std::is_same_v;

using std::is_base_of;
using std::is_base_of_v;

using std::is_pointer;
using std::is_pointer_v;

using std::is_reference;
using std::is_reference_v;

using std::is_lvalue_reference;
using std::is_lvalue_reference_v;

using std::is_rvalue_reference;
using std::is_rvalue_reference_v;

//using std::is_pod;
//using std::is_pod_v;

using std::is_trivial;
using std::is_trivial_v;

using std::is_void;
using std::is_void_v;

using std::is_constructible;
using std::is_constructible_v;

using std::is_copy_constructible;
using std::is_copy_constructible_v;

using std::is_trivially_constructible;
using std::is_trivially_constructible_v;

using std::is_move_constructible;
using std::is_move_constructible_v;

using std::is_trivially_destructible;
using std::is_trivially_destructible_v;

using std::has_virtual_destructor;
using std::has_virtual_destructor_v;

using std::is_assignable;
using std::is_assignable_v;

using std::conditional;
using std::conditional_t;

using std::alignment_of;
using std::alignment_of_v;

using std::remove_cv;
using std::remove_cv_t;
using std::remove_reference;
using std::remove_reference_t;
using std::remove_pointer;
using std::remove_pointer_t;

using std::is_const;
using std::is_const_v;
using std::add_const;
using std::add_const_t;
using std::as_const;

using std::is_convertible;
using std::is_convertible_v;

using std::is_object;
using std::is_object_v;

using std::is_function;
using std::is_function_v;

template <bool V, typename T>
using add_const_if_t = conditional_t<V, add_const_t<T>, T>;

template <typename T, typename RefT>
using same_const_t = add_const_if_t<is_const_v<RefT>, T>;

using std::add_pointer;
using std::add_pointer_t;
using std::add_lvalue_reference;
using std::add_lvalue_reference_t;
using std::add_rvalue_reference;
using std::add_rvalue_reference_t;

using std::reference_wrapper;
using std::ref;
using std::cref;

// placeholders
template <int I> struct arg_c
{
    static constexpr int value = I;
    
    //template <typename ... ArgsT> struct apply : variadic::type_at<I - 1, ArgsT...> { };
    template <typename ... ArgsT> using apply = variadic::type_at<I - 1, ArgsT...>;
    template <typename ... ArgsT> using apply_t = typename apply<ArgsT...>::type;
};

template <class VT> struct arg
{
    using type = arg;//arg_c<VT::value>;
    static constexpr int value = VT::value;
};

template <class T> struct is_arg : integral_constant<int, 0> {};
template <class T> struct is_arg<arg<T>> : integral_constant<int, T::value> {};
template <int I> struct is_arg<arg_c<I>> : integral_constant<int, I> {};
template <class T> constexpr int is_arg_v = is_arg<T>::value;

template <class T> struct is_placeholder : integral_constant<int, std::is_placeholder_v<T> + boost::is_placeholder<T>::value + is_arg_v<T>> {};
template <class T> constexpr int is_placeholder_v = is_placeholder<T>::value;

template <class T> struct is_reference_wrapper : false_type{};
template <class T> struct is_reference_wrapper<reference_wrapper<T>> : true_type{};
template <class T> constexpr int is_reference_wrapper_v = is_reference_wrapper<T>::value;

template <class T, class Enabler = void> struct size_of : integral_constant<int, sizeof(T)> {};
template <class T> struct size_of<T, enable_if_t<is_void_v<T>>> : integral_constant<int, 0> {};
template <class T> constexpr size_t size_of_v = size_of<T>::value;

using std::in_place;
using std::in_place_t;
using std::in_place_type;
using std::in_place_type_t;

using boost::mpl::identity;
using boost::mpl::make_identity;

template <typename T> using identity_t = typename identity<T>::type;

using std::remove_cvref;
using std::remove_cvref_t;

template <bool Test, class T = void> using disable_if = enable_if<!Test, T>;
template <bool Test, class T = void> using disable_if_t = enable_if_t<!Test, T>;

template <class T, class TestT, class RT = void> using enable_if_same_ref = enable_if<is_same_v<T, remove_cvref_t<TestT>>, RT>;
template <class T, class TestT, class RT = void> using enable_if_same_ref_t = typename enable_if_same_ref<T, TestT, RT>::type;

template <class T, class TestT, class RT = void> using disable_if_same_ref = disable_if<is_same_v<T, remove_cvref_t<TestT>>, RT>;
template <class T, class TestT, class RT = void> using disable_if_same_ref_t = typename disable_if_same_ref<T, TestT, RT>::type;

template <typename WhatT, typename ... ListT>
struct is_in : bool_constant<false || (is_same_v<WhatT, ListT> || ...)> {};

template <typename WhatT, typename ... ListT>
using is_in_t = typename is_in<WhatT, ListT ...>::type;

template <typename WhatT, typename ... ListT>
constexpr bool is_in_v = is_in<WhatT, ListT ...>::value;

template <template <class ...> class TemplateT, class T>
struct is_template_instance : false_type {};

template <template <class ...> class TemplateT, class ... Ts>
struct is_template_instance<TemplateT, TemplateT<Ts...>> : true_type {};

template <template <auto ...> class TemplateT, class T>
constexpr bool is_template_instance_v = is_template_instance<TemplateT, T>::value;

template <typename T>
using arrow_deref_type_t = remove_pointer_t<decltype(std::declval<T>().operator->())>;

// call traits
template <typename T> using call_param_t = typename boost::call_traits<T>::param_type;

enum class endian
{
#ifdef _WIN32
    little = 0,
    big    = 1,
    native = little
#else
    little = __ORDER_LITTLE_ENDIAN__,
    big    = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
};

using agnostic::ceiling_v;

template <typename T> T * get_pointer(T * ptr) { return ptr; }

// in_place_factory
template <class T> struct is_in_place_factory : is_base_of<boost::in_place_factory_base, T> {};
template <class T> constexpr bool is_in_place_factory_v = is_in_place_factory<T>::value;

template <class T> struct is_typed_in_place_factory : is_base_of<boost::typed_in_place_factory_base, T> {};
template <class T> constexpr bool is_typed_in_place_factory_v = is_typed_in_place_factory<T>::value;

template <class T> constexpr bool is_factory_v = is_in_place_factory_v<T> || is_typed_in_place_factory_v<T>;

// meta programming
template <class TargetT, typename TagT>
using apply_t = typename TargetT::template apply<TagT>;


template <typename ... ArgsT> struct switchable;
template <typename ... ArgsT> using switchable_t = typename switchable<ArgsT...>::type;

template <typename DT, typename ... ArgsT> struct switchable1 { using type = DT; };

template <typename CT, typename RT, typename ... ArgsT>
struct switchable2 : conditional<CT::value, RT, switchable_t<ArgsT ...>> {};

template <typename ... ArgsT> struct switchable2_lazy : switchable2<ArgsT...> {};

template <typename ... ArgsT>
struct switchable : conditional_t<2 < sizeof...(ArgsT), switchable2_lazy<ArgsT...>, switchable1<ArgsT...>> {};

}
