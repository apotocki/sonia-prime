// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ITERATOR_END

namespace std {

template <class C>
constexpr auto end(C & c) -> decltype(c.end())
{
    return c.end();
}

template <class C>
constexpr auto end(C const& c) -> decltype(c.end())
{
    return c.end();
}

template <class T, size_t N>
constexpr T* end(T(&arr)[N]) noexcept
{
    return arr + N;
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <iterator>
#endif
