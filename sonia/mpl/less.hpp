//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_LESS_HPP
#define SONIA_MPL_LESS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

namespace sonia::mpl {

template <typename LHS, typename RHS> struct less : bool_constant<(LHS::value < RHS::value)> {};

}

#endif // SONIA_MPL_LESS_HPP
