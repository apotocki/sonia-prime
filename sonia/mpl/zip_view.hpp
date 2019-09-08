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

#if 1

template <> struct size_impl<zip_view_tag>
{
    template <class ZV> struct apply : deref_t<min_element_t<transform_view<typename ZV::seqs_t, size<_1>>>>{};
};

template <> struct at_impl<zip_view_tag>
{
    template <class ZV, class IdxT> struct apply
    {
        using type = transform_view<typename ZV::seqs_t, at<_1, IdxT>>;
        //using type = transform_view<typename ZV::seqs_t, _1>;
    };
};

template <> struct at_c_impl<zip_view_tag>
{
    template <class ZV, size_t IdxV> struct apply
    {
        using type = transform_view<typename ZV::seqs_t, at<_1, integral_constant<size_t, IdxV>>>;
        //using type = transform_view<typename ZV::seqs_t, at_t<_1, integral_constant<size_t, 0>>>;
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

template <class SeqT, class F> struct size<transform_view<SeqT, F>> : size<SeqT> {};
template <class SeqT, class F, class IdxT> struct at<transform_view<SeqT, F>, IdxT> : lambda_t<F>::template apply<at_t<SeqT, IdxT>> {};
template <class SeqT, class F, size_t IdxV> struct at_c<transform_view<SeqT, F>, IdxV> : lambda_t<F>::template apply<at_c_t<SeqT, IdxV>> {};
template <class SeqT, class F> struct begin<transform_view<SeqT, F>> { using type = iterator<0, transform_view<SeqT, F>>; };
template <class SeqT, class F> struct end<transform_view<SeqT, F>> { using type = iterator<size_v<SeqT>, transform_view<SeqT, F>>; };

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
