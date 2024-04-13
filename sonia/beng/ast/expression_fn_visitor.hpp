//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"
#include "sonia/optional.hpp"
#include "fn_compiler_context.hpp"

#include "../semantic.hpp"
#include "../entities/functional_entity.hpp"
#include "sonia/beng/errors.hpp"

namespace sonia::lang::beng {

struct expression_fn_visitor : static_visitor<std::expected<beng_type, error_storage>>
{
    fn_compiler_context& ctx;
    functional_entity const& fn;
    context_locator_t const& cl_;

    expression_fn_visitor(fn_compiler_context& c, functional_entity const& f, context_locator_t const& cl)
        : ctx{ c }
        , fn{ f }
        , cl_{ cl }
    {}

    //inline result_type operator()(beng_bool_t const& v) const
    //{
    //    return std::unexpected(ctx.error_cannot_convert(el_, v));
    //}

    //inline result_type operator()(beng_string_t const& v) const
    //{
    //    return std::unexpected(ctx.error_cannot_convert(el_, v));
    //}

    //inline result_type operator()(beng_decimal_t const& v) const
    //{
    //    return std::unexpected(ctx.error_cannot_convert(el_, v));
    //}

    //inline result_type operator()(beng_int_t const& v) const
    //{
    //    return std::unexpected(ctx.error_cannot_convert(el_, v));
    //}

    //inline result_type operator()(beng_float_t const& v) const
    //{
    //    return std::unexpected(ctx.error_cannot_convert(el_, v));
    //}

    //inline result_type operator()(beng_object_t const& v) const
    //{
    //    // to do: from_fn?
    //    return std::unexpected(ctx.error_cannot_convert(el_, v));
    //}
    
    result_type operator()(beng_union_t const& v) const
    {
        alt_error aerr;
        for (beng_type const& ut : v) {
            auto opt = apply_visitor(*this, ut);
            if (opt.has_value()) { return opt; }
            aerr.alternatives.emplace_back(std::move(opt.error()));
        }
        return std::unexpected(make_error<alt_error>(std::move(aerr)));
    }

    inline result_type operator()(beng_fn_t const& v) const
    {
        span<const beng_type> position_params = v.arg.fields;
        span<const std::tuple<annotated_identifier, beng_type>> named_params = v.arg.named_fields;
        
        function_signature const* fs = fn.find(ctx, position_params, named_params);
        if (!fs || fs->fn_type.result != v.result) {
            return std::unexpected(make_error<cast_error>(cl_(), v, nullopt));
        }
        qname fnm = fn.name() + fs->mangled_id;
        variable_entity const* pv = ctx.resolve_variable(fnm);
        if (pv) {
            ctx.append_expression(semantic::push_variable{ pv });
        } else {
            THROW_INTERNAL_ERROR("function '%1%' is not materialized"_fmt % ctx.u().print(fnm));
            //result.emplace_back(semantic::push_value{ function_value { std::move(fnm) } });
        }
        return v;
    }

    ///*
    template <typename T>
    result_type operator()(T const& v) const
    {
        return std::unexpected(make_error<cast_error>(cl_(), v, nullopt));
    }
    //*/
};

}
