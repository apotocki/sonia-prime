/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

namespace sonia::conversion {

template <typename ErrorHandlerT>
struct utf32_state : private ErrorHandlerT
{
    char32_t value  : 24;
    char32_t cached : 2;

    template <typename ArgT>
    inline void clear(ArgT && arg) {
        static_cast<ErrorHandlerT&>(*this) = std::forward<ArgT>(arg);
        value = cached = 0;
    }

    inline bool operator == (utf32_state const& other) const
    { 
        return cached == other.cached && (cached == 0 || value == other.value);
    }

    template <typename VT>
    inline void error(VT&& v, const char *errmsg) {
        ErrorHandlerT::operator()(std::forward<VT>(v), *this, errmsg);
    }

    inline void reset() { value = cached = 0; }
};

}
