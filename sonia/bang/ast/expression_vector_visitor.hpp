//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"
#include "sonia/optional.hpp"
#include "sonia/utility/scope_exit.hpp"

#include "../semantic.hpp"
#include "expression_visitor.hpp"
#include "fn_compiler_context.hpp"

namespace sonia::lang::bang {

struct expression_vector_visitor_obs : static_visitor<optional<bang_type>>
{
    fn_compiler_context& ctx;
    expression_vector_t & vec;
    lex::resource_location const& loc_;

    expression_vector_visitor_obs(fn_compiler_context& c, expression_vector_t & v, lex::resource_location const& loc)
        : ctx{ c }
        , vec{ v }
        , loc_{ loc }
    {}

    result_type operator()(bang_vector_t const& v) const
    {
        auto estate = ctx.expressions_state();
        expression_visitor elemvis{ ctx, expected_result_t{ v.type, loc_ } };
        for (expression_t & e : vec.elements) {
            if (!apply_visitor(elemvis, e)) {
                return nullopt;
            }
        }
        ctx.append_expression(semantic::push_value{ decimal{ vec.elements.size() } });
        ctx.append_expression(ctx.u().get_builtin_function(unit::builtin_fn::arrayify));
        estate.detach();
        return v;
    }

    result_type operator()(bang_array_t const& ar) const
    {
        if (ar.size != vec.elements.size() || !this->operator()(bang_vector_t{ ar.type })) return nullopt;
        return ar;
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
    inline result_type operator()(bang_int_t const& v) const { return nullopt; }
    inline result_type operator()(bang_float_t const& v) const { return nullopt; }
    inline result_type operator()(bang_decimal_t const& v) const { return nullopt; }
    inline result_type operator()(bang_string_t const& v) const { return nullopt; }
    inline result_type operator()(bang_object_t const& v) const { return nullopt; }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

}
