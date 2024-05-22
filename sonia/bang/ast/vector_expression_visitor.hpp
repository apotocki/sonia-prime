//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/errors.hpp"
#include "sonia/bang/semantic.hpp"

#include "fn_compiler_context.hpp"

namespace sonia::lang::bang {

struct vector_expression_visitor : static_visitor<error_storage>
{
    fn_compiler_context& ctx;
    expression_vector_t& vec;
    lex::resource_location const& type_decl_location;

    vector_expression_visitor(fn_compiler_context& c, expression_vector_t& v, lex::resource_location const& loc)
        : ctx{c}, vec{v}, type_decl_location{ loc }
    {}

    result_type operator()(bang_vector_t const& v) const
    {
        expression_visitor elemvis{ ctx, expected_result_t{v.type, type_decl_location} };
        for (expression_t& e : vec.elements) {
            if (auto opterr = apply_visitor(elemvis, e); opterr) return std::move(opterr);
        }
        ctx.append_expression(semantic::push_value{ mp::integer{ vec.elements.size() } });
        ctx.append_expression(ctx.u().get_builtin_function(unit::builtin_fn::arrayify));
        ctx.context_type = v;
        return {};
    }

    result_type operator()(bang_array_t const& v) const
    {
        if (v.size != vec.elements.size()) {
            return make_error<cast_error>(type_decl_location, v, nullopt, vec);
        }
        expression_visitor elemvis{ ctx, expected_result_t{v.type, type_decl_location} };
        for (expression_t& e : vec.elements) {
            if (auto opterr = apply_visitor(elemvis, e); opterr) return std::move(opterr);
        }
        ctx.append_expression(semantic::push_value{ mp::integer{ vec.elements.size() } });
        ctx.append_expression(ctx.u().get_builtin_function(unit::builtin_fn::arrayify));
        ctx.context_type = v;
        return {};
    }

    result_type operator()(bang_union_t const& v) const
    {
        alt_error aerr;
        for (bang_type const& t : v) {
            if (auto opt = apply_visitor(*this, t); opt) {
                aerr.alternatives.emplace_back(std::move(opt));
            } else {
                return {};
            }
        }
        return make_error<alt_error>(std::move(aerr));
    }

    inline result_type operator()(bang_float_t const& v) const
    {
        return make_error<cast_error>(type_decl_location, v, nullopt, vec);
    }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

}