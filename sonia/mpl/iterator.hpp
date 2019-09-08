//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_BEGIN_HPP
#define SONIA_MPL_BEGIN_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <tuple>

namespace sonia::mpl {

template <int PosV, typename SeqT> struct iterator{};

template <typename IteratorT> struct deref { using type = void; };
template <typename IteratorT> using deref_t = typename deref<IteratorT>::type;
template <size_t PosV, typename ... ArgsT> struct deref<iterator<PosV, std::tuple<ArgsT...>>>
{
    using type = std::tuple_element_t<PosV, std::tuple<ArgsT...>>;
};

template <typename IteratorT> struct next { using type = void; };
template <typename IteratorT> using next_t = typename next<IteratorT>::type;
template <size_t PosV, typename ... ArgsT> struct next<iterator<PosV, std::tuple<ArgsT...>>>
{
    using type = iterator<PosV + 1, std::tuple<ArgsT...>>;
};

template <typename IteratorT> struct prior { using type = void; };
template <typename IteratorT> using prior_t = typename next<IteratorT>::type;
template <size_t PosV, typename ... ArgsT> struct prior<iterator<PosV, std::tuple<ArgsT...>>>
{
    using type = iterator<PosV - 1, std::tuple<ArgsT...>>;
};

template <typename SeqT> struct begin { using type = void; };
template <typename SeqT> using begin_t = typename begin<SeqT>::type;
template <typename ... ArgsT> struct begin<std::tuple<ArgsT...>>
{
    using type = iterator<0, std::tuple<ArgsT...>>;
};

template <typename SeqT> struct end { using type = void; };
template <typename SeqT> using end_t = typename end<SeqT>::type;
template <typename ... ArgsT> struct end<std::tuple<ArgsT...>>
{
    using type = iterator<sizeof ...(ArgsT), std::tuple<ArgsT...>>;
};

}

#endif // SONIA_MPL_BEGIN_HPP
