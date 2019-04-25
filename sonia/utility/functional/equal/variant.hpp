//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_EQUAL_VARIANT_HPP
#define SONIA_FUNCTIONAL_EQUAL_VARIANT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/variant.hpp>

#include "sonia/utility/functional/equal.hpp"

namespace sonia {

template <typename ArgT>
struct variant_equal_visitor : boost::static_visitor<bool>
{
    call_param_t<ArgT> r_;

    variant_equal_visitor(call_param_t<ArgT> r) : r_(r) {}

    template <typename T>
    size_t operator()(T && val) const
    {
        return equal_f(std::forward<T>(val), r_);
    }
};

template <typename ... Ts, typename RT>
struct equal<boost::variant<Ts...>, RT>
{
    template <typename T>
    size_t operator()(boost::variant<Ts...> const& l, T && r) const
    {
        return boost::apply_visitor(variant_hash_visitor<RT>(std::forward<RT>(r)), l);
    }
};

template <typename LT, typename ... Ts>
struct equal<LT, boost::variant<Ts...>>
{
    template <typename T>
    size_t operator()(T && l, boost::variant<Ts...> const& r) const
    {
        return boost::apply_visitor(variant_hash_visitor<LT>(std::forward<RT>(r)), r);
    }
};

template <typename ... LTs, typename ... RTs>
struct equal<boost::variant<LTs...>, boost::variant<RTs...>>
{
    size_t operator()(boost::variant<LTs...> const& l, boost::variant<RTs...> const& r) const
    {
        return boost::apply_visitor(variant_hash_visitor<boost::variant<LTs...>>(l), r);
    }
};

}

#endif // SONIA_FUNCTIONAL_EQUAL_VARIANT_HPP
