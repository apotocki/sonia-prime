//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_DECIMAL_HPP
#define SONIA_UTILITY_DECIMAL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/multiprecision/cpp_int.hpp>

#include "basic_decimal.hpp"

namespace sonia {

typedef basic_decimal<
    boost::multiprecision::number<boost::multiprecision::cpp_int_backend<65, 0>>,
    int32_t
> decimal;

}

#endif // SONIA_UTILITY_DECIMAL_HPP
