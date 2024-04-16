//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"
#include "sonia/optional.hpp"
#include "../semantic.hpp"
#include "fn_compiler_context.hpp"

namespace sonia::lang::bang {

struct expression_decimal_visitor : static_visitor<optional<bang_type>>
{
    fn_compiler_context& ctx;
    decimal const& dec;
    std::vector<semantic::expression_type>& result;

    expression_decimal_visitor(fn_compiler_context& c, decimal const& d, std::vector<semantic::expression_type>& r)
        : ctx{ c }
        , dec{ d }
        , result{ r }
    {}

    inline result_type operator()(bang_decimal_t const&) const
    {
        result.emplace_back(semantic::push_value{ dec });
        return bang_decimal_t{};
    }

    inline result_type operator()(bang_int_t const&) const
    {
        result.emplace_back(semantic::push_value{ dec });
        return bang_int_t{};
    }

    inline result_type operator()(bang_float_t const&) const
    {
        result.emplace_back(semantic::push_value{ dec });
        return bang_float_t{};
    }

    result_type operator()(bang_union_t const& v) const
    {
        for (bang_type const& ut : v) {
            if (auto optrest = apply_visitor(*this, ut); optrest) {
                return *optrest;
            }
        }
        return nullopt;
    }

    inline result_type operator()(bang_bool_t const& v) const { return nullopt; }
    inline result_type operator()(bang_string_t const& v) const { return nullopt; }
    inline result_type operator()(bang_preliminary_object_t const& v) const { return nullopt; } // to do: from_decimal?
    /*
    inline result_type operator()(bang_preliminary_array_t const& v) const { return nullopt; }
    inline result_type operator()(bang_preliminary_vector_t const& v) const { return nullopt; }
    inline result_type operator()(bang_preliminary_tuple_t const& v) const { return nullopt; }
    inline result_type operator()(bang_preliminary_fn_t const& v) const { return nullopt; }
    */
    ///*
    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
    //*/
};

}

