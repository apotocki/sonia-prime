//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_BIND_HPP
#define SONIA_MPL_BIND_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"

namespace sonia::mpl {

template <typename F, typename ... ArgsT> struct bind;

template <typename T, typename ... ArgsT> struct wrapped_arg
{
    using type = T;
};

template <int I, typename ... ArgsT> struct wrapped_arg<arg_c<I>, ArgsT...>
{
    using type = typename arg_c<I>::template apply_t<ArgsT...>;
};

template <typename F, typename ... BArgsT, typename ... ArgsT> struct wrapped_arg<bind<F, BArgsT...>, ArgsT...>
{
    using type = typename bind<F, BArgsT...>::template apply_t<ArgsT...>;
};

template <typename F, typename ... ArgsT>
struct bind
{
    template <typename ... UsT> using apply = 
        typename wrapped_arg<F, UsT...>::type::template apply<
            typename wrapped_arg<ArgsT, UsT...>::type ...
        >;
    template <typename ... UsT> using apply_t = typename apply<UsT...>::type;
};

}

#endif // SONIA_MPL_BIND_HPP
