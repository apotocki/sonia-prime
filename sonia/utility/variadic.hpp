//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILIT_VARIADIC_HPP
#define SONIA_UTILIT_VARIADIC_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

namespace sonia { namespace variadic {

template <size_t N, typename ArgT0, typename ... ArgsT> struct type_at : type_at<N - 1, ArgsT ...> {};
template <typename ArgT0, typename ... ArgsT> struct type_at<0, ArgT0, ArgsT ...> { typedef ArgT0 type; };
template <size_t N, typename ... ArgsT> using type_at_t = typename type_at<N, ArgsT...>::type;

template <size_t N, typename ArgT0, typename ... ArgsT> struct forward_at_impl : forward_at_impl<N - 1, ArgsT ...> {
    typedef forward_at_impl<N - 1, ArgsT ...> base_t;
    auto&& operator()(ArgT0&&, ArgsT&& ... args) const { return base_t::operator()(std::forward<ArgsT>(args) ...); }
};

template <typename ArgT0, typename ... ArgsT> struct forward_at_impl<0, ArgT0, ArgsT ...> {
    typedef ArgT0 result_type;
    auto && operator()(ArgT0&& arg0, ArgsT&& ...) const { return std::forward<ArgT0>(arg0); }
};

template <size_t N, typename ... ArgsT>
auto&& forward_at(ArgsT&& ... args) {
    return forward_at_impl<N, ArgsT...>()(std::forward<ArgsT>(args) ...);
}

}}

#endif // SONIA_UTILIT_VARIADIC_HPP
