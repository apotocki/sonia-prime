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
struct ascii85_tag : cvt_tag
{
    using customer_tag = ascii85_tag;
    using provider_tag = ascii85_tag;

    bool y_abbr = false;
    explicit ascii85_tag(ErrorHandlerT const& eh = ErrorHandlerT(), bool yabbr = false) 
        : errh_{eh}, y_abbr{ yabbr }
    {}

    ErrorHandlerT errh_;
};

inline const ascii85_tag<throw_error_handler> ascii85;
inline const ascii85_tag<throw_error_handler> ascii85_with_y = ascii85_tag<throw_error_handler>{ throw_error_handler{}, true };

}
