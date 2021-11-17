/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "utf16_tag.hpp"
#include "../utf32/utf32_tag.hpp"
#include "../concrete_converter.hpp"

#include "utf16_to_utf32.hpp"
#include "utf16_to_utf16le.hpp"
#include "utf16_to_utf16be.hpp"

#include "utf32_to_utf16.hpp"
#include "utf16le_to_utf16.hpp"
#include "utf16be_to_utf16.hpp"

namespace sonia::conversion {

template <typename T, typename ErrorHandlerT>
struct concrete_converter<utf16_tag<T>, utf32_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf16_to_utf32<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

template <typename T, typename ErrorHandlerT>
struct concrete_converter<utf16_tag<T>, utf16le_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf16_to_utf16le<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

template <typename T, typename ErrorHandlerT>
struct concrete_converter<utf16_tag<T>, utf16be_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf16_to_utf16be<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

template <typename T, typename ErrorHandlerT>
struct concrete_converter<utf32_tag<T>, utf16_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf32_to_utf16<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

template <typename T, typename ErrorHandlerT>
struct concrete_converter<utf16le_tag<T>, utf16_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf16le_to_utf16<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

template <typename T, typename ErrorHandlerT>
struct concrete_converter<utf16be_tag<T>, utf16_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf16be_to_utf16<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

}
