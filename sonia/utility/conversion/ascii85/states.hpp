/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "sonia/cstdint.hpp"

namespace sonia::conversion {

struct ascii85_put_state
{
    uint32_t tuple;
    int32_t count : 16;
    int32_t y_abbr : 1;

    inline void clear()
    {
        tuple = 0;
        count = 0;
    }
};

struct ascii85_enc_get_state
{
    char data[5];
    int8_t count;
    int8_t cidx;
    int8_t y_abbr;

    inline void clear()
    {
        count = 0;
    }
};

struct ascii85_dec_get_state
{
    uint32_t tuple;
    int8_t count;
    int8_t cidx;
    int8_t y_abbr;

    inline void clear()
    {
        count = 0;
    }
};

}
