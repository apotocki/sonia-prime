//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_NEXT_HPP
#define SONIA_MPL_NEXT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <tuple>

namespace sonia::mpl {

template <typename SeqT> struct begin;
template <typename SeqT> using begin_t = typename begin<SeqT>::type;

template <typename ArgT0, typename ... ArgsT> struct begin<std::tuple<ArgT0, ArgsT ...>>
{
    using type = ArgT0;
};

}

#endif // SONIA_MPL_NEXT_HPP
