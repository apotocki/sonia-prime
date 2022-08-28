/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "ascii85_tag.hpp"
#include "../concrete_converter.hpp"

#include "int8_to_ascii85.hpp"
#include "ascii85_to_int8.hpp"

namespace sonia::conversion {

template <typename ErrorHandlerT>
struct concrete_converter<int8_tag, ascii85_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        using type = int8_to_ascii85<ProviderT, CustomerT, ErrorHandlerT>;
    };
};

template <typename ErrorHandlerT>
struct concrete_converter<ascii85_tag<ErrorHandlerT>, int8_tag>
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        using type = ascii85_to_int8<ProviderT, CustomerT, ErrorHandlerT>;
    };
};

}
