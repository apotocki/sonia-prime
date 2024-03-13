//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"
#include "sonia/optional.hpp"
#include "terms.hpp"
#include "../semantic.hpp"
#include "compiler_context.hpp"

namespace sonia::lang::beng {

struct expression_decimal_visitor : static_visitor<optional<beng_type>>
{
    compiler_context& ctx;
    decimal const& dec;
    std::vector<semantic_expression_type>& result;

    expression_decimal_visitor(compiler_context& c, decimal const& d, std::vector<semantic_expression_type>& r)
        : ctx{ c }
        , dec{ d }
        , result{ r }
    {}

    inline result_type operator()(beng_decimal_t const&) const
    {
        result.emplace_back(semantic::push_value{ dec });
        return beng_decimal_t{};
    }

    inline result_type operator()(beng_int_t const&) const
    {
        result.emplace_back(semantic::push_value{ dec });
        return beng_int_t{};
    }

    inline result_type operator()(beng_float_t const&) const
    {
        result.emplace_back(semantic::push_value{ dec });
        return beng_float_t{};
    }

    result_type operator()(beng_preliminary_union_t const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
        /*
        for (beng_type const& ut : v.members) {
            if (auto optrest = apply_visitor(*this, ut); optrest) {
                return *optrest;
            }
        }
        return nullopt;
        */
    }

    inline result_type operator()(beng_bool_t const& v) const { return nullopt; }
    inline result_type operator()(beng_string_t const& v) const { return nullopt; }
    inline result_type operator()(beng_preliminary_object_t const& v) const { return nullopt; } // to do: from_decimal?
    /*
    inline result_type operator()(beng_preliminary_array_t const& v) const { return nullopt; }
    inline result_type operator()(beng_preliminary_vector_t const& v) const { return nullopt; }
    inline result_type operator()(beng_preliminary_tuple_t const& v) const { return nullopt; }
    inline result_type operator()(beng_preliminary_fn_t const& v) const { return nullopt; }
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

