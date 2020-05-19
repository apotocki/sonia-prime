// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_DESTROY_HPP
#define AGNOSTIC_STD_DESTROY_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_DESTROY_AT
#   include "destroy_at.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ADDRESSOF
#   include "addressof.hpp"
#endif

namespace std {

template <class ForwardIt>
constexpr void destroy_at(ForwardIt first, ForwardIt last)
{
    for (; first != last; ++first)
        std::destroy_at(addressof(*first));
}

}

#endif // AGNOSTIC_STD_DESTROY_HPP
