// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_MIN

#include "agnostic/std/initializer_list.hpp"
#include "agnostic/std/algorithm/min_element.hpp"

namespace std {

template <class T>
constexpr const T& (min)(const T& a, const T& b)
{
    return (a < b) ? a : b;
}

template <class T, class Compare>
constexpr const T& (min)(const T& a, const T& b, Compare comp)
{
    return (comp(a, b)) ? a : b;
}

template <class T>
constexpr T (min)(std::initializer_list<T> ilist)
{
    return *min_element(ilist.begin(), ilist.end());
}

template< class T, class Compare >
constexpr T (min)(std::initializer_list<T> ilist, Compare comp)
{
    return *min_element(ilist.begin(), ilist.end(), comp);
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <algorithm>
#endif
