//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_POP_FRONT_HPP
#define SONIA_MPL_POP_FRONT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <tuple>

namespace sonia::mpl {

template <typename SeqT> struct pop_front;
template <typename SeqT> using pop_front_t = typename pop_front<SeqT>::type;

template <typename T, typename ... ArgsT> struct pop_front<std::tuple<T, ArgsT ...>>
{
    using type = std::tuple<ArgsT ...>;
};

}

#endif // SONIA_MPL_POP_FRONT_HPP
