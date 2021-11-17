// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_CONSTRUCT_AT

#include "agnostic/std/new/placement_new.hpp"
#include "agnostic/std/utility/forward.hpp"

namespace std {

template<class T, class... Args>
constexpr T* construct_at(T* p, Args&&... args)
{
    return ::new (const_cast<void*>(static_cast<const volatile void*>(p)))
        T(std::forward<Args>(args)...);
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <memory>
#endif
