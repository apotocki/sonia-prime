//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_PROTECT_HPP
#define SONIA_MPL_PROTECT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/mpl/bind.hpp"

namespace sonia::mpl {

template <typename T> struct protect { using type = T; };
template <typename ... ArgsT> struct protect<bind<ArgsT...>>
{
    template <typename ... BArgsT> using apply = typename bind<ArgsT...>::template apply<BArgsT...>;
    using type = protect;
};
template <typename T> using protect_t = typename protect<T>::type;

}

#endif // SONIA_MPL_PROTECT_HPP
