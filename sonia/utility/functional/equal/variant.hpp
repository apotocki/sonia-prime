//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#ifndef SONIA_VARIANT_DECLARED
#   error do not include directly, use "sonia/variant.hpp"
#endif
#include <utility>
#include "sonia/utility/functional/equal.hpp"

namespace sonia {

template <typename ArgT>
struct variant_equal_visitor : static_visitor<bool>
{
    call_param_t<ArgT> r_;

    variant_equal_visitor(call_param_t<ArgT> r) : r_(r) {}

    template <typename T>
    bool operator()(T && val) const
    {
        return equal_f(std::forward<T>(val), r_);
    }
};

template <typename ... Ts, typename RT>
struct equal<variant<Ts...>, RT>
{
    template <typename T>
    bool operator()(variant<Ts...> const& l, T && r) const
    {
        return apply_visitor(variant_equal_visitor<RT>(std::forward<T>(r)), l);
    }
};

template <typename LT, typename ... RTs>
struct equal<LT, variant<RTs...>>
{
    template <typename T>
    bool operator()(T && l, variant<RTs...> const& r) const
    {
        return apply_visitor(variant_equal_visitor<LT>(std::forward<T>(l)), r);
    }
};

template <typename ... LTs, typename ... RTs>
struct equal<variant<LTs...>, variant<RTs...>>
{
    bool operator()(variant<LTs...> const& l, variant<RTs...> const& r) const
    {
        return apply_visitor(variant_equal_visitor<variant<LTs...>>(l), r);
    }
};

}
