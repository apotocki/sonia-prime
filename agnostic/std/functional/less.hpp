// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_LESS

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

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <functional>
#endif
