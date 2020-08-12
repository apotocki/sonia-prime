// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_MAX_ELEMENT

namespace std {

template <class ForwardIt>
constexpr ForwardIt max_element(ForwardIt first, ForwardIt last)
{
    if (first == last) return last;

    ForwardIt largest = first;
    ++first;
    for (; first != last; ++first) {
        if (*largest < *first) {
            largest = first;
        }
    }
    return largest;
}

//template< class ExecutionPolicy, class ForwardIt >
//ForwardIt max_element(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last)
//{
//
//}

template< class ForwardIt, class Compare >
constexpr ForwardIt max_element(ForwardIt first, ForwardIt last, Compare comp)
{
    if (first == last) return last;

    ForwardIt largest = first;
    ++first;
    for (; first != last; ++first) {
        if (comp(*largest, *first)) {
            largest = first;
        }
    }
    return largest;
}

//template< class ExecutionPolicy, class ForwardIt, class Compare >
//ForwardIt max_element(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, Compare comp)
//{
//
//}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <algorithm>
#endif
