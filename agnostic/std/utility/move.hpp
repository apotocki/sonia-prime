// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_MOVE

#include "agnostic/std/type_traits/remove_reference.hpp"

namespace std {

template <class T>
inline T&& move(T& t) noexcept
{
    return static_cast<remove_reference_t<T>&&>(t);
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <utility>
#endif
