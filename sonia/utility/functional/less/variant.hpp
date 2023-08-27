//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#ifndef SONIA_VARIANT_DECLARED
#   error do not include directly, use "sonia/variant.hpp"
#endif
#include <utility>
#include "sonia/utility/functional/less.hpp"

namespace sonia {

template <typename ArgT>
struct variant_less_visitor : static_visitor<bool>
{
    call_param_t<ArgT> l_;

    variant_less_visitor(call_param_t<ArgT> l) : l_(l) {}

    template <typename T>
    bool operator()(T && val) const
    {
        return less_f(l_, std::forward<T>(val));
    }
};

template <typename ArgT>
struct variant_greater_visitor : static_visitor<bool>
{
    call_param_t<ArgT> l_;

    variant_greater_visitor(call_param_t<ArgT> l) : l_(l) {}

    template <typename T>
    bool operator()(T&& val) const
    {
        return less_f(std::forward<T>(val), l_);
    }
};

template <typename ... Ts, typename RT>
struct less<variant<Ts...>, RT>
{
    template <typename T>
    bool operator()(variant<Ts...> const& l, T && r) const
    {
        return apply_visitor(variant_greater_visitor<RT>(std::forward<T>(r)), l);
    }
};

template <typename LT, typename ... RTs>
struct less<LT, variant<RTs...>>
{
    template <typename T>
    bool operator()(T && l, variant<RTs...> const& r) const
    {
        return apply_visitor(variant_less_visitor<LT>(std::forward<T>(l)), r);
    }
};

template <typename ... LTs, typename ... RTs>
struct less<variant<LTs...>, variant<RTs...>>
{
    bool operator()(variant<LTs...> const& l, variant<RTs...> const& r) const
    {
        return apply_visitor(variant_less_visitor<variant<LTs...>>(l), r);
    }
};

}
