//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_LINUX_HPP
#define SONIA_UTILITY_LINUX_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <tuple>

namespace sonia { namespace linux {

std::tuple<int, int, int> kernel_version();

}}

#endif // SONIA_UTILITY_LINUX_HPP
