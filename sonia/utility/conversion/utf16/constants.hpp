/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

namespace sonia::conversion {

const wchar_t UTF16_SUR_HIGH_START  = 0xD800;
const wchar_t UTF16_SUR_HIGH_END    = 0xDBFF;
const wchar_t UTF16_SUR_LOW_START   = 0xDC00;
const wchar_t UTF16_SUR_LOW_END     = 0xDFFF;
const wchar_t UTF16_BOM             = 0xFEFF;

const char32_t UTF16_LEAD_OFFSET = 0xD800 - (0x10000 >> 10);
const char32_t UTF16_SURROGATE_OFFSET = 0x10000 - (0xD800 << 10) - 0xDC00;

}
