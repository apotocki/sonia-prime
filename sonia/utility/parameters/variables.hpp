//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_PARAMETERS_VALUES_HPP
#define SONIA_PARAMETERS_VALUES_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/unordered_map.hpp>
#include <boost/any.hpp>

#include "sonia/string.hpp"

namespace sonia { namespace parameters {

class variable {
public:
    explicit variable(boost::any const& rval) : val_(rval) {}

    template <typename T>
    T const& as() const {
        return boost::any_cast<T>(val_);
    }

private:
    boost::any const& val_;
};

class variables {
public:
    variable operator[](string_view) const;
    bool is_available(string_view) const;

    void set_variable(string_view, boost::any);

private:
    boost::unordered_map<std::string, boost::any> map_;
};


}}

#endif // SONIA_PARAMETERS_VALUES_HPP
