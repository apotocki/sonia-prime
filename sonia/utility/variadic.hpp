//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_VARIADIC_HPP
#define SONIA_UTILITY_VARIADIC_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <type_traits>
#include <boost/mpl/identity.hpp>

namespace sonia::variadic {

template <size_t N, typename ArgT0, typename ... ArgsT> struct type_at : type_at<N - 1, ArgsT ...> {};
template <typename ArgT0, typename ... ArgsT> struct type_at<0, ArgT0, ArgsT ...> { using type = ArgT0; };
template <size_t N, typename ... ArgsT> using type_at_t = typename type_at<N, ArgsT...>::type;

template <size_t N, typename ArgT0, typename ... ArgsT> struct forward_at_impl : forward_at_impl<N - 1, ArgsT ...>
{
    using base_type = forward_at_impl<N - 1, ArgsT ...>;
    auto&& operator()(ArgT0&&, ArgsT&& ... args) const { return base_type::operator()(std::forward<ArgsT>(args) ...); }
};

template <typename ArgT0, typename ... ArgsT> struct forward_at_impl<0, ArgT0, ArgsT ...>
{
    using result_type = ArgT0;
    auto && operator()(ArgT0&& arg0, ArgsT&& ...) const { return std::forward<ArgT0>(arg0); }
};

template <size_t N, typename ... ArgsT>
auto&& forward_at(ArgsT&& ... args)
{
    return forward_at_impl<N, ArgsT...>()(std::forward<ArgsT>(args) ...);
}

template <typename T, typename PosT, typename ElemT0, typename ... ListT>
struct find_type_index : std::conditional_t<
        std::is_same_v<ElemT0, T>,
        PosT,
        find_type_index<T, std::integral_constant<typename PosT::value_type, PosT::value + 1>, ListT...>
    >
{};

template <typename T, typename PosT, typename ElemT0>
struct find_type_index<T, PosT, ElemT0> : std::conditional_t<
        std::is_same_v<ElemT0, T>,
        PosT,
        boost::mpl::identity<void>
    >
{};

template <typename T, typename ... ArgsT>
auto&& forward_type(ArgsT&& ... args)
{
    return forward_at<find_type_index<T, std::integral_constant<size_t, 0>, ArgsT...>::type::value>(args...);
}

//template <typename PredicateT, typename FtorT, typename ArgT, typename ... ArgsT> 
//auto filter_n(ArgT && arg, ArgsT&& ... args)
//{
//    if constexpr (PredicateT::template apply<ArgT&&>::type::value) {
//        filter_n
//    }
//}

template <size_t ... IndxV, typename FtorT, typename ... ArgsT>
decltype(auto) filter_by_indexes(FtorT && ftor, ArgsT&& ... args)
{
    return std::forward<FtorT>(ftor)(forward_at<IndxV>(std::forward<ArgsT>(args)...)...);
}

template <size_t ... IndxV> struct index_vector{};
template <typename PredicateT, size_t CI, typename IVT, typename ... ArgsT> struct index_filter;
template <typename PredicateT, size_t CI, size_t ... IndxV, typename ArgT, typename ... ArgsT> struct index_filter<PredicateT, CI, index_vector<IndxV ...>, ArgT, ArgsT...>
{
    using next_v_t = std::conditional_t<
        PredicateT::template apply<ArgT>::type::value,
        index_vector<IndxV ..., CI>,
        index_vector<IndxV ...>
    >;
    using type = typename std::conditional_t<
        sizeof ...(ArgsT) != 0,
        index_filter<PredicateT, CI + 1, next_v_t, ArgsT...>,
        boost::mpl::identity<next_v_t>
    >::type;
};

template <typename PredicateT, typename FtorT, size_t ... IndxV, typename ... ArgsT>
decltype(auto) filter_i(FtorT && ftor, index_vector<IndxV...>, ArgsT&& ... args)
{
    return filter_by_indexes<IndxV...>(std::forward<FtorT>(ftor), std::forward<ArgsT>(args)...);
}

template <typename PredicateT, typename FtorT, typename ... ArgsT> 
decltype(auto) filter(FtorT && ftor, ArgsT&& ... args)
{
    using iv_t = typename index_filter<PredicateT, 0, index_vector<>, ArgsT&& ...>::type;
    return filter_i<PredicateT>(std::forward<FtorT>(ftor), iv_t(), std::forward<ArgsT>(args)...);
}

template <typename PredicateT, typename FtorT, typename ... ArgsT> 
decltype(auto) reorder(FtorT && ftor, ArgsT&& ... args)
{
    return ftor(forward_at<PredicateT::template apply<ArgsT>::value>(std::forward<ArgsT>(args)...)...);
}

}

#endif // SONIA_UTILITY_VARIADIC_HPP
