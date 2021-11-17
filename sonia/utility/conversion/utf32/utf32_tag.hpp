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
struct utf32_tag : cvt_tag
{
    typedef utf32_tag customer_tag;
    typedef utf32_tag provider_tag;

    explicit utf32_tag(ErrorHandlerT const& eh = ErrorHandlerT()) : errh_(eh) {}

    ErrorHandlerT errh_;
};

template <typename ErrorHandlerT>
struct utf32le_tag : cvt_tag
{
    typedef utf32le_tag customer_tag;
    typedef utf32le_tag provider_tag;

    explicit utf32le_tag(ErrorHandlerT const& eh = ErrorHandlerT()) : errh_(eh) {}

    ErrorHandlerT errh_;
};

template <typename ErrorHandlerT>
struct utf32be_tag : cvt_tag
{
    typedef utf32be_tag customer_tag;
    typedef utf32be_tag provider_tag;

    explicit utf32be_tag(ErrorHandlerT const& eh = ErrorHandlerT()) : errh_(eh) {}

    ErrorHandlerT errh_;
};

template <typename ErrorHandlerT>
struct utf32sfold_tag : utf32_tag<ErrorHandlerT>
{
    typedef utf32sfold_tag customer_tag;
    typedef utf32_tag<ErrorHandlerT> provider_tag;

    explicit utf32sfold_tag(ErrorHandlerT const& eh = ErrorHandlerT()) :  utf32_tag<ErrorHandlerT>(eh) {}
};

inline utf32_tag<throw_error_handler> utf32() {
    return utf32_tag<throw_error_handler>();
}

template <typename ErrorHandlerT>
inline utf32_tag<ErrorHandlerT> utf32(ErrorHandlerT const& err) {
    return utf32_tag<ErrorHandlerT>(err);
}
    
inline utf32le_tag<throw_error_handler> utf32le() {
    return utf32le_tag<throw_error_handler>();
}

template <typename ErrorHandlerT>
inline utf32le_tag<ErrorHandlerT> utf32le(ErrorHandlerT const& err) {
    return utf32le_tag<ErrorHandlerT>(err);
}

inline utf32be_tag<throw_error_handler> utf32be() {
    return utf32be_tag<throw_error_handler>();
}

template <typename ErrorHandlerT>
inline utf32be_tag<ErrorHandlerT> utf32be(ErrorHandlerT const& err) {
    return utf32be_tag<ErrorHandlerT>(err);
}

inline utf32sfold_tag<throw_error_handler> utf32_sfold() {
    return utf32sfold_tag<throw_error_handler>();
}

}
