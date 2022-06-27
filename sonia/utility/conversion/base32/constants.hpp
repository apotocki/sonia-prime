/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "sonia/cstdint.hpp"

namespace sonia::conversion {

struct base32constants
{
    static const char * alphabet;
    static const char* hex_alphabet;

    static const uint8_t matrix[];
    static const uint8_t hex_matrix[];
};

}
