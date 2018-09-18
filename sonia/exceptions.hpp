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

class exception : public std::runtime_error {
public:
    template <class ... ArgsT>
    exception(ArgsT && ... args) 
        : std::runtime_error((... + sonia::to_string(std::forward<ArgsT>(args))))
    {}
};

class shutdown_exception : public exception {
public:
    template <class ArgT>
    explicit shutdown_exception(ArgT && arg) : exception("shutdown: ", std::forward<ArgT>(arg)) {}
};

class internal_error : public exception {
public:
    template <class ArgT>
    explicit internal_error(ArgT && arg)
        : exception("internal error: ", std::forward<ArgT>(arg))
    {}

    template <class ArgT0, class ... ArgsT>
    explicit internal_error(ArgT0 && arg0, ArgsT && ... args)
        : exception(std::forward<ArgT0>(arg0), std::forward<ArgsT>(args) ...)
    {}
};

class closed_exception : public exception {
public:
    template <class ArgT>
    explicit closed_exception(ArgT && arg)
        : exception("closed: ", std::forward<ArgT>(arg))
    {}

    template <class ArgT0, class ... ArgsT>
    explicit closed_exception(ArgT0 && arg0, ArgsT && ... args)
        : exception(std::forward<ArgT0>(arg0), std::forward<ArgsT>(args) ...)
    {}
};

class not_implemented_error : public internal_error {
public:
    not_implemented_error() : internal_error("not implemented") {}

    template <class ArgT>
    explicit not_implemented_error(ArgT && arg)
        : internal_error("not implemented error: ", std::forward<ArgT>(arg))
    {}

    template <class ArgT0, class ... ArgsT>
    explicit not_implemented_error(ArgT0 && arg0, ArgsT && ... args)
        : exception(std::forward<ArgT0>(arg0), std::forward<ArgsT>(args) ...)
    {}
};

}

#endif // SONIA_EXCEPTIONS_HPP
