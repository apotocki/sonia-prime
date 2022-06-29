/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <utility>
#include "sonia/cstdint.hpp"

namespace sonia::conversion {

template <typename ErrorHandlerT>
struct ubits_state_t : private ErrorHandlerT
{
    const char* alphabet;
    const uint8_t* matrix;
    uint_least16_t bit:4;
    uint_least16_t cache:12;
    char padding_character;

    template <typename ArgT>
    inline void clear(ArgT const& arg) {
        static_cast<ErrorHandlerT&>(*this) = arg;
        bit = 0; cache = 0;
    }

    inline bool operator== (ubits_state_t const& rhs) const
    {
        bool r =  bit == rhs.bit && (((cache ^ rhs.cache) & ((1 << bit) - 1)) == 0);
        return r;
    }

    template <typename IteratorT>
    inline void error(char c, const char *errmsg, IteratorT & it) {
        ErrorHandlerT::operator()(c, *this, errmsg, it);
    }
};

}