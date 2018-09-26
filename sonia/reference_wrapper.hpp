//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_REFERENCE_WRAPPER_HPP
#define SONIA_REFERENCE_WRAPPER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <functional>
#include "sonia/type_traits.hpp"

namespace sonia {

using std::reference_wrapper;
using std::ref;
using std::cref;

}

#endif // SONIA_REFERENCE_WRAPPER_HPP
