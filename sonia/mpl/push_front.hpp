//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_PUSH_FRONT_HPP
#define SONIA_MPL_PUSH_FRONT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <tuple>

namespace sonia::mpl {

template <typename SeqT, typename T> struct push_front;
template <typename SeqT, typename T> using push_front_t = typename push_front<SeqT, T>::type;

template <typename ... ArgsT, typename T> struct push_front<std::tuple<ArgsT ...>, T>
{
    using type = std::tuple<T, ArgsT ...>;
};

}

#endif // SONIA_MPL_PUSH_FRONT_HPP
