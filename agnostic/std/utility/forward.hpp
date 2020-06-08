// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_REFERENCE
#   include "agnostic/std/type_traits/remove_reference.hpp"
#endif

namespace std {

template <typename T>
inline T&& forward(remove_reference_t<T>& t) noexcept
{
    return static_cast<T&&>(t);
}

template <typename T>
inline T&& forward(remove_reference_t<T>&& t) noexcept
{
    return static_cast<T&&>(t);
}

}
