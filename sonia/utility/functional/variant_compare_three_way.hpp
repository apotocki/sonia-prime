//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"

namespace sonia {

struct variant_compare_three_way_visitor : static_visitor<std::strong_ordering>
{
    variant_compare_three_way_visitor() = default;
    template <typename L, typename R>
    result_type operator()(L const&, R const&) const { std::terminate(); }

    template <typename T>
    result_type operator()(T const& l, T const& r) const { return l <=> r; }
};

struct variant_compare_three_way
{
    template <typename VariantT>
    auto operator()(VariantT const& l, VariantT const& r)
    {
        if (l.which() != r.which()) return l.which() <=> r.which();
        return apply_visitor(variant_compare_three_way_visitor{}, l, r);
    }
};

}
