/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

namespace sonia::conversion {

struct utf7constants
{
    static const unsigned char utf7_matrix[];
};

static const char UTF7SHIFT_IN  = '+';
static const char UTF7SHIFT_OUT = '-';

}
