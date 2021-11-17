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
struct base16_tag : cvt_tag
{
    typedef base16_tag customer_tag;
    typedef base16_tag provider_tag;

    base16_tag(ErrorHandlerT const& eh = ErrorHandlerT()) : errh_(eh) {}

    ErrorHandlerT errh_;
};

inline base16_tag<throw_error_handler> base16() {
    return base16_tag<throw_error_handler>();
}

template <typename ErrorHandlerT>
inline base16_tag<ErrorHandlerT> base16(ErrorHandlerT const& err) {
    return base16_tag<ErrorHandlerT>(err);
}

}
