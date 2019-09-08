//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_APPLY_HPP
#define SONIA_MPL_APPLY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/mpl/lambda.hpp"

namespace sonia::mpl {

template <typename F, typename ... ArgsT> using apply = typename lambda_t<F>::template apply<ArgsT ...>;
template <typename F, typename ... ArgsT> using apply_t = typename apply<F, ArgsT ...>::type;

}

#endif // SONIA_MPL_APPLY_HPP
