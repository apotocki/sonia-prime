// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#include "sonia/config.hpp"
#include "sonia/fibers/fixedsize_stack.hpp"

namespace sonia::fibers {

SONIA_PRIME_API default_stack::default_stack()
    : boost::context::protected_fixedsize_stack{ 16 * 1024 * 1024 }
{}

}
