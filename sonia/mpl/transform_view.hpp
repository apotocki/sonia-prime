//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_TRANSFROM_VIEW_HPP
#define SONIA_MPL_TRANSFROM_VIEW_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sequence.hpp"

namespace sonia::mpl {
    
template <class SequenceT, class TransformerT>
struct transform_view
{
    explicit transform_view(SequenceT & s) : base(s) {}

    SequenceT & base;
};

template <class TransformerT, class SequenceT>
transform_view<SequenceT, TransformerT> make_transform_view(SequenceT & s)
{
    return transform_view<SequenceT, TransformerT>(s);
}

template <class TransformerT, class SequenceT>
transform_view<const SequenceT, TransformerT> make_transform_view(SequenceT const& s)
{
    return transform_view<const SequenceT, TransformerT>(s);
}

template <class SequenceT, class TransformerT>
struct size<transform_view<SequenceT, TransformerT>> : size<SequenceT> {};

template <size_t I, class SequenceT, class TransformerT>
decltype(auto) at_c(transform_view<SequenceT, TransformerT> const& tv)
{
    return typename TransformerT::template apply<at_t<SequenceT, I>, I>()(std::get<I>(tv.base));
}

}

namespace sonia {

template <class SequenceT, class TransformerT>
struct is_mpl_sequence<mpl::transform_view<SequenceT, TransformerT>> : true_type {};

}

#endif // SONIA_MPL_TRANSFROM_VIEW_HPP
