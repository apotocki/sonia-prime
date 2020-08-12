// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_MAX

#include "agnostic/std/initializer_list.hpp"
#include "agnostic/std/algorithm/max_element.hpp"

namespace std {

template <class T>
constexpr const T& (max)(const T& a, const T& b)
{
    return (a < b) ? b : a;
}

template <class T, class Compare>
constexpr const T& (max)(const T& a, const T& b, Compare comp)
{
    return (comp(a, b)) ? b : a;
}

template <class T>
constexpr T (max)(std::initializer_list<T> ilist)
{
    return *max_element(ilist.begin(), ilist.end());
}

template< class T, class Compare >
constexpr T (max)(std::initializer_list<T> ilist, Compare comp)
{
    return *max_element(ilist.begin(), ilist.end(), comp);
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <algorithm>
#endif
