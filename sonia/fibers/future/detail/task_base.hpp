//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/intrusive_ptr.hpp>

#include "sonia/fibers/detail/config.hpp"
#include "sonia/fibers/future/detail/shared_state.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace sonia::fibers::detail {

template< typename R, typename ... Args >
struct task_base : public shared_state< R >
{
    using ptr_type = boost::intrusive_ptr< task_base >;

    virtual ~task_base() = default;
    
    virtual void run( Args && ... args) = 0;

    virtual ptr_type reset() = 0;
};

}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif
