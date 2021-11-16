// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_DESTROY_AT

#include "agnostic/std/type_traits/is_array.hpp"
#include "agnostic/std/memory/addressof.hpp"

namespace std {

template <class T>
constexpr void destroy_at(T* p)
{
    if constexpr (std::is_array_v<T>) {
        for (auto& elem : *p) {
            destroy_at(addressof(elem));
        }
    } else {
        p->~T();
    }
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <memory>
#endif
