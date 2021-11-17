/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "base64_tag.hpp"
#include "../concrete_converter.hpp"

#include "int8_to_base64.hpp"
#include "base64_to_int8.hpp"

namespace sonia::conversion {

template <char CV, typename ErrorHandlerT>
struct concrete_converter<int8_tag, base64_tag<CV, ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef int8_to_base64<ProviderT, CustomerT, CV, ErrorHandlerT> type;
    };
};

template <char CV, typename ErrorHandlerT>
struct concrete_converter<base64_tag<CV, ErrorHandlerT>, int8_tag>
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef base64_to_int8<ProviderT, CustomerT, CV, ErrorHandlerT> type;
    };
};

}
