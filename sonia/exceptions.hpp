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
    template <class ArgT>
    explicit exception(ArgT && arg) 
        : std::runtime_error(sonia::to_string(std::forward<ArgT>(arg)))
    {}
};

class shutdown_exception : public exception {
public:
    template <class ArgT>
    explicit shutdown_exception(ArgT && arg) : exception(std::forward<ArgT>(arg)) {}
};

class internal_error : public exception {
public:
    template <class ArgT>
    explicit internal_error(ArgT && arg) : exception(std::forward<ArgT>(arg)) {}
};

class closed_exception : public exception {
public:
    template <class ArgT>
    explicit closed_exception(ArgT && arg) : exception(std::forward<ArgT>(arg)) {}
};

class not_implemented_error : public internal_error {
public:
    template <class ArgT>
    explicit not_implemented_error(ArgT && arg) : internal_error(std::forward<ArgT>(arg)) {}
};

}

#endif // SONIA_EXCEPTIONS_HPP
