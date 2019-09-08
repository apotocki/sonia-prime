//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_BEGIN_HPP
#define SONIA_MPL_BEGIN_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

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

}

#endif // SONIA_MPL_BEGIN_HPP
