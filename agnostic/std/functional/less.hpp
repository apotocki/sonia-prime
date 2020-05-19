// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_LESS_HPP
#define AGNOSTIC_STD_LESS_HPP

#pragma once

namespace std {

template <typename T = void>
struct less
{
    constexpr bool operator()(T const& lhs, T const& rhs) const
    {
        return lhs < rhs;
    }
};

template <>
struct less<void>
{
    template <typename LT, typename RT>
    constexpr bool operator()(LT const& lhs, RT const& rhs) const
    {
        return lhs < rhs;
    }
};

}

#endif // AGNOSTIC_STD_LESS_HPP
