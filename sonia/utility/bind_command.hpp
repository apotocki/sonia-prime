//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_BIND_COMMAND_HPP
#define SONIA_BIND_COMMAND_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/mpl/vector.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/integral_c.hpp>

#include <boost/mpl/max_element.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/insert.hpp>

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/assert.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/function.hpp"
#include "sonia/utility/serialization/serialization_fwd.hpp"

#include "command.hpp"
#include "bind.hpp"

namespace sonia {

template <class BindTupleT, typename R>
class bind_command_proxy : public command_base<R> {
    SONIA_DECLARE_SERIALIZATION_FRIENDLY;

public:
    template <typename ... BindArgsT>
    explicit bind_command_proxy(BindArgsT && ... args)
        : btpl_{std::forward<BindArgsT>(args)...}
    {

    }

    std::type_index ti() const override {
        return typeid(bind_command_proxy);
    }

protected:
    BindTupleT btpl_;
};

template <class CallableT, class BindTupleT, typename R, typename ... ArgsT>
class bind_command : public command<R, ArgsT ...> {
public:
    template <typename CallableArgT, typename ... BindArgsT>
    explicit bind_command(CallableArgT && c, BindArgsT && ... args)
        : callable_{std::forward<CallableArgT>(c)}, btpl_{std::forward<BindArgsT>(args)...}
    {
    
    }

    R operator()(ArgsT ... args) const override {
        return apply_placeholders(callable_, btpl_, std::forward<ArgsT>(args) ...);
    }

    std::type_index ti() const override {
        return typeid(bind_command);
    }

private:
    CallableT callable_;
    BindTupleT btpl_;
};

template <class CallableT, class BindTupleT, typename FuncSigT> struct bind_command_composer;

template <class CallableT, class BindTupleT, typename RT, typename ... ArgsT> struct bind_command_composer<CallableT, BindTupleT, RT(ArgsT...)> {
    typedef bind_command<CallableT, BindTupleT, RT, ArgsT ...> type;
    typedef bind_command_proxy<BindTupleT, RT> proxy_type;
};

namespace bind_command_detail {

template <typename ... sT> struct tvector {};

template <
    typename FArgsT,
    size_t I,
    typename BArgsT,
    typename RT = boost::mpl::map1<boost::mpl::pair<boost::mpl::integral_c<int, 0>, void>>
> struct placeholders_retriever;

template <typename FArgsT, size_t I, typename RT> struct placeholders_retriever<FArgsT, I, tvector<>, RT>
{
    // find max
    static const int max_arg = boost::mpl::first<
        typename boost::mpl::deref<
        typename boost::mpl::max_element<
        RT, boost::mpl::less<boost::mpl::first<boost::mpl::_1>, boost::mpl::first<boost::mpl::_2>>
        >::type
        >::type
    >::type::value;

    // sort out the placeholder types
    typedef typename boost::mpl::fold<
        boost::mpl::range_c<int, 0, max_arg + 1>,
        boost::mpl::vector0<>,
        boost::mpl::push_back<boost::mpl::_1, boost::mpl::at<RT, boost::mpl::_2>>
    >::type type;
};

template <typename FArgsT, size_t I, typename BArg0T, typename ... BArgsT, typename RT>
struct placeholders_retriever<FArgsT, I, tvector<BArg0T, BArgsT ...>, RT>
    : placeholders_retriever<FArgsT, I + 1, tvector<BArgsT...>, typename conditional<
        !!is_placeholder_v<BArg0T>,
        typename boost::mpl::insert<RT, boost::mpl::pair<boost::mpl::integral_c<int, is_placeholder_v<BArg0T>>, typename boost::mpl::at_c<FArgsT, I>::type>>::type,
        RT
    >::type>
{};

template <typename FT, typename ... ArgsT>
struct bind_command_helper {
    typedef typename boost::function_types::function_type<FT>::type f_type;
    typedef typename boost::function_types::result_type<f_type>::type result_type;

    typedef typename boost::function_types::parameter_types<f_type>::type args_type;
    static const size_t func_arity = boost::function_types::function_arity<FT>::value;

    BOOST_MPL_ASSERT_RELATION(sizeof ...(ArgsT), == , func_arity);

    typedef typename placeholders_retriever<
        args_type, 0, tvector<remove_cvref_t<ArgsT> ...>,
        boost::mpl::map1<boost::mpl::pair<boost::mpl::integral_c<int, 0>, result_type>> // attach command result type as a first element
    >::type placeholders_vec_t;

    typedef typename boost::function_types::function_type<placeholders_vec_t>::type command_sig_t;
    
    typedef bind_command_composer<function<f_type>, bind_tuple_t<ArgsT ...>, command_sig_t> composer_t;
    typedef typename composer_t::type command_type;
    typedef typename composer_t::proxy_type command_proxy_type;
};

} // namespace bind_command_detail

template <typename FT, typename ... ArgsT>
typename bind_command_detail::bind_command_helper<FT, ArgsT ...>::command_proxy_type make_bind_command_proxy(FT const&, ArgsT&& ... args) {
    typedef bind_command_detail::bind_command_helper<FT, ArgsT ...> helper_t;
    return typename helper_t::command_proxy_type{std::forward<ArgsT>(args)...};
}

template <typename FT, typename ... ArgsT>
typename bind_command_detail::bind_command_helper<FT, ArgsT ...>::command_type make_bind_command(FT const& f, ArgsT&& ... args) {
    typedef bind_command_detail::bind_command_helper<FT, ArgsT ...> helper_t;

    //std::cout << typeid(typename helper_t::command_type).name() << "\n" << helper_t::func_arity << "\n";

    return typename helper_t::command_type{f, std::forward<ArgsT>(args)...};
}

}

#endif // SONIA_BIND_COMMAND_HPP
