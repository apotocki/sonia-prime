// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_CONSTRUCT_AT_HPP
#define AGNOSTIC_STD_CONSTRUCT_AT_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_PLACEMENT_NEW
#   include "../new/placement_new.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_FORWARD
#   include "../utility/forward.hpp"
#endif

namespace std {

template<class T, class... Args>
constexpr T* construct_at(T* p, Args&&... args)
{
    return ::new (const_cast<void*>(static_cast<const volatile void*>(p)))
        T(std::forward<Args>(args)...);
}

}

#endif // AGNOSTIC_STD_CONSTRUCT_AT_HPP
