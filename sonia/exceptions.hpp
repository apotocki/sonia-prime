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

#include <boost/throw_exception.hpp>

#include "sonia/string.hpp"

namespace sonia {

class exception : public std::runtime_error
{
public:
    exception() : std::runtime_error("error") {}

    template <class ... ArgsT>
    exception(ArgsT && ... args) 
        : std::runtime_error((... + sonia::to_string(std::forward<ArgsT>(args))))
    {}
};

class silence_exception : public exception {};

#define DECLARE_EXCEPTION(name, msg, base)                                      \
class name : public base                                                        \
{                                                                               \
public:                                                                         \
    name() : base(#msg) {}                                                      \
    name(name const&) = default;                                                \
                                                                                \
    template <class ArgT>                                                       \
    explicit name(ArgT && arg)                                                  \
        : base(#msg ": ", std::forward<ArgT>(arg))                              \
    {}                                                                          \
                                                                                \
    template <class ArgT0, class ArgT1, class ... ArgsT>                        \
    name(ArgT0 && arg0, ArgT1 && arg1, ArgsT && ... args)                       \
        : base(std::forward<ArgT0>(arg0), std::forward<ArgT1>(arg1),            \
            std::forward<ArgsT>(args) ...)                                      \
    {}                                                                          \
};

DECLARE_EXCEPTION(eof_exception, "end of file", exception)
DECLARE_EXCEPTION(closed_exception, "closed", exception)
DECLARE_EXCEPTION(fatal_error, "fatal error", exception)
DECLARE_EXCEPTION(client_error, "client error", exception)

DECLARE_EXCEPTION(internal_error, "internal error", exception)
DECLARE_EXCEPTION(not_implemented_error, "not implemented error", internal_error)
DECLARE_EXCEPTION(not_supported_operation_error, "not supported operation error", internal_error)
DECLARE_EXCEPTION(illegal_argument_error, "illegal argument", internal_error)

}

#endif // SONIA_EXCEPTIONS_HPP
