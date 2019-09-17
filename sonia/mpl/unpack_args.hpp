//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_UNPACK_ARGS_HPP
#define SONIA_MPL_UNPACK_ARGS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/mpl/as_vector.hpp"
#include "sonia/mpl/lambda.hpp"

namespace sonia::mpl {

template <typename F> struct unpack_args_impl
{
    template <typename SeqT> struct apply : apply<as_vector_t<SeqT>> {};
    template <typename ... Types> struct apply<vector<Types...>> : F::template apply<Types...> {};
};

template <typename F> using unpack_args = unpack_args_impl<lambda_t<F>>;

//template <typename F>
//struct unpack_args
//{
//    template <typename SeqT> struct do_apply;
//    template <typename ... Types> struct do_apply<vector<Types...>> : lambda_t<F>::template apply<Types...> {};
//    //template <typename ... Types> struct do_apply<vector<Types...>> : bind<F, Types...> {};
//
//    //template <typename SeqT> struct apply : apply<as_vector_t<SeqT>> {};
//    template <typename SeqT> struct apply : do_apply<as_vector_t<SeqT>> {};
//    //template <typename ... Types> struct apply<vector<Types...>> : lambda_t<F>::template apply<Types...> {};
//};

}

#endif // SONIA_MPL_UNPACK_ARGS_HPP
