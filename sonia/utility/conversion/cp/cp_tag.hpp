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
struct cp_tag : cvt_tag
{
    using customer_tag = cp_tag;
    using provider_tag = cp_tag;

    cp_tag(const char* lnm = std::locale().name().c_str(), ErrorHandlerT const& eh = ErrorHandlerT()) : localename_(lnm), errh_(eh) {}
    const char * localename_;
    ErrorHandlerT errh_;
};

inline cp_tag<throw_error_handler> cp(const char * locale_name) {
    return cp_tag<throw_error_handler>(locale_name);
}

template <typename ErrorHandlerT>
inline cp_tag<ErrorHandlerT> cp(const char * locale_name, ErrorHandlerT const& err) {
    return cp_tag<ErrorHandlerT>(locale_name, err);
}

}
