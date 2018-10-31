//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_HAS_HPP
#define SONIA_FUNCTIONAL_HAS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <type_traits>

#define HAS_MEMBER_TRAIT_DEF(X)                                                     \
template<class T> class has_member_impl_##X {                                       \
    struct fallback_type { int X; };                                                \
    struct derived_type : T, fallback_type { };                                     \
    template<typename U, U> struct check;                                           \
    template<typename U> static char & func(check<int fallback_type::*, &U::X> *);  \
    template<typename U> static char (&func(...))[2];                               \
  public:                                                                           \
    typedef has_member_impl_##X type;                                               \
    enum { value = sizeof(func<derived_type>(0)) != 1 };                            \
};                                                                                  \
template<class T> class has_member_##X                                              \
    : public std::bool_constant<has_member_impl_##X<T>::value> {};                  \
template <class T> constexpr bool has_member_##X##_v = has_member_##X<T>::value;

#endif // SONIA_FUNCTIONAL_HAS_HPP
