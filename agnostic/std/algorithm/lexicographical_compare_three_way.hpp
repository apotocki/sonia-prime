#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_LEXICOGRAPHICAL_COMPARE_THREE_WAY

//#include "agnostic/std/type_traits/integral_constant.hpp"
//#include "agnostic/std/type_traits/void.hpp"
//#include "agnostic/std/utility/declval.hpp"
//#include "agnostic/std/compare/ordering.hpp"
//#include "agnostic/std/compare/equality.hpp"
//
//#include "agnostic/utility/dependent_bool.hpp"

// https://github.com/llvm/llvm-project/commit/2a06757a200cc8dd4c3aeca98509d50d75bb4a27
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "agnostic/std/algorithm/min.hpp"
#include "agnostic/std/compare/ordering.hpp"
#include "agnostic/std/algorithm/compare_3way.hpp"
#include "agnostic/std/utility/move.hpp"

#include "agnostic/std/iterator/iterator_traits.hpp"
#include "agnostic/std/type_traits/common_type.hpp"
#include "agnostic/std/type_traits/is_copy_constructible.hpp"
#include "agnostic/std/type_traits/is_same.hpp"

namespace std {

template <class _Comp>
using __three_way_comp_ref_type = _Comp&;

/*
template <class _Iter>
using __iter_diff_t = typename iterator_traits<_Iter>::difference_type;

template<class _Tp, class... _Args>
inline constexpr bool __one_of_v = (is_same_v<_Tp, _Args> || ...);

template <class _Tp>
concept __comparison_category = __one_of_v<_Tp, partial_ordering, weak_ordering, strong_ordering>;
*/

// Fast path for random access iterators which computes the number of loop iterations up-front and
// then skips the iterator comparisons inside the loop.
template <class _InputIterator1, class _InputIterator2, class _Cmp>
[[nodiscard]] constexpr auto __lexicographical_compare_three_way_fast_path(
    _InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _InputIterator2 __last2, _Cmp& __comp)
    -> decltype(__comp(*__first1, *__first2)) {
    static_assert(
        signed_integral<__iter_diff_t<_InputIterator1>>, "Using a non-integral difference_type is undefined behavior.");
    static_assert(
        signed_integral<__iter_diff_t<_InputIterator2>>, "Using a non-integral difference_type is undefined behavior.");

    using _Len1 = __iter_diff_t<_InputIterator1>;
    using _Len2 = __iter_diff_t<_InputIterator2>;
    using _Common = common_type_t<_Len1, _Len2>;

    _Len1 __len1 = __last1 - __first1;
    _Len2 __len2 = __last2 - __first2;
    _Common __min_len = std::min<_Common>(__len1, __len2);

    for (_Common __i = 0; __i < __min_len; ++__i) {
        auto __c = __comp(*__first1, *__first2);
        if (__c != 0) {
            return __c;
        }
        ++__first1;
        ++__first2;
    }

    return __len1 <=> __len2;
}

// Unoptimized implementation which compares the iterators against the end in every loop iteration
template <class _InputIterator1, class _InputIterator2, class _Cmp>
[[nodiscard]] constexpr auto __lexicographical_compare_three_way_slow_path(
    _InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _InputIterator2 __last2, _Cmp& __comp)
    -> decltype(__comp(*__first1, *__first2)) {
    while (true) {
        bool __exhausted1 = __first1 == __last1;
        bool __exhausted2 = __first2 == __last2;

        if (__exhausted1 || __exhausted2) {
            if (!__exhausted1)
                return strong_ordering::greater;
            if (!__exhausted2)
                return strong_ordering::less;
            return strong_ordering::equal;
        }

        auto __c = __comp(*__first1, *__first2);
        if (__c != 0) {
            return __c;
        }

        ++__first1;
        ++__first2;
    }
}

template <class _InputIterator1, class _InputIterator2, class _Cmp>
[[nodiscard]] inline constexpr auto lexicographical_compare_three_way(
    _InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _InputIterator2 __last2, _Cmp __comp)
    -> decltype(__comp(*__first1, *__first2)) {
    static_assert(__comparison_category<decltype(__comp(*__first1, *__first2))>,
        "The comparator passed to lexicographical_compare_three_way must return a comparison category type.");
    static_assert(std::is_copy_constructible_v<_InputIterator1>, "Iterators must be copy constructible.");
    static_assert(std::is_copy_constructible_v<_InputIterator2>, "Iterators must be copy constructible.");
    __three_way_comp_ref_type<_Cmp> __wrapped_comp_ref(__comp);
    if constexpr (__is_cpp17_random_access_iterator<_InputIterator1>::value &&
        __is_cpp17_random_access_iterator<_InputIterator2>::value) {
        return std::__lexicographical_compare_three_way_fast_path(
            std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2), __wrapped_comp_ref);
    }
    else {
        // Unoptimized implementation which compares the iterators against the end in every loop iteration
        return std::__lexicographical_compare_three_way_slow_path(
            std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2), __wrapped_comp_ref);
    }
}

template <class _InputIterator1, class _InputIterator2>
[[nodiscard]] inline constexpr auto lexicographical_compare_three_way(
    _InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _InputIterator2 __last2) {
    return std::lexicographical_compare_three_way(
        std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2), std::compare_three_way());
}

} // _LIBCPP_END_NAMESPACE_STD

//_LIBCPP_POP_MACROS

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <algorithm>
#endif
