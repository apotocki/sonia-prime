/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "rc5_tag.hpp"
#include "../tags.hpp"
#include "../concrete_converter.hpp"

#include "int8_to_rc5.hpp"
#include "rc5_to_int8.hpp"

namespace sonia::conversion {

template <unsigned int w, unsigned int r, unsigned int b, typename ErrorHandlerT>
struct concrete_converter< int8_tag, rc5_tag<w, r, b, ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef int8_to_rc5<ProviderT, CustomerT, w, r, b, ErrorHandlerT> type;
    };
};

template <unsigned int w, unsigned int r, unsigned int b, typename ErrorHandlerT>
struct concrete_converter<rc5_tag<w, r, b, ErrorHandlerT>, int8_tag>
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef rc5_to_int8<ProviderT, CustomerT, w, r, b, ErrorHandlerT> type;
    };
};

}
