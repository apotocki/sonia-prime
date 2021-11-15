/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "urlenc_tag.hpp"
#include "../utf8/utf8_tag.hpp"
#include "../concrete_converter.hpp"

#include "utf8_to_urlenc.hpp"
#include "urlenc_to_utf8.hpp"

namespace sonia::conversion::detail {

template <typename T, typename ErrorHandlerT>
struct concrete_converter_impl<utf8_tag<T>, urlenc_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef utf8_to_urlenc<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

template <typename T, typename ErrorHandlerT>
struct concrete_converter_impl<urlenc_tag<T>, utf8_tag<ErrorHandlerT> >
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef urlenc_to_utf8<ProviderT, CustomerT, ErrorHandlerT> type;
    };
};

} // namespace sonia::conversion::detail
