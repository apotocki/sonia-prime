//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <sstream>
#include <expected>

//#include "sonia/utility/scope_exit.hpp"

#include "../semantic.hpp"
//#include "expression_visitor.hpp"
#include "casts/expression_cast_to_bool_visitor.hpp"
#include "casts/expression_cast_to_float_visitor.hpp"

#include "casts/expression_cast_to_array_visitor.hpp"
#include "casts/expression_cast_to_vector_visitor.hpp"
#include "casts/expression_cast_to_object_visitor.hpp"
#include "casts/expression_cast_to_string_visitor.hpp"

#include "fn_compiler_context.hpp"

#include "sonia/beng/errors.hpp"

namespace sonia::lang::beng {

struct expression_implicit_cast_visitor : static_visitor<std::expected<beng_type, error_storage>>
{
    fn_compiler_context& ctx;
    beng_type const& type2cast;
    context_locator_t cl_;
    
    expression_implicit_cast_visitor(fn_compiler_context& c, beng_type const& t, context_locator_t const& cl)
        : ctx{ c }
        , type2cast{ t }
        , cl_{ cl }
    {}

    // return std::unexpected(ctx.error_cannot_convert(var.location, ctx.u().print(var.name), varptr->type(), *expected_result));

    /*
    result_type operator()(beng_vector_t const& v) const
    {
        size_t stored_pos = result.size();
        EXCEPTIONAL_SCOPE_EXIT([this, stored_pos](){ result.resize(stored_pos); });
        expression_visitor elemvis{ ctx, result, &v.type };
        for (expression_t const& e : vec.elements) {
            if (!apply_visitor(elemvis, e)) {
                result.resize(stored_pos);
                return nullopt;
            }
        }
        result.emplace_back(semantic::push_value{ decimal{ vec.elements.size() } });
        result.emplace_back(semantic::invoke_function{ ctx.u().arrayify_entity_name(), (uint32_t)vec.elements.size() + 1 });
        return v;
    }

    result_type operator()(beng_union_t const& v) const
    {
        for (beng_generic_type const& ut : v.members) {
            if (auto optrest = apply_visitor(*this, ut); optrest) {
                return *optrest;
            }
        }
        return nullopt;
    }

    inline result_type operator()(beng_bool_t const& v) const { return nullopt; }
    inline result_type operator()(beng_int_t const& v) const { return nullopt; }
    inline result_type operator()(beng_float_t const& v) const { return nullopt; }
    inline result_type operator()(beng_decimal_t const& v) const { return nullopt; }
    inline result_type operator()(beng_string_t const& v) const { return nullopt; }
    inline result_type operator()(beng_object_t const& v) const { return nullopt; }
    */

    inline result_type operator()(beng_float_t const&) const
    {
        return apply_visitor(expression_cast_to_float_visitor{ ctx, cl_ }, type2cast);
    }

    inline result_type operator()(beng_object_t const& v) const
    {
        return apply_visitor(expression_cast_to_object_visitor{ ctx, v, cl_ }, type2cast);
    }

    inline result_type operator()(beng_vector_t const& v) const
    {
        return apply_visitor(expression_cast_to_vector_visitor{ ctx, v, cl_ }, type2cast);
    }

    inline result_type operator()(beng_array_t const& ar) const
    {
        return apply_visitor(expression_cast_to_array_visitor{ ctx, ar, cl_ }, type2cast);
    }

    inline result_type operator()(beng_string_t const& v) const
    {
        ctx.append_expression(ctx.u().get_builtin_function(unit::builtin_fn::tostring));
        return beng_string_t{};
        //expression_cast_to_string_visitor vis{ ctx, result };
        //return apply_visitor(vis, type2cast);
    }

    inline result_type operator()(beng_union_t const& v) const
    {
        alt_error aerr;
        for (beng_type const& t : v) {
            auto opt = apply_visitor(*this, t);
            if (opt.has_value()) { return opt; }
            aerr.alternatives.emplace_back(std::move(opt.error()));
        }
        return std::unexpected(make_error<alt_error>(std::move(aerr)));
    }

    inline result_type operator()(beng_bool_t const&) const
    {
        return apply_visitor(expression_cast_to_bool_visitor{ ctx, cl_ }, type2cast);
    }

    inline result_type operator()(beng_any_t const&) const
    {
        return type2cast;
    }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

}
