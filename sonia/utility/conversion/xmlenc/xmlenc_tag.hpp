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
struct xmlenc_tag : cvt_tag
{
    typedef xmlenc_tag customer_tag;
    typedef xmlenc_tag provider_tag;

    xmlenc_tag(ErrorHandlerT const& eh = ErrorHandlerT()) : errh_(eh) {}

    ErrorHandlerT errh_;
};

inline
xmlenc_tag<throw_error_handler> xmlenc() {
    return xmlenc_tag<throw_error_handler>();
}

template <typename ErrorHandlerT>
inline xmlenc_tag<ErrorHandlerT> xmlenc(ErrorHandlerT const& err) {
    return xmlenc_tag<ErrorHandlerT>(err);
}

}
