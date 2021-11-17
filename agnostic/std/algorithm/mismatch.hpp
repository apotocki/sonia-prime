// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_MISMATCH

#include "agnostic/std/utility/pair.hpp"

namespace std {

template <typename InputIt1, typename InputIt2>
constexpr pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
    while (first1 != last1 && *first1 == *first2) {
        ++first1, ++first2;
    }
    return pair{first1, first2};
}

template <typename InputIt1, typename InputIt2, typename BinaryPredicate>
constexpr pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p)
{
    while (first1 != last1 && p(*first1, *first2)) {
        ++first1, ++first2;
    }
    return pair{first1, first2};
}

template <typename InputIt1, typename InputIt2>
constexpr pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
    while (first1 != last1 && first2 != last2 && *first1 == *first2) {
        ++first1, ++first2;
    }
    return pair{first1, first2};
}

template <typename InputIt1, typename InputIt2, typename BinaryPredicate>
constexpr pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, BinaryPredicate p)
{
    while (first1 != last1 && first2 != last2 && p(*first1, *first2)) {
        ++first1, ++first2;
    }
    return pair{first1, first2};
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <algorithm>
#endif
