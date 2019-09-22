//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_QUOTE_HPP
#define SONIA_MPL_QUOTE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"

namespace sonia::mpl {

template <template <typename ...> class F>
struct quote
{
    template <typename ... PsT> static true_type check(remove_cvref_t<typename F<PsT...>::type> *);
    template <typename ... PsT> static false_type check(...);	

    template <typename ... PsT> using apply = conditional_t<
        decltype(quote::check<PsT...>(nullptr))::value,
        F<PsT...>,
        identity<F<PsT...>>
    >;
    template <typename ... PsT> using apply_t = typename apply<PsT...>::type;
};

}

#endif // SONIA_MPL_QUOTE_HPP
