/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "base16_tag.hpp"
#include "../concrete_converter.hpp"

#include "int8_to_base16.hpp"
#include "base16_to_int8.hpp"

namespace sonia::conversion {

template <typename ErrorHandlerT>
struct concrete_converter<int8_tag, base16_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef int8_to_base16<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

template <typename ErrorHandlerT>
struct concrete_converter<base16_tag<ErrorHandlerT>, int8_tag>
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef base16_to_int8<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

}
