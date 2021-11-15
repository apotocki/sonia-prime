/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "../tags.hpp"

namespace sonia::conversion {

template <char PaddingCV, typename ErrorHandlerT>
struct base64_tag : cvt_tag
{
    using customer_tag = base64_tag;
    using provider_tag = base64_tag;

    base64_tag(ErrorHandlerT const& eh = ErrorHandlerT()) : errh_(eh) {}

    ErrorHandlerT errh_;
};

const char default_base64_padding_char = '=';

inline base64_tag<default_base64_padding_char, throw_error_handler> base64() {
    return base64_tag<default_base64_padding_char, throw_error_handler>();
}

template <char PaddingCV>
inline base64_tag<PaddingCV, throw_error_handler> base64() {
    return base64_tag<PaddingCV, throw_error_handler>();
}

template <typename ErrorHandlerT>
inline base64_tag<default_base64_padding_char, ErrorHandlerT> base64(ErrorHandlerT const& err) {
    return base64_tag<default_base64_padding_char, ErrorHandlerT>(err);
}

template <char PaddingCV, typename ErrorHandlerT>
inline base64_tag<PaddingCV, ErrorHandlerT> base64(ErrorHandlerT const& err) {
    return base64_tag<PaddingCV, ErrorHandlerT>(err);
}

}
