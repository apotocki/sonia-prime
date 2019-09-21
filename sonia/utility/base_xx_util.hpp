//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_BASE_XX_UTIL_HPP
#define SONIA_UTILITY_BASE_XX_UTIL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"

namespace sonia {

template <typename OutputIteratorT>
bool base16_decode(string_view s, OutputIteratorT it)
{
    bool lastpart = false;
    uint8_t result = 0;
    for (char c : s) {
        result *= 16;
        if (c >= '0' && c <= '9') {
            result |= (c - '0');
        } else if (c >= 'A' && c <= 'F') {
            result |= (c - 'A') + 10;
        } else if (c >= 'a' && c <= 'f') {
            result |= (c - 'a') + 10;
        } else {
            return false;
        }
        if (lastpart) {
            *it = result; ++it;
            result = 0;
        }
        lastpart = !lastpart;
    }
    return true;
}

template <typename InputIteratorT, typename OutputIteratorT>
void base16_encode(InputIteratorT iit, InputIteratorT eit, OutputIteratorT oit)
{
    auto encoder = [](uint8_t v) -> char {
        return v + (v < 10 ? '0' : 'a' - 10);
    };
    for (; iit != eit; ++iit) {
        uint8_t c = *iit;
        *oit++ = encoder(c >> 4);
        *oit++ = encoder(c & 15);
    }
}

}

#endif // SONIA_UTILITY_BASE_XX_UTIL_HPP
