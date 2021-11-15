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
struct utf16_tag : cvt_tag
{
    typedef utf16_tag customer_tag;
    typedef utf16_tag provider_tag;

    utf16_tag(ErrorHandlerT const& eh = ErrorHandlerT()) : errh_(eh) {}

    ErrorHandlerT errh_;
};

template <typename ErrorHandlerT>
struct utf16le_tag : cvt_tag
{
    typedef utf16le_tag customer_tag;
    typedef utf16le_tag provider_tag;

    utf16le_tag(ErrorHandlerT const& eh = ErrorHandlerT()) : errh_(eh) {}

    ErrorHandlerT errh_;
};

template <typename ErrorHandlerT>
struct utf16be_tag : cvt_tag
{
    typedef utf16be_tag customer_tag;
    typedef utf16be_tag provider_tag;

    utf16be_tag(ErrorHandlerT const& eh = ErrorHandlerT()) : errh_(eh) {}

    ErrorHandlerT errh_;
};

inline
utf16_tag<throw_error_handler> utf16() {
    return utf16_tag<throw_error_handler>();
}

template <typename ErrorHandlerT>
inline utf16_tag<ErrorHandlerT> utf16(ErrorHandlerT const& err) {
    return utf16_tag<ErrorHandlerT>(err);
}

inline
utf16le_tag<throw_error_handler> utf16le() {
    return utf16le_tag<throw_error_handler>();
}

template <typename ErrorHandlerT>
inline utf16le_tag<ErrorHandlerT> utf16le(ErrorHandlerT const& err) {
    return utf16le_tag<ErrorHandlerT>(err);
}

inline
utf16be_tag<throw_error_handler> utf16be() {
    return utf16be_tag<throw_error_handler>();
}

template <typename ErrorHandlerT>
inline utf16be_tag<ErrorHandlerT> utf16be(ErrorHandlerT const& err) {
    return utf16be_tag<ErrorHandlerT>(err);
}

}
