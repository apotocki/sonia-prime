// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_RANGES_SWAP

#include "agnostic/std/type_traits/remove_reference.hpp"
#include "agnostic/std/type_traits/is_nothrow_move_constructible.hpp"
#include "agnostic/std/type_traits/is_nothrow_move_assignable.hpp"
#include "agnostic/std/concepts/move_constructible.hpp"
#include "agnostic/std/concepts/assignable_from.hpp"

namespace std::ranges { namespace _Swap {

template <class T>
void swap(T&, T&) = delete;

template <class _Ty>
concept is_class_or_enum_or_union =
    __is_class(remove_reference_t<_Ty>) || __is_enum(remove_reference_t<_Ty>) || __is_union(remove_reference_t<_Ty>);

template <class T1, class T2>
concept use_adl_swap = (is_class_or_enum_or_union<T1> || is_class_or_enum_or_union<T2>)
    && requires(T1 && t, T2 && u) { swap(static_cast<T1&&>(t), static_cast<T2&&>(u)); };

struct _Cpo
{
    template <class T1, class T2>
    requires use_adl_swap<T1, T2>
    constexpr void operator()(T1&& t, T2&& u) const
        noexcept(noexcept(swap(static_cast<T1&&>(t), static_cast<T2&&>(u))))
    {
        swap(static_cast<T1&&>(t), static_cast<T2&&>(u));
    }

    template <class T>
    requires (!use_adl_swap<T&, T&> && move_constructible<T> && assignable_from<T&, T>)
        constexpr void operator()(T& x, T& y) const
        noexcept(is_nothrow_move_constructible_v<T>&& is_nothrow_move_assignable_v<T>)
    {
        T tmp(static_cast<T&&>(x));
        x = static_cast<T&&>(y);
        y = static_cast<T&&>(tmp);
    }

    template <class T1, class T2, size_t size>
    constexpr void operator()(T1(&t)[size], T2(&u)[size]) const
        noexcept(noexcept((*this)(t[0], u[0])))
        requires requires(const _Cpo& f) { f(t[0], u[0]); }
    {
        for (size_t i = 0; i < size; ++i) {
            (*this)(t[i], u[i]);
        }
    }
};

} // namespace _Swap

inline namespace _Cpos {
    inline constexpr _Swap::_Cpo swap;
}

} // namespace ranges

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
