/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "../tags.hpp"

namespace sonia::conversion {

template <typename ErrorHandlerT>
struct utf8_tag : cvt_tag
{
    using customer_tag = utf8_tag;
    using provider_tag = utf8_tag;

    utf8_tag(ErrorHandlerT const& eh = ErrorHandlerT()) : errh_(eh) {}

    ErrorHandlerT errh_;
};

inline utf8_tag<throw_error_handler> utf8() {
    return utf8_tag<throw_error_handler>();
}

template <typename ErrorHandlerT>
inline utf8_tag<ErrorHandlerT> utf8(ErrorHandlerT const& err) {
    return utf8_tag<ErrorHandlerT>(err);
}

}
