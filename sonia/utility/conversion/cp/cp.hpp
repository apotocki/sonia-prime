/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "cp_tag.hpp"
#include "../utf16/utf16_tag.hpp"
#include "../concrete_converter.hpp"

#include "cp_to_utf16.hpp"
#include "utf16_to_cp.hpp"

namespace sonia::conversion {

template <typename HT, typename ErrorHandlerT>
struct concrete_converter<cp_tag<HT>, utf16_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef cp_to_utf16<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

template <typename HT, typename ErrorHandlerT>
struct concrete_converter<utf16_tag<HT>, cp_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf16_to_cp<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

}
