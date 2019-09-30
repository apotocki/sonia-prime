//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_LAMBDA_FWD_HPP
#define SONIA_MPL_LAMBDA_FWD_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"

namespace sonia::mpl {

using _1 = arg_c<1>;
using _2 = arg_c<2>;
using _3 = arg_c<3>;
using _4 = arg_c<4>;
using _5 = arg_c<5>;

template <typename X> struct is_placeholder_expression : false_type {};
template <int I> struct is_placeholder_expression<arg_c<I>> : true_type {};

}

#endif // SONIA_MPL_LAMBDA_FWD_HPP
