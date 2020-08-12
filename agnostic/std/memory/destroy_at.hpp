// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_DESTROY_AT_HPP
#define AGNOSTIC_STD_DESTROY_AT_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_IS_ARRAY
#   include "../type_traits/is_array.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ADDRESSOF
#   include "addressof.hpp"
#endif

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

#endif // AGNOSTIC_STD_DESTROY_AT_HPP
