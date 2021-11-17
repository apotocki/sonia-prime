/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <utility>

namespace sonia::conversion {

template <typename ErrorHandlerT>
struct utf8st : private ErrorHandlerT
{
    char32_t num:2;
    char32_t data:30;

    template <typename ArgT>
    void clear(ArgT const& arg)
    {
        static_cast<ErrorHandlerT&>(*this) = arg;
        data = num = 0;
    }

    inline void reset() { data = num = 0; }

    inline bool operator==(utf8st const& rhs) const
    {
        return num == rhs.num && data == rhs.data;
    }

    template <typename ArgT>
    inline void error(ArgT && c, const char *errmsg) {
        ErrorHandlerT::operator()(std::forward<ArgT>(c), *this, errmsg);
    }
};

}
