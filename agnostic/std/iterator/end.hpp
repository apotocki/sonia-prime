// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ITERATOR_END_HPP
#define AGNOSTIC_STD_ITERATOR_END_HPP

#pragma once

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

#endif // AGNOSTIC_STD_ITERATOR_END_HPP
