//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_PARAMETERS_VALUE_DESCRIPTOR_HPP
#define SONIA_PARAMETERS_VALUE_DESCRIPTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <boost/any.hpp>
#include "sonia/utility/json/value.hpp"

namespace sonia { namespace parameters {





template <typename T>
class typed_arrayvalue_descriptor : public value_descriptor {
public:
    typed_arrayvalue_descriptor * required() noexcept { required_ = true; return this; }
};

template <typename T>
value_descriptor * value() {
    return new typed_value_descriptor<T>();
}

template <typename T>
value_descriptor * array_value() {
    return new typed_arrayvalue_descriptor<T>();
}

}}

#endif // SONIA_PARAMETERS_VALUE_DESCRIPTOR_HPP
