/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "utf7_tag.hpp"
#include "../utf16/utf16_tag.hpp"
#include "../concrete_converter.hpp"

#include "utf7_to_utf16.hpp"
#include "utf16_to_utf7.hpp"

namespace sonia::conversion {

template <bool OptV, typename HT, typename ErrorHandlerT>
struct concrete_converter<utf7_tag<OptV, HT>, utf16_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf7_to_utf16<ProviderT, CustomerT, ErrorHandlerT, OptV> type;
    };
};

template <typename HT, bool OptV, typename ErrorHandlerT>
struct concrete_converter<utf16_tag<HT>, utf7_tag<OptV, ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf16_to_utf7<ProviderT, CustomerT, ErrorHandlerT, OptV> type;
    };
};

}
