//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_EXCEPTIONS_HPP
#define SONIA_EXCEPTIONS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <stdexcept>
#include <utility>

#include "sonia/string.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/utility/boost_stacktrace.hpp"

#include <boost/exception/all.hpp>
#include <boost/throw_exception.hpp>

namespace sonia {

class exception : public std::runtime_error
{
public:
    template <typename ... ArgsT>
    exception(ArgsT && ... args) : std::runtime_error(construct(std::forward<ArgsT>(args) ...)) {}

private:
    static std::string construct() { return "error"; }
    static std::runtime_error const& construct(exception & e) { return e; }
    template <typename ... ArgsT> static std::string construct(ArgsT && ... args) { return (... + sonia::to_string(std::forward<ArgsT>(args))); }
};

class silence_exception : public exception {};

#define DECLARE_EXCEPTION(name, msg, base)                                      \
class name : public base                                                        \
{                                                                               \
public:                                                                         \
    template <typename ... ArgsT>                                               \
    name(ArgsT && ... args) : base(construct(std::forward<ArgsT>(args) ...)) {} \
private:                                                                        \
    static base construct() { return base{msg}; }                               \
    static base const& construct(name & e) { return e; }                        \
    template <class ArgT> base construct(ArgT && arg)                           \
    { return base{std::forward<ArgT>(arg)}; }                                   \
    template <class ... ArgsT> base construct(ArgsT && ... args)                \
    { return base{std::forward<ArgsT>(args) ...}; }                             \
};

DECLARE_EXCEPTION(eof_exception, "end of file", exception)
DECLARE_EXCEPTION(closed_exception, "closed", exception)
DECLARE_EXCEPTION(timeout_exception, "timeout", exception)
DECLARE_EXCEPTION(fatal_error, "fatal error", exception)
DECLARE_EXCEPTION(client_error, "client error", exception)

DECLARE_EXCEPTION(internal_error, "internal error", exception)
DECLARE_EXCEPTION(not_implemented_error, "not implemented error", internal_error)
DECLARE_EXCEPTION(not_supported_operation_error, "not supported operation error", internal_error)
DECLARE_EXCEPTION(illegal_argument_error, "illegal argument", internal_error)

using trace_info = boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace>;

#define THROW_PARTICULAR_ERROR_RAW(cl, current_function, file, line, ...) \
    throw boost::enable_error_info(cl(__VA_ARGS__)) << boost::throw_function(current_function) \
        << boost::throw_file(file) \
        << boost::throw_line(line) \
        << sonia::trace_info(boost::stacktrace::stacktrace())

#define THROW_PARTICULAR_ERROR_RAW2(err, current_function, file, line) \
    throw boost::enable_error_info(err) << boost::throw_function(current_function) \
        << boost::throw_file(file) \
        << boost::throw_line(line) \
        << sonia::trace_info(boost::stacktrace::stacktrace())

#define THROW_ERROR(err) THROW_PARTICULAR_ERROR_RAW2(err, BOOST_THROW_EXCEPTION_CURRENT_FUNCTION,__FILE__,__LINE__)

#define THROW_INTERNAL_ERROR(...) THROW_PARTICULAR_ERROR_RAW2(sonia::internal_error(__VA_ARGS__), BOOST_THROW_EXCEPTION_CURRENT_FUNCTION,__FILE__,__LINE__)
#define THROW_NOT_IMPLEMENTED_ERROR(...) THROW_PARTICULAR_ERROR_RAW(sonia::not_implemented_error, BOOST_THROW_EXCEPTION_CURRENT_FUNCTION,__FILE__,__LINE__,##__VA_ARGS__)
#define THROW_NOT_SUPPORTED_ERROR(...) THROW_PARTICULAR_ERROR_RAW(sonia::not_supported_operation_error, BOOST_THROW_EXCEPTION_CURRENT_FUNCTION,__FILE__,__LINE__,##__VA_ARGS__)
#define THROW_FATAL_ERROR(...) THROW_PARTICULAR_ERROR_RAW2(sonia::fatal_error(__VA_ARGS__), BOOST_THROW_EXCEPTION_CURRENT_FUNCTION,__FILE__,__LINE__)

}

#endif // SONIA_EXCEPTIONS_HPP
