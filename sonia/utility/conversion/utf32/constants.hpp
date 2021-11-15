/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <map>

namespace sonia::conversion {

struct utf32constants
{
    static const char32_t simple_folding[][2];
    static std::map<char32_t, char32_t> const & get_simple_folding_map();
    utf32constants();

private:
    std::map<char32_t, char32_t> map_;
};

}
