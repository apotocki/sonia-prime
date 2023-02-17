//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <functional>
#include <tuple>

#include "sonia/type_traits.hpp"
#include "sonia/function.hpp"
#include "sonia/utility/rvalue.hpp"
#include "sonia/utility/variadic.hpp"

namespace sonia {

template <typename T, bool IsPlaceholder = !!is_placeholder_v<remove_cvref_t<T>>>
struct bound_parameter { using type = remove_cvref_t<T>; };

template <typename T> struct bound_parameter<T*, false> { using type = T*; };
template <typename T> struct bound_parameter<T&, false> { using type = reference_wrapper<T>; };
template <typename T> struct bound_parameter<T&&, false> { using type = rvalue_wrapper<T>; };
template <typename T> struct bound_parameter<T* &&, false> { using type = T * ; };
template <typename T> struct bound_parameter<reference_wrapper<T>&, false> { using type = reference_wrapper<T>; };
template <typename T> struct bound_parameter<reference_wrapper<T> const&, false> { using type = reference_wrapper<T>; };
template <typename T> struct bound_parameter<reference_wrapper<T>&&, false> { using type = reference_wrapper<T>; };
template <typename T> struct bound_parameter<rvalue_reference_wrapper<T>&, false> { using type = rvalue_reference_wrapper<T>; };
template <typename T> struct bound_parameter<rvalue_reference_wrapper<T> const&, false> { using type = rvalue_reference_wrapper<T>; };
template <typename T> struct bound_parameter<rvalue_reference_wrapper<T>&&, false> { using type = rvalue_reference_wrapper<T>; };

template <typename T> using bound_parameter_t = typename bound_parameter<T>::type;

template <typename ... ArgsT> using bind_tuple_t = std::tuple<bound_parameter_t<ArgsT&&> ...>;

template <typename ... ArgsT>
bind_tuple_t<ArgsT...> make_bind_tuple(ArgsT&& ... args)
{
    return bind_tuple_t<ArgsT...>(std::forward<ArgsT>(args)...);
}

template <typename T, int pidx = is_placeholder_v<T>>
struct substitute_placeholder_value
{
    template <typename Arg, typename ... Args>
    decltype(auto) operator()(Arg &&, Args&& ... args) const
    {
        return variadic::forward_at<pidx - 1>(std::forward<Args>(args) ...);
    }
};

template <typename T>
struct substitute_placeholder_value<T, 0>
{
    template <typename Arg, typename ... Args>
    decltype(auto) operator()(Arg && arg, Args&& ...) const
    {
        return std::forward<Arg>(arg);
    }
};

template <typename CallableT, class TupleT, size_t ... I,  typename ... Args>
auto apply_placeholders_helper(std::index_sequence<I ...>, CallableT && c, TupleT && tpl, Args&& ... args)
{
    return std::invoke(
        std::forward<CallableT>(c),
        substitute_placeholder_value<std::tuple_element_t<I, remove_cvref_t<TupleT>>>()(
            std::get<I>(std::forward<TupleT>(tpl)),
            std::forward<Args>(args) ...
        ) ...
    );
}

template <typename SigT, class TupleT, size_t ... I,  typename ... Args>
auto apply_placeholders_helper(std::index_sequence<I ...>, function<SigT> const& c, TupleT && tpl, Args&& ... args)
{
    return c(
        substitute_placeholder_value<std::tuple_element_t<I, remove_cvref_t<TupleT>>>()(
            std::get<I>(std::forward<TupleT>(tpl)),
            std::forward<Args>(args) ...
        ) ...
    );
}

template <typename CallableT, typename TupleT, typename ... Args>
auto apply_placeholders(CallableT && c, TupleT && tpl, Args&& ... args)
{
    return apply_placeholders_helper(
        std::make_index_sequence<std::tuple_size_v<remove_cvref_t<TupleT>>>(),
        std::forward<CallableT>(c), std::forward<TupleT>(tpl),
        std::forward<Args>(args) ...
    );
}

}
