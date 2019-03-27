//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_EXCEPTIONS_INTERNAL_ERRORS_HPP
#define SONIA_EXCEPTIONS_INTERNAL_ERRORS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>

#include "sonia/exceptions.hpp"

namespace sonia {
    
using trace_info = boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace>;

#define THROW_PARTICULAR_ERROR(cl, current_function, file, line, ...) \
    throw boost::enable_error_info(cl(__VA_ARGS__)) << boost::throw_function(current_function) \
        << boost::throw_file(file) \
        << boost::throw_line(line) \
        << trace_info(boost::stacktrace::stacktrace())

#define THROW_INTERNAL_ERROR(...) THROW_PARTICULAR_ERROR(internal_error, BOOST_THROW_EXCEPTION_CURRENT_FUNCTION,__FILE__,__LINE__,##__VA_ARGS__)
#define THROW_NOT_IMPLEMENTED_ERROR(...) THROW_PARTICULAR_ERROR(not_implemented_error, BOOST_THROW_EXCEPTION_CURRENT_FUNCTION,__FILE__,__LINE__,##__VA_ARGS__)
#define THROW_NOT_SUPPORTED_ERROR(...) THROW_PARTICULAR_ERROR(not_supported_operation_error, BOOST_THROW_EXCEPTION_CURRENT_FUNCTION,__FILE__,__LINE__,##__VA_ARGS__)

}

#endif // SONIA_EXCEPTIONS_INTERNAL_ERRORS_HPP
