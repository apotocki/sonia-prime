//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_LAMBDA_HPP
#define SONIA_MPL_LAMBDA_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/mpl/bind.hpp"
#include "sonia/mpl/protect.hpp"
#include "sonia/mpl/quote.hpp"

namespace sonia::mpl {

template <typename X> struct is_placeholder_expression : false_type {};
template <typename X> struct is_placeholder_expression<protect<X>> : false_type {};
template <int I> struct is_placeholder_expression<arg_c<I>> : true_type {};
template <template <typename ...> class F, typename ... ArgsT> struct is_placeholder_expression<F<ArgsT...>>
{
    static constexpr bool test() { return (is_placeholder_expression<ArgsT>::value || ...); }
    static constexpr bool value = test();
    using type = bool_constant<value>;
};

template <typename X> struct placeholder_lambda;
template <typename X> struct unprotected_placeholder_lambda;

template <typename X> struct lambda
{
    using type = typename conditional_t<is_placeholder_expression<X>::value, placeholder_lambda<X>, identity<X>>::type;
};

template <int I> struct lambda<arg_c<I>> { using type = arg_c<I>; };

template <typename X> struct unprotected_lambda
{
    using type = typename conditional_t<is_placeholder_expression<X>::value, unprotected_placeholder_lambda<X>, identity<X>>::type;
};

template <int I> struct unprotected_lambda<arg_c<I>> { using type = arg_c<I>; };

template <template <typename ...> class F, typename ... ArgsT> struct placeholder_lambda<F<ArgsT...>>
{
    using type = protect_t<bind<quote<F>, typename unprotected_lambda<ArgsT>::type ...>>;
};

template <template <typename ...> class F, typename ... ArgsT> struct unprotected_placeholder_lambda<F<ArgsT...>>
{
    using type = bind<quote<F>, typename unprotected_lambda<ArgsT>::type ...>;
};

template <typename X> using lambda_t = typename lambda<X>::type;

}

#endif // SONIA_MPL_LAMBDA_HPP
