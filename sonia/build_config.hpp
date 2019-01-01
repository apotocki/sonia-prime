//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_BUILD_CONFIG_HPP
#define SONIA_BUILD_CONFIG_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

#include "sonia/config.hpp"

#ifdef SONIA_ONE_UNIQUE_BUILDINFO
#   include SONIA_ONE_UNIQUE_BUILDINFO
#elif defined(SONIA_ONE_BUNDLE_BUILDINFO)
#   include SONIA_ONE_BUNDLE_BUILDINFO
#else
#   define BUILD_NAME "custom build"
#   define REVISION_NAME "undefined"
#endif

#ifndef BUILD_DATETIME
#   define BUILD_DATETIME __DATE__ " " __TIME__
#endif

#ifndef HELLO_MESSAGE
#   define HELLO_MESSAGE ""
#endif

#ifndef BUILD_ID
#   define BUILD_ID BUILD_NAME ", rev: " REVISION_NAME
#endif

#endif // SONIA_BUILD_CONFIG_HPP
