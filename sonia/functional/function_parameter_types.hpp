//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL__FUNCTION_PARAMETER_TYPES_HPP
#define SONIA_FUNCTIONAL__FUNCTION_PARAMETER_TYPES_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/mpl/vector.hpp"

namespace sonia {

template <typename FuncT> struct function_parameter_types;
template <typename FuncT> using function_parameter_types_t = typename function_parameter_types<FuncT>::type;

template <typename R, typename ...ArgsT> struct function_parameter_types<R(ArgsT...)>
{
    using type = mpl::vector<ArgsT...>;
};

template <typename SeqT> struct synthesize_function_type;
template <typename SeqT> using synthesize_function_type_t = typename synthesize_function_type<SeqT>::type;

template <typename ResultT, typename ... ArgsT> struct synthesize_function_type<mpl::vector<ResultT, ArgsT...>>
{
    using type = ResultT(ArgsT...);
};

}

#endif // SONIA_FUNCTIONAL__FUNCTION_PARAMETER_TYPES_HPP
