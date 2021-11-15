/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "../tags.hpp"

namespace sonia::conversion {

template <bool opt, typename ErrorHandlerT>
struct utf7_tag : cvt_tag
{
    typedef utf7_tag customer_tag;
    typedef utf7_tag provider_tag;

    explicit utf7_tag(ErrorHandlerT const& eh = ErrorHandlerT()) : errh_(eh) {}
    ErrorHandlerT errh_;
};

inline utf7_tag<false, throw_error_handler> utf7() {
    return utf7_tag<false, throw_error_handler>();
}

template <bool opt>
inline utf7_tag<opt, throw_error_handler> utf7() {
    return utf7_tag<opt, throw_error_handler>();
}

template <typename ErrorHandlerT>
inline utf7_tag<false, ErrorHandlerT> utf7(ErrorHandlerT const& err) {
    return utf7_tag<false, ErrorHandlerT>(err);
}

template <bool opt, typename ErrorHandlerT>
inline utf7_tag<opt, ErrorHandlerT> utf7(ErrorHandlerT const& err) {
    return utf7_tag<opt, ErrorHandlerT>(err);
}

}
