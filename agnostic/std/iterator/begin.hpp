// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ITERATOR_BEGIN_HPP
#define AGNOSTIC_STD_ITERATOR_BEGIN_HPP

#pragma once

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

#endif // AGNOSTIC_STD_ITERATOR_BEGIN_HPP
