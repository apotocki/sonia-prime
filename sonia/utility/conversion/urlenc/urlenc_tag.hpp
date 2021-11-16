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
struct urlenc_tag : cvt_tag
{
    using customer_tag = urlenc_tag;
    using provider_tag = urlenc_tag;

    urlenc_tag(ErrorHandlerT const& eh = ErrorHandlerT()) : errh_(eh) {}

    ErrorHandlerT errh_;
};

inline
urlenc_tag<throw_error_handler> urlenc() {
    return urlenc_tag<throw_error_handler>();
}

template <typename ErrorHandlerT>
inline urlenc_tag<ErrorHandlerT> urlenc(ErrorHandlerT const& err) {
    return urlenc_tag<ErrorHandlerT>(err);
}

}
