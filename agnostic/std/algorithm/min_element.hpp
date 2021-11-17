// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_MIN_ELEMENT

namespace std {

template <class ForwardIt>
constexpr ForwardIt min_element(ForwardIt first, ForwardIt last)
{
    if (first == last) return last;

    ForwardIt smallest = first;
    ++first;
    for (; first != last; ++first) {
        if (*first < *smallest) {
            smallest = first;
        }
    }
    return smallest;
}

//template< class ExecutionPolicy, class ForwardIt >
//ForwardIt min_element(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last)
//{
//
//}

template< class ForwardIt, class Compare >
constexpr ForwardIt min_element(ForwardIt first, ForwardIt last, Compare comp)
{
    if (first == last) return last;

    ForwardIt smallest = first;
    ++first;
    for (; first != last; ++first) {
        if (comp(*first, *smallest)) {
            smallest = first;
        }
    }
    return smallest;
}

//template< class ExecutionPolicy, class ForwardIt, class Compare >
//ForwardIt min_element(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, Compare comp)
//{
//
//}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <algorithm>
#endif
