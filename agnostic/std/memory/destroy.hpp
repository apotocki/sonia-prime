// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_DESTROY

#include "agnostic/std/memory/destroy_at.hpp"
#include "agnostic/std/memory/addressof.hpp"

namespace std {

template <class ForwardIt>
constexpr void destroy(ForwardIt first, ForwardIt last)
{
    for (; first != last; ++first)
        std::destroy_at(addressof(*first));
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <memory>
#endif
