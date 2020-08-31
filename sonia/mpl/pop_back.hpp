//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_POP_BACK_HPP
#define SONIA_MPL_POP_BACK_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <tuple>

namespace sonia::mpl {

template <typename SeqT> struct pop_back;
template <typename SeqT> using pop_back_t = typename pop_back<SeqT>::type;

template <typename T, typename ... ArgsT> struct pop_back<std::tuple<ArgsT ..., T>>
{
    using type = std::tuple<ArgsT ...>;
};

}

#endif // SONIA_MPL_POP_BACK_HPP
