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
#include <string>

#include <boost/throw_exception.hpp>

namespace sonia {

class exception : public std::runtime_error {
public:
    template <class ArgT>
    exception(ArgT && arg) : runtime_error(std::string(std::forward<ArgT>(arg))) {}
};

class internal_error : public exception {
public:
    template <class ArgT>
    internal_error(ArgT && arg) : exception(std::string(std::forward<ArgT>(arg))) {}
};

class not_implemented_error : public internal_error {
public:
    template <class ArgT>
    not_implemented_error(ArgT && arg) : internal_error(std::string(std::forward<ArgT>(arg))) {}
};

}

#endif // SONIA_EXCEPTIONS_HPP
