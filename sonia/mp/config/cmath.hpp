/*=============================================================================
    Copyright (c) Alexander Pototskiy 2025

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once
#include <cmath>

#if defined(__GNUC__) && __GNUC__ <= 13

namespace std {
    // GCC 13 and earlier do not support long double logl
    inline long double logl(long double x) { return log(x); }
    inline long double powl(long double x, long double y) { return pow(x, y); }
}

#endif
