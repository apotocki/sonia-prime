/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <cassert>

#include "sonia/cstdint.hpp"
#include "../base64/constants.hpp"

namespace sonia::conversion {

template <typename ErrorHandlerT>
struct utf7_state : private ErrorHandlerT
{
    uint_least32_t cache : 20;
    uint_least32_t bit : 5;
    uint_least32_t shifting  : 1;
    uint_least32_t mode : 3;
    wchar_t wcached;

    template <typename ArgT>
    void clear(ArgT const& arg)
    {
        static_cast<ErrorHandlerT&>(*this) = arg;
        shifting = bit = mode = 0; wcached = 0;
    }

    inline bool operator == (utf7_state const& rhs) const
    { 
        return shifting == rhs.shifting && wcached == rhs.wcached && mode == rhs.mode &&
            bit == rhs.bit && (((cache ^ rhs.cache) & ((1 << bit) - 1)) == 0);;
    }

    inline void put_6b(unsigned char c)
    {
        cache <<= 6;
        cache |= c;
        bit += 6;
    }

    inline wchar_t pop_wc()
    {
        assert(bit >= 16);
        bit -= 16;
        return static_cast<wchar_t>((cache >> bit) & 0xFFFF);
    }

    inline void put_wc(wchar_t wc)
    {
        assert(bit <= 4);
        cache <<= 16;
        cache |= static_cast<boost::uint_least32_t>(wc & 0xFFFF);
        bit += 16;
    }

    inline char put_wc_pop(wchar_t wc)
    {
        assert(bit <= 4);
        cache <<= 16;
        cache |= static_cast<boost::uint_least32_t>(wc & 0xFFFF);
        bit += 10;
        return base64constants::base64_alphabet[(cache >> bit) & 0x3F];
    }

    inline char pop()
    {
        assert(bit >= 6);
        bit -= 6;
        return base64constants::base64_alphabet[(cache >> bit) & 0x3F];
    }

    inline char unshift()
    {
        assert(bit > 0 && bit <= 4);
        cache <<= (6 - bit);
        bit = 0;
        return base64constants::base64_alphabet[cache & 0x3F];
    }

    template <typename ArgT>
    inline void error(ArgT && c, const char *errmsg) {
        ErrorHandlerT::operator()(std::forward<ArgT>(c), *this, errmsg);
    }
};

}
