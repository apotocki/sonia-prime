/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "buffer_tag.hpp"
#include "../concrete_converter.hpp"

#include "type_to_buffer.hpp"
#include "unfold.hpp"

namespace sonia::conversion {

template <typename T, size_t SzV>
struct concrete_converter<type_tag<T>, buffer_tag<T, SzV>>
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef type_to_buffer<ProviderT, CustomerT, T, SzV> type;
    };
};

template <typename T>
struct concrete_converter<array_tag<T>, type_tag<T>>
{
    template <typename ProviderT, typename CustomerT>
    struct apply {
        typedef array_unfold<ProviderT, CustomerT, T> type;
    };
};

}
