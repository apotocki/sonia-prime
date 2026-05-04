//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/size.hpp>

#include <functional>
#include <tuple>
#include <type_traits>

#include "sonia/shared_ptr.hpp"
#include "sonia/utility/invocation/invocation.hpp"

namespace sonia::invocation {

namespace detail {

template <typename F, bool IsClassV = std::is_class_v<F>>
struct callable_boost_traits_impl;

template <typename F>
struct callable_boost_traits_impl<F, false>
{
    using sig_t = F;
    using f_type = typename boost::function_types::function_type<sig_t>::type;
    using result_type = typename boost::function_types::result_type<f_type>::type;
    using args_type = typename boost::function_types::parameter_types<f_type>::type;

    static constexpr size_t arg_count = boost::mpl::size<args_type>::type::value;

    template <size_t I>
    using arg_t = typename boost::mpl::at_c<args_type, I>::type;
};

template <typename F>
struct callable_boost_traits_impl<F, true>
{
    using sig_t = decltype(&F::operator());
    using f_type = typename boost::function_types::function_type<sig_t>::type;
    using result_type = typename boost::function_types::result_type<f_type>::type;
    using args_type = typename boost::function_types::parameter_types<f_type>::type;
    using pure_args_type = typename boost::mpl::pop_front<args_type>::type;

    static constexpr size_t arg_count = boost::mpl::size<pure_args_type>::type::value;

    template <size_t I>
    using arg_t = typename boost::mpl::at_c<pure_args_type, I>::type;
};

template <typename F>
using callable_boost_traits = callable_boost_traits_impl<std::remove_cvref_t<F>>;

template <typename ArgT, typename StoredArgT>
decltype(auto) forward_invocation_arg(StoredArgT& value)
{
    if constexpr (std::is_lvalue_reference_v<ArgT>) {
        return static_cast<ArgT>(value);
    } else {
        return std::move(value);
    }
}

template <typename TraitsT, typename F, size_t... Is>
smart_blob invoke_functor(F& fn, span<const blob_result> args, std::index_sequence<Is...>)
{
    if (args.size() != sizeof...(Is)) {
        THROW_INTERNAL_ERROR("invalid number of invocation arguments");
    }

    using converted_tuple_t = std::tuple<std::remove_cvref_t<typename TraitsT::template arg_t<Is>>...>;
    converted_tuple_t converted_args{
        from_blob<std::remove_cvref_t<typename TraitsT::template arg_t<Is>>>{}(args[Is])...
    };

    using result_t = typename TraitsT::result_type;
    if constexpr (std::is_void_v<result_t>) {
        std::invoke(fn, forward_invocation_arg<typename TraitsT::template arg_t<Is>>(std::get<Is>(converted_args))...);
        return {};
    } else {
        return smart_blob{ particular_blob_result(std::invoke(
            fn,
            forward_invocation_arg<typename TraitsT::template arg_t<Is>>(std::get<Is>(converted_args))...
        )) };
    }
}

}

// Wraps any callable object (functor, lambda, std::function) as an invocation::callable.
// Callable argument and result types are deduced from F.
template <typename F>
class functor_callable : public callable
{
public:
    explicit functor_callable(F fn) : fn_{ std::move(fn) } {}

    smart_blob invoke(span<const blob_result> args) override
    {
        using traits_t = detail::callable_boost_traits<F>;
        return detail::invoke_functor<traits_t>(fn_, args, std::make_index_sequence<traits_t::arg_count>{});
    }

private:
    F fn_;
};

template <typename F>
shared_ptr<callable> make_callable(F&& fn)
{
    return make_shared<functor_callable<std::decay_t<F>>>(std::forward<F>(fn));
}

}
