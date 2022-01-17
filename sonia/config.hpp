//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

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

#if defined(_MSC_VER) & !defined(_SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING) 
#   define _SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING // boost::archive
#	define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING // boost::asio
#	define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS // boost::circular_buffer
#endif

#if (defined(__MINGW32__) || defined(__MINGW64__))
#   ifndef _WIN32_WINNT
#       define _WIN32_WINNT 0x0601 // Windows 7
#   endif
#   ifndef WINVER
#       define WINVER _WIN32_WINNT
#   endif
#endif

#define BOOST_BIND_NO_PLACEHOLDERS
#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <boost/config.hpp>

#define SONIA_ONE_VERSION "0.1.0"

#ifdef BOOST_WINDOWS
#   define BOOST_STACKTRACE_USE_WINDBG
#endif

#ifndef BOOST_THREAD_VERSION
#   define BOOST_THREAD_VERSION 3
#endif

#if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
#   error "Outdated compiler (no variadic templates)"
#endif

#define DO_NOT_USE_AGNOSTIC_REMOVE_CONST
#define DO_NOT_USE_AGNOSTIC_REMOVE_VOLATILE
#define DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#define DO_NOT_USE_AGNOSTIC_ALIGNMENT_OF
#define DO_NOT_USE_AGNOSTIC_ENABLE_IF
#define DO_NOT_USE_AGNOSTIC_PLACEMENT_NEW
#define DO_NOT_USE_AGNOSTIC_DESTROY_AT
#define DO_NOT_USE_AGNOSTIC_FORWARD
#define DO_NOT_USE_AGNOSTIC_CONDITIONAL
#define DO_NOT_USE_AGNOSTIC_TYPE_IDENTITY
#define DO_NOT_USE_AGNOSTIC_ADDRESSOF
#define DO_NOT_USE_AGNOSTIC_POINTER_TRAITS
#define DO_NOT_USE_AGNOSTIC_ALLOCATOR_TRAITS
#define DO_NOT_USE_AGNOSTIC_ADD_CONST
#define DO_NOT_USE_AGNOSTIC_ADD_LVALUE_REFERENCE
#define DO_NOT_USE_AGNOSTIC_REMOVE_CV
#define DO_NOT_USE_AGNOSTIC_REMOVE_REFERENCE
#define DO_NOT_USE_AGNOSTIC_ITERATOR_TAGS
#define DO_NOT_USE_AGNOSTIC_ITERATOR_TRAITS
#define DO_NOT_USE_AGNOSTIC_REVERSE_ITERATOR
#define DO_NOT_USE_AGNOSTIC_ALLOCATOR
#define DO_NOT_USE_AGNOSTIC_IS_ARRAY
#define DO_NOT_USE_AGNOSTIC_IS_POINTER
#define DO_NOT_USE_AGNOSTIC_IS_SAME
#define DO_NOT_USE_AGNOSTIC_IS_VOID
#define DO_NOT_USE_AGNOSTIC_IS_ENUM
#define DO_NOT_USE_AGNOSTIC_IS_FUNCTION
#define DO_NOT_USE_AGNOSTIC_IS_INTEGRAL
#define DO_NOT_USE_AGNOSTIC_IS_FLOATING_POINT
#define DO_NOT_USE_AGNOSTIC_IS_ARITHMETIC
#define DO_NOT_USE_AGNOSTIC_IS_TRIVIAL
#define DO_NOT_USE_AGNOSTIC_IS_MEMBER_POINTER
#define DO_NOT_USE_AGNOSTIC_IS_NULL_POINTER
#define DO_NOT_USE_AGNOSTIC_IS_SCALAR
#define DO_NOT_USE_AGNOSTIC_IS_VOLATILE
#define DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_COPYABLE
#define DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_DESTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_DEFAULT_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_ASSIGNABLE
#define DO_NOT_USE_AGNOSTIC_IS_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_CONVERTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_COPY_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_NOTHROW_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_NOTHROW_COPY_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_NOTHROW_MOVE_CONSTRUCTIBLE
#define DO_NOT_USE_AGNOSTIC_IS_NOTHROW_MOVE_ASSIGNABLE
#define DO_NOT_USE_AGNOSTIC_DISJUNCTION
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_COPY
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_MOVE
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_MAX_ELEMENT
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_MAX
#define DO_NOT_USE_AGNOSTIC_UNINITIALIZED_COPY
#define DO_NOT_USE_AGNOSTIC_UNINITIALIZED_MOVE
#define DO_NOT_USE_AGNOSTIC_UNINITIALIZED_DEFAULT_CONSTRUCT
#define DO_NOT_USE_AGNOSTIC_UNINITIALIZED_VALUE_CONSTRUCT
#define DO_NOT_USE_AGNOSTIC_UNINITIALIZED_FILL
#define DO_NOT_USE_AGNOSTIC_DESTROY
#define DO_NOT_USE_AGNOSTIC_ITERATOR_BEGIN
#define DO_NOT_USE_AGNOSTIC_ITERATOR_END
#define DO_NOT_USE_AGNOSTIC_ADD_RVALUE_REFERENCE
#define DO_NOT_USE_AGNOSTIC_CONVERTIBLE_TO
#define DO_NOT_USE_AGNOSTIC_SAME_AS
#define DO_NOT_USE_AGNOSTIC_NOTHROW
#define DO_NOT_USE_AGNOSTIC_IN_PLACE
#define DO_NOT_USE_AGNOSTIC_FLOAT_ROUND_STYLE
#define DO_NOT_USE_AGNOSTIC_FLOAT_DENORM_STYLE
#define DO_NOT_USE_AGNOSTIC_NUMERIC_LIMITS
#define DO_NOT_USE_AGNOSTIC_INITIALIZER_LIST
#define DO_NOT_USE_AGNOSTIC_PAIR
#define DO_NOT_USE_AGNOSTIC_ORDERING
#define DO_NOT_USE_AGNOSTIC_UNDERLYING_TYPE
#define DO_NOT_USE_AGNOSTIC_LAUNDER
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_MIN_ELEMENT
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_MIN
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_ITER_SWAP
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_SWAP_RANGES
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_MISMATCH
#define DO_NOT_USE_AGNOSTIC_SAME_AS_CONCEPT
#define DO_NOT_USE_AGNOSTIC_RANDOM_ACCESS_ITERATOR_CONCEPT
#define DO_NOT_USE_AGNOSTIC_DECLVAL
#define DO_NOT_USE_AGNOSTIC_ALGORITHM_COMPARE_3WAY
#define DO_NOT_USE_AGNOSTIC_ORDERING
//#if __cplusplus > 201703L
#   define DO_NOT_USE_AGNOSTIC_REMOVE_CVREF
//#endif
