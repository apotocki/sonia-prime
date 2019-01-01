//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_ON_CLOSE_HPP
#define SONIA_UTILITY_ON_CLOSE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <mutex>

#include "sonia/prime_config.hpp"
#include "sonia/function.hpp"

namespace sonia { namespace services {

SONIA_PRIME_API void on_close(function<void()> const&);

}}

#endif // SONIA_UTILITY_ON_CLOSE_HPP
