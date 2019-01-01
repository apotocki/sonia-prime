//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_HAS_HPP
#define SONIA_FUNCTIONAL_HAS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <type_traits>

/**
based on https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature
*/

#define HAS_MEMBER_TRAIT_DEF(X)                                                     \
template<class T> class has_member_impl_##X {                                       \
    struct fallback_type { int X; };                                                \
    struct derived_type : T, fallback_type { };                                     \
    template<typename U, U> struct check;                                           \
    template<typename U> static char func(check<int fallback_type::*, &U::X> *);    \
    template<typename U> static char (&func(...))[2];                               \
public:                                                                             \
    using type = has_member_impl_##X;                                               \
    static constexpr bool value = sizeof(func<derived_type>(0)) != 1;               \
};                                                                                  \
template<class T> class has_member_##X                                              \
    : public std::bool_constant<has_member_impl_##X<T>::value> {};                  \
template <class T> constexpr bool has_member_##X##_v = has_member_##X<T>::value;


#define HAS_METHOD_TRAIT_DEF(X)                                                     \
template <typename T, typename FT> class has_method_##X {                           \
static_assert(																		\
	std::integral_constant<FT, false>::value,                                       \
	"Second template parameter needs to be of function type.");						\
};																					\
template <class T, typename RT, typename... ArgsT>									\
class has_method_##X<T, RT(ArgsT...)> {										        \
    template <typename ST> static constexpr auto check(ST*) ->						\
	    std::is_same<RT, decltype(std::declval<ST>().X(std::declval<ArgsT>()...))>; \
    template <typename> static std::false_type check(...);							\
public:                                                                             \
	using type  = decltype(check<T>(nullptr));										\
    static constexpr bool value = type::value;						                \
};                                                                                  \
template <class T, typename FT> constexpr bool has_method_##X##_v =					\
	has_method_##X<T, FT>::value;

#endif // SONIA_FUNCTIONAL_HAS_HPP
