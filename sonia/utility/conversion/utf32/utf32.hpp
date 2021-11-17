/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "utf32_tag.hpp"
#include "../concrete_converter.hpp"

#include "utf32_to_utf32le.hpp"
#include "utf32_to_utf32be.hpp"

#include "utf32le_to_utf32.hpp"
#include "utf32be_to_utf32.hpp"

#include "utf32_simple_folding.hpp"

namespace sonia::conversion {

template <typename T, typename ErrorHandlerT>
struct concrete_converter<utf32_tag<T>, utf32le_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf32_to_utf32le<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

template <typename T, typename ErrorHandlerT>
struct concrete_converter<utf32_tag<T>, utf32be_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf32_to_utf32be<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};


template <typename T, typename ErrorHandlerT>
struct concrete_converter<utf32le_tag<T>, utf32_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf32le_to_utf32<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

template <typename T, typename ErrorHandlerT>
struct concrete_converter<utf32be_tag<T>, utf32_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf32be_to_utf32<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

template <typename T, typename ErrorHandlerT>
struct concrete_converter<utf32_tag<T>, utf32sfold_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf32_simple_folding<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

}
