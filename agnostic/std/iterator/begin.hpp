// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ITERATOR_BEGIN

namespace std {

template <class C>
constexpr auto begin(C & c) -> decltype(c.begin())
{
    return c.begin();
}

template <class C>
constexpr auto begin(C const& c) -> decltype(c.begin())
{
    return c.begin();
}

template <class T, size_t N>
constexpr T* begin(T(&arr)[N]) noexcept
{
    return arr;
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <iterator>
#endif
