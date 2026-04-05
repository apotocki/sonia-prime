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

template <typename LessT = less_fn>
struct variant_less_visitor
{
    using is_transparent = void;
    
    template <typename T, typename ... RTs>
    inline bool operator()(T const& l, variant<RTs...> const& r) const noexcept
    {
        return std::visit([&l](auto const& rv) { return LessT{}(l, rv); }, r);
    }

    template <typename ... LTs, typename T>
    inline bool operator()(variant<LTs...> const& l, T const& r) const noexcept
    {
        return std::visit([&r](auto const& lv) { return LessT{}(lv, r); }, l);
    }

    template <typename ... LTs, typename ... RTs>
    inline bool operator()(variant<LTs...> const& l, variant<RTs...> const& r) const noexcept
    {
        return std::visit([](auto const& lv, auto const& rv) { return LessT{}(lv, rv); }, l, r);
    }
};

template <typename LessT = less>
struct variant_greater_visitor
{
    using is_transparent = void;

    template <typename T, typename ... RTs>
    inline bool operator()(T const& l, variant<RTs...> const& r) const noexcept
    {
        return std::visit([&l](auto const& rv) { return LessT{}(rv, l); }, r);
    }

    template <typename ... LTs, typename T>
    inline bool operator()(variant<LTs...> const& l, T const& r) const noexcept
    {
        return std::visit([&r](auto const& lv) { return LessT{}(r, lv); }, l);
    }

    template <typename ... LTs, typename ... RTs>
    inline bool operator()(variant<LTs...> const& l, variant<RTs...> const& r) const noexcept
    {
        return std::visit([](auto const& lv, auto const& rv) { return LessT{}(rv, lv); }, l, r);
    }
};

template <typename LT, typename RT>
requires(is_variant_v<LT> || is_variant_v<RT>)
struct less<LT, RT>
{
    inline bool operator()(LT const& l, RT const& r) const noexcept
    {
        return variant_less_visitor<>{}(l, r);
    }
};

}
