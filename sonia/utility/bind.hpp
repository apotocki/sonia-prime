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

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/function.hpp"
#include "sonia/reference_wrapper.hpp"
#include "sonia/utility/rvalue.hpp"
#include "sonia/utility/variadic.hpp"

namespace sonia {

template <typename T, bool IsPlaceholder = !!is_placeholder_v<remove_cvref_t<T>>>
struct bind_parameter { typedef remove_cvref_t<T> type; };

template <typename T> struct bind_parameter<T*, false> { typedef T* type; };
template <typename T> struct bind_parameter<T&, false> { typedef reference_wrapper<T> type; };
template <typename T> struct bind_parameter<T&&, false> { typedef rvalue_wrapper<T> type; };
template <typename T> struct bind_parameter<T* &&, false> { typedef T* type; };
template <typename T> struct bind_parameter<reference_wrapper<T>&, false> { typedef reference_wrapper<T> type; };
template <typename T> struct bind_parameter<reference_wrapper<T> const&, false> { typedef reference_wrapper<T> type; };
template <typename T> struct bind_parameter<reference_wrapper<T>&&, false> { typedef reference_wrapper<T> type; };
template <typename T> struct bind_parameter<rvalue_reference_wrapper<T>&, false> { typedef rvalue_reference_wrapper<T> type; };
template <typename T> struct bind_parameter<rvalue_reference_wrapper<T> const&, false> { typedef rvalue_reference_wrapper<T> type; };
template <typename T> struct bind_parameter<rvalue_reference_wrapper<T>&&, false> { typedef rvalue_reference_wrapper<T> type; };

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

template <typename T, int pidx = is_placeholder_v<T>>
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
auto apply_placeholders_helper(std::index_sequence<I ...>, Callable && c, Tuple && tpl, Args&& ... args) {
    return std::invoke(
        std::forward<Callable>(c),
        substitute_placeholder_value<std::tuple_element_t<I, remove_cvref_t<Tuple>>>()(
            std::get<I>(std::forward<Tuple>(tpl)),
            std::forward<Args>(args) ...
        ) ...
    );
}

template <typename SigT, typename Tuple, size_t ... I,  typename ... Args>
auto apply_placeholders_helper(std::index_sequence<I ...>, function<SigT> const& c, Tuple && tpl, Args&& ... args) {
    return c(
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

// stub routine
template <typename T, typename Enabler = void>
struct stub_bound_parameter { typedef remove_cvref_t<T> type; };

template <typename T> using stub_bound_parameter_t = typename stub_bound_parameter<T>::type;

template <typename SigT> struct stub_tuple_composer;

template <typename R, typename ... ArgsT>
struct stub_tuple_composer<R(ArgsT ...)> {
    typedef std::tuple<stub_bound_parameter_t<ArgsT> ...> type;
};



template <typename SigT, SigT FuncV>
struct binding_tag_facade {
    typedef typename boost::function_types::function_type<SigT>::type f_type;
    typedef typename boost::function_types::result_type<f_type>::type result_type;
    typedef typename boost::function_types::parameter_types<f_type>::type args_type;

    //define stub tuple
    typedef typename stub_tuple_composer<f_type>::type stub_tuple_t;

    //template <typename InputIterator>
    //InputIterator
};

}

#define SONIA_DECLARE_BINDING_TAG(tagnm, func) \
struct tagnm : ::sonia::binding_tag_facade<decltype(func), func> {};

#define SONIA_REGISTER_BINDING_TAG(tag, tagnm, serv) \
::sonia::services::register_durable_id(tagnm, serv, typeid(tag))

#endif // SONIA_UTILIT_BIND_HPP
