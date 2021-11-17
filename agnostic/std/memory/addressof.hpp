// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ADDRESSOF

namespace std {

#ifdef _MSC_VER

template< class T >
constexpr T* addressof(T& arg) noexcept
{
    return __builtin_addressof(arg);
}

#else

template< class T >
T* addressof(T& arg)
{
    return reinterpret_cast<T*>(
        &const_cast<char&>(
            reinterpret_cast<const volatile char&>(arg)));
}

#endif

template <class T> const T* addressof(const T&&) = delete;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <memory>
#endif
