//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_MPL_TRANSFROM_VIEW_HPP
#define SONIA_MPL_TRANSFROM_VIEW_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/mpl/sequence.hpp"
#include "sonia/mpl/iterator.hpp"
#include "sonia/mpl/lambda.hpp"

namespace sonia::mpl {

struct transform_view_tag{};
template <class SeqT, class F> struct transform_view;
template <class SeqT, class F> struct tag_of<transform_view<SeqT, F>> { using type = transform_view_tag; };

template <class SeqT, class F>
struct transform_view
{
    using seq_t = SeqT;
    using ftor_t = lambda_t<F>;

    explicit transform_view(SeqT & s) : base{s} {}

    SeqT & base;
};

template <class F, class SeqT>
transform_view<SeqT, F> make_transform_view(SeqT & s)
{
    return transform_view<SeqT, F>(s);
}

template <class F, class SeqT>
transform_view<const SeqT, F> make_transform_view(SeqT const& s)
{
    return transform_view<const SeqT, F>(s);
}

#if 0

template <> struct size_impl<transform_view_tag>
{
    template <typename TV> using apply = size<typename TV::seq_t>;
};

template <> struct at_impl<transform_view_tag>
{
    template <class TV, class IdxT> using apply = typename TV::ftor_t::template apply<at_t<typename TV::seq_t, IdxT>>;
};

template <> struct at_c_impl<transform_view_tag>
{
    template <class TV, size_t IdxV> using apply = typename TV::ftor_t::template apply<at_c_t<typename TV::seq_t, IdxV>>;
};

template <> struct begin_impl<transform_view_tag>
{
    template <typename TV> struct apply { using type = iterator<0, TV>; };
};

template <> struct end_impl<transform_view_tag>
{
    template <typename TV> struct apply { using type = iterator<size_v<typename TV::seq_t>, TV>; };
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

template <size_t I, class SeqT, class F>
decltype(auto) at_c(mpl::transform_view<SeqT, F> const& tv)
{
    return typename F::template apply<mpl::at_c_t<SeqT, I>, I>()(std::get<I>(tv.base));
}

template <class SeqT, class F>
struct is_mpl_sequence<mpl::transform_view<SeqT, F>> : true_type {};

}

#endif // SONIA_MPL_TRANSFROM_VIEW_HPP
