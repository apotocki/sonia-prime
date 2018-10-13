//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_CONFIG_HPP
#define SONIA_CONFIG_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

#ifndef BOOST_ENABLE_ASSERT_HANDLER
#   define BOOST_ENABLE_ASSERT_HANDLER
#endif

// VC specific
#if defined(_MSC_VER) & !defined(_CRT_SECURE_NO_WARNINGS) 
#   define _CRT_SECURE_NO_WARNINGS
#endif

#if defined(_MSC_VER) & !defined(_SCL_SECURE_NO_WARNINGS) 
#   define _SCL_SECURE_NO_WARNINGS
#endif

#if defined(_MSC_VER) & !defined(_ITERATOR_DEBUG_LEVEL) 
#   define _ITERATOR_DEBUG_LEVEL 0
#endif

#if defined(_MSC_VER)
#   define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
//#   define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
#endif

#if (defined(__MINGW32__) || defined(__MINGW64__))
#   define _WIN32_WINNT 0x0601 // Windows 7
#   define WINVER _WIN32_WINNT
#endif

#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <boost/config.hpp>

#define SONIA_ONE_VERSION "0.1.0"

#ifndef BOOST_THREAD_VERSION
#   define BOOST_THREAD_VERSION 3
#endif

#endif // SONIA_CONFIG_HPP
