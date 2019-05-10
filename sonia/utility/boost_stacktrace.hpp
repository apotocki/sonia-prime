//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_BOOST_STACKTRACE_HPP
#define SONIA_UTILITY_BOOST_STACKTRACE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/stacktrace.hpp>

#if defined(_MSC_VER) && (defined(BOOST_STACKTRACE_LINK) || defined(BOOST_STACKTRACE_DYN_LINK))

#if defined(BOOST_STACKTRACE_USE_WINDBG)
#   define BOOST_LIB_NAME boost_stacktrace_windbg
#elif defined(BOOST_STACKTRACE_USE_WINDBG_CACHED)
#   define BOOST_LIB_NAME boost_stacktrace_windbg_cached
#elif defined(BOOST_STACKTRACE_USE_BACKTRACE)
#   define BOOST_LIB_NAME boost_stacktrace_backtrace
#elif defined(BOOST_STACKTRACE_USE_ADDR2LINE)
#   define BOOST_LIB_NAME boost_stacktrace_addr2line
#elif defined(BOOST_STACKTRACE_USE_NOOP)
#   define BOOST_LIB_NAME boost_stacktrace_noop
#else
#   define BOOST_LIB_NAME boost_stacktrace_basic
#endif

#define BOOST_DYN_LINK
#include <boost/config/auto_link.hpp>
#undef BOOST_LIB_NAME
#undef BOOST_DYN_LINK
#endif

#endif // SONIA_UTILITY_BOOST_STACKTRACE_HPP
