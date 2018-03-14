//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_CONFIG_HPP
#define SONIA_CONFIG_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
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

#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <boost/config.hpp>

#endif // SONIA_CONFIG_HPP
