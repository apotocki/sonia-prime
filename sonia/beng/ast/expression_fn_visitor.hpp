//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"
#include "sonia/optional.hpp"
#include "fn_compiler_context.hpp"

#include "../semantic.hpp"
#include "../entities/functional_entity.hpp"

namespace sonia::lang::beng {

struct expression_fn_visitor : static_visitor<optional<beng_type>>
{
    fn_compiler_context& ctx;
    functional_entity const& fn;
    std::vector<semantic_expression_type>& result;

    expression_fn_visitor(fn_compiler_context& c, functional_entity const& f, std::vector<semantic_expression_type>& r)
        : ctx{ c }
        , fn{ f }
        , result{ r }
    {}

    inline result_type operator()(beng_bool_t const& v) const { return nullopt; }
    inline result_type operator()(beng_string_t const& v) const { return nullopt; }
    inline result_type operator()(beng_decimal_t const&) const { return nullopt; }
    inline result_type operator()(beng_int_t const&) const { return nullopt; }
    inline result_type operator()(beng_float_t const&) const { return nullopt; }
    inline result_type operator()(beng_object_t const& v) const { return nullopt; } // to do: from_fn?

    result_type operator()(beng_union_t const& v) const
    {
        for (beng_type const& ut : v.members) {
            if (auto optrest = apply_visitor(*this, ut); optrest) {
                return *optrest;
            }
        }
        return nullopt;
    }

    inline result_type operator()(beng_fn_t const& v) const
    {
        span<const beng_type> position_params = v.arg.fields;
        span<const std::tuple<annotated_identifier, beng_type>> named_params = v.arg.named_fields;
        
        function_signature const* fs = fn.find(ctx, position_params, named_params);
        if (!fs || fs->fn_type.result != v.result) {
            return nullopt;
        }
        qname fnm = fn.name() + fs->mangled_id;
        variable_entity const* pv = ctx.resolve_variable(fnm);
        if (pv) {
            result.emplace_back(semantic::push_variable{ pv });
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
        THROW_NOT_IMPLEMENTED_ERROR();
    }
    //*/
};

}

