
//          Copyright Oliver Kowalke 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

#include <boost/context/detail/disable_overload.hpp>

#include "sonia/fibers/detail/config.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
# include BOOST_ABI_PREFIX
#endif

namespace sonia::fibers::detail {

template< typename X, typename Y >
using disable_overload = boost::context::detail::disable_overload< X, Y >;

}

#ifdef BOOST_HAS_ABI_HEADERS
#include BOOST_ABI_SUFFIX
#endif
