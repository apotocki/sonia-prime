//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/mpl/sequence.hpp"

namespace sonia::mpl {

template <size_t PosV, typename SeqT> struct iterator{};

template <typename IteratorT> struct deref { using type = void; };
template <typename IteratorT> using deref_t = typename deref<IteratorT>::type;
template <size_t PosV, typename SeqT> struct deref<iterator<PosV, SeqT>>
{
    using type = at_c_t<SeqT, PosV>;
};

template <typename IteratorT> struct next { using type = void; };
template <typename IteratorT> using next_t = typename next<IteratorT>::type;
template <size_t PosV, typename SeqT> struct next<iterator<PosV, SeqT>>
{
    using type = iterator<PosV + 1, SeqT>;
};

template <typename IteratorT> struct prior { using type = void; };
template <typename IteratorT> using prior_t = typename next<IteratorT>::type;
template <size_t PosV, typename SeqT> struct prior<iterator<PosV, SeqT>>
{
    using type = iterator<PosV - 1, SeqT>;
};

template <typename FromIteratorT, typename ToIteratorT> struct distance{};
template <typename FromIteratorT, typename ToIteratorT> static constexpr size_t distance_v = distance<FromIteratorT, ToIteratorT>::value;
template <size_t FromV, size_t ToV, typename SeqT> struct distance<iterator<FromV, SeqT>, iterator<ToV, SeqT>> : std::integral_constant<size_t, ToV - FromV>
{ static_assert(FromV <= ToV); };

}
