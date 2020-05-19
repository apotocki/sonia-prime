// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_LAUNDER_HPP
#define AGNOSTIC_STD_LAUNDER_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_IS_FUNCTION
#   include "../type_traits/is_function.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_VOID
#   include "../type_traits/is_void.hpp"
#endif

namespace std {

template <class T>
[[nodiscard]] constexpr T* launder(T* _Ptr) noexcept
{
    static_assert(!is_function_v<_Ty> && !is_void_v<_Ty>);
    return __builtin_launder(_Ptr);
}

}

#endif // AGNOSTIC_STD_LAUNDER_HPP
