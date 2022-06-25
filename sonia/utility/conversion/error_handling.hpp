/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <boost/throw_exception.hpp>
#include "sonia/exceptions.hpp"

namespace sonia::conversion {

struct throw_error_handler
{
    template <typename DataT, typename StateT>
    void operator()(DataT&, StateT&, const char *errmsg) const
    {
        BOOST_THROW_EXCEPTION(exception(errmsg));
    }

    template <typename DataT, typename StateT, typename IteratorT>
    void operator()(DataT&, StateT&, const char *errmsg, IteratorT&) const
    {
        BOOST_THROW_EXCEPTION(exception(errmsg));
    }
};

}
