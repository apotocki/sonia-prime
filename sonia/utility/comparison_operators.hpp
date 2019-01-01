//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_COMPARISON_OPERATORS_HPP
#define SONIA_UTILITY_COMPARISON_OPERATORS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <utility>

#define MAKE_COMPARISON_OPERATORS(clsnm)                                                        \
                                                                                                \
template <typename T>                                                                           \
bool operator!= (T && rhs) const { return !(*this == std::forward<T>(rhs)); }                   \
                                                                                                \
template <typename T>                                                                           \
bool operator<= (T && rhs) const { return !(*this > std::forward<T>(rhs)); }                    \
                                                                                                \
template <typename T>                                                                           \
bool operator>= (T && rhs) const { return !(*this < std::forward<T>(rhs)); }                    \
                                                                                                \
                                                                                                \
template <typename T>                                                                           \
friend bool operator== (T && lhs, clsnm const& rhs) { return rhs == std::forward<T>(lhs); }     \
                                                                                                \
template <typename T>                                                                           \
friend bool operator!= (T && lhs, clsnm const& rhs) { return !(rhs == std::forward<T>(lhs)); }  \
                                                                                                \
template <typename T>                                                                           \
friend bool operator< (T && lhs, clsnm const& rhs) { return rhs > std::forward<T>(lhs); }       \
                                                                                                \
template <typename T>                                                                           \
friend bool operator<= (T && lhs, clsnm const& rhs) { return !(rhs < std::forward<T>(lhs)); }   \
                                                                                                \
template <typename T>                                                                           \
friend bool operator> (T && lhs, clsnm const& rhs) { return rhs < std::forward<T>(lhs); }       \
                                                                                                \
template <typename T>                                                                           \
friend bool operator>= (T && lhs, clsnm const& rhs) { return !(rhs > std::forward<T>(lhs)); }   \


#define MAKE_FREE_COMPARISON_OPERATORS(cls_confirmer)                                                                         \
template <typename LT, typename RT>                                                                                           \
enable_if_t<cls_confirmer<LT> || cls_confirmer<RT>, bool> operator!= (LT const& lhs, RT const& rhs) { return !(lhs == rhs); } \
                                                                                                                              \
template <typename LT, typename RT>                                                                                           \
enable_if_t<cls_confirmer<LT> || cls_confirmer<RT>, bool> operator> (LT const& lhs, RT const& rhs) { return rhs < lhs; }      \
                                                                                                                              \
template <typename LT, typename RT>                                                                                           \
enable_if_t<cls_confirmer<LT> || cls_confirmer<RT>, bool> operator<= (LT const& lhs, RT const& rhs) { return !(rhs < lhs); }  \
                                                                                                                              \
template <typename LT, typename RT>                                                                                           \
enable_if_t<cls_confirmer<LT> || cls_confirmer<RT>, bool> operator>= (LT const& lhs, RT const& rhs) { return !(lhs < rhs); }


#endif // SONIA_UTILITY_COMPARISON_OPERATORS_HPP
