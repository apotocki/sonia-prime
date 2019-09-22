//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_ZIP_VIEW_HPP
#define SONIA_MPL_ZIP_VIEW_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/mpl/sequence.hpp"
#include "sonia/mpl/iterator.hpp"
#include "sonia/mpl/min_element.hpp"
#include "sonia/mpl/transform_view.hpp"

namespace sonia::mpl {

struct zip_view_tag{};
template <class SeqOfSeqsT> struct zip_view;
template <class SeqOfSeqsT> struct tag_of<zip_view<SeqOfSeqsT>> { using type = zip_view_tag; };

template <class SeqOfSeqsT>
struct zip_view
{
    using seqs_t = SeqOfSeqsT;
 
    explicit zip_view(SeqOfSeqsT & s) : base{s} {}

    SeqOfSeqsT & base;
};

#if 0

template <> struct size_impl<zip_view_tag>
{
    template <class ZV> using apply = deref_t<min_element_t<transform_view<typename ZV::seqs_t, size<_1>>>>;
};

template <> struct at_impl<zip_view_tag>
{
    template <class ZV, class IdxT> struct apply
    {
        using type = transform_view<typename ZV::seqs_t, at<_1, IdxT>>;
    };
};

template <> struct at_c_impl<zip_view_tag>
{
    template <class ZV, size_t IdxV> struct apply
    {
        using type = transform_view<typename ZV::seqs_t, at<_1, integral_constant<size_t, IdxV>>>;
    };
};

template <> struct begin_impl<zip_view_tag>
{
    template <typename ZV> struct apply { using type = iterator<0, ZV>; };
};

template <> struct end_impl<zip_view_tag>
{
    template <typename ZV> struct apply { using type = iterator<size_v<ZV>, ZV>; };
};

# else

template <class SeqsT> struct size<zip_view<SeqsT>> : deref_t<min_element_t<transform_view<SeqsT, size<_1>>>> {};
template <class SeqsT, class IdxT> struct at<zip_view<SeqsT>, IdxT> { using type = transform_view<SeqsT, at<_1, IdxT>>; };
template <class SeqsT, size_t IdxV> struct at_c<zip_view<SeqsT>, IdxV> { using type = transform_view<SeqsT, at<_1, integral_constant<size_t, IdxV>>>; };
template <class SeqsT> struct begin<zip_view<SeqsT>> { using type = iterator<0, zip_view<SeqsT>>; };
template <class SeqsT> struct end<zip_view<SeqsT>> { using type = iterator<size_v<zip_view<SeqsT>>, zip_view<SeqsT>>; };

#endif

}

namespace sonia {

//template <size_t I, class SeqsT>
//decltype(auto) at_c(mpl::zip_view<SeqsT> const& zv)
//{
//    return typename F::template apply<mpl::at_c_t<SeqT, I>, I>()(std::get<I>(tv.base));
//}

template <class SeqsT>
struct is_mpl_sequence<mpl::zip_view<SeqsT>> : true_type {};

}

#endif // SONIA_MPL_ZIP_VIEW_HPP
