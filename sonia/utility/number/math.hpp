//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_NUMBER_ROUND_HPP
#define SONIA_UTILITY_NUMBER_ROUND_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "decimal.hpp"

namespace sonia {

void numeric_round(decimal &);
void ceil(decimal &);
void floor(decimal &);

}

#endif // SONIA_UTILITY_NUMBER_ROUND_HPP
