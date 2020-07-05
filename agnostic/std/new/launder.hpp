// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_LAUNDER

#include "agnostic/std/type_traits/is_function.hpp"
#include "agnostic/std/type_traits/is_void.hpp"

namespace std {

template <class T>
[[nodiscard]] constexpr T* launder(T* _Ptr) noexcept
{
    static_assert(!is_function_v<T> && !is_void_v<T>);
    return __builtin_launder(_Ptr);
}

}
#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <new>
#endif
