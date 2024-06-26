//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/variant.hpp>
#include <boost/variant/multivisitors.hpp>

#include "sonia/utility/functional/equal.hpp"

namespace sonia {

using boost::variant;
using boost::recursive_variant_;
using boost::static_visitor;
using boost::apply_visitor;
using boost::get;
using boost::make_recursive_variant;
using boost::recursive_wrapper;

}

#define SONIA_VARIANT_DECLARED

#include "sonia/utility/functional/equal/variant.hpp"
#include "sonia/utility/functional/less/variant.hpp"
#include "sonia/utility/functional/hash/variant.hpp"
//#include "sonia/utility/streaming/variant.hpp"
