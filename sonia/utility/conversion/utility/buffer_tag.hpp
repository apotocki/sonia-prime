/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "../tags.hpp"

namespace sonia::conversion {

template <typename T, size_t SzV>
struct buffer_tag : array_tag<T>
{
    using customer_tag = buffer_tag;
    using provider_tag = array_tag<T>;
};


template <typename T, size_t SzV>
inline buffer_tag<T, SzV> buffer() { return buffer_tag<T, SzV>(); }

template <size_t SzV>
inline buffer_tag<char, SzV> int8_buffer() { return buffer_tag<char, SzV>(); }

}
