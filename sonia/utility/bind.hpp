//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILIT_BIND_HPP
#define SONIA_UTILIT_BIND_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <functional>
#include <tuple>

#include "sonia/type_traits.hpp"

namespace sonia {

template <typename T, bool IsPlaceholder = !!std::is_placeholder_v<remove_cvref_t<T>>>
struct bind_parameter { typedef remove_cvref_t<T> type; };

template <typename T> struct bind_parameter<T*, false> { typedef T* type; };
template <typename T> struct bind_parameter<T&, false> { typedef reference_wrapper<T> type; };
template <typename T> struct bind_parameter<T&&, false> { typedef rvalue_reference_wrapper<T> type; };
template <typename T> struct bind_parameter<T* &&, false> { typedef T* type; };
template <typename T> struct bind_parameter<reference_wrapper<T>&, false> { typedef reference_wrapper<T> type; };
template <typename T> struct bind_parameter<reference_wrapper<T> const&, false> { typedef reference_wrapper<T> type; };
template <typename T> struct bind_parameter<reference_wrapper<T>&&, false> { typedef reference_wrapper<T> type; };

template <typename T> using bind_parameter_t = typename bind_parameter<T>::type;

template <typename ... ArgsT>
struct bind_tuple {
    typedef std::tuple<bind_parameter_t<ArgsT&&> ...> type;
};

template <typename ... ArgsT> using bind_tuple_t = typename bind_tuple<ArgsT...>::type;

template <typename ... ArgsT>
bind_tuple_t<ArgsT...> make_bind_tuple(ArgsT&& ... args) {
    return bind_tuple_t<ArgsT...>(std::forward<ArgsT>(args)...);
}

template <typename T, int pidx = std::is_placeholder_v<T>>
struct substitute_placeholder_value {
    template <typename Arg, typename ... Args>
    auto&& operator()(Arg &&, Args&& ... args)  {
        return variadic::forward_at<pidx - 1>(std::forward<Args>(args) ...);
    }
};

template <typename T>
struct substitute_placeholder_value<T, 0> {
    template <typename Arg, typename ... Args>
    auto operator()(Arg && arg, Args&& ...)  {
        return std::forward<Arg>(arg);
    }
};

template <typename Callable, typename Tuple, size_t ... I,  typename ... Args>
auto apply_placeholders_helper(std::index_sequence<I...>, Callable && c, Tuple && tpl, Args&& ... args) {
    return std::invoke(
        std::forward<Callable>(c),
        substitute_placeholder_value<std::tuple_element_t<I, remove_cvref_t<Tuple>>>()(
            std::get<I>(std::forward<Tuple>(tpl)),
            std::forward<Args>(args) ...
        ) ...
    );
}

template <typename Callable, typename Tuple, typename ... Args>
auto apply_placeholders(Callable && c, Tuple && tpl, Args&& ... args) {
    return apply_placeholders_helper(
        std::make_index_sequence<std::tuple_size_v<remove_cvref_t<Tuple>>>(),
        std::forward<Callable>(c), std::forward<Tuple>(tpl),
        std::forward<Args>(args) ...
    );
}

}

#endif // SONIA_UTILIT_BIND_HPP
