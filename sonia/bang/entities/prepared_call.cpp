//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include "prepared_call.hpp"

#include "sonia/bang/auxiliary.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

namespace sonia::lang::bang {

prepared_call::prepared_call(pure_call_t const& call)
    : location{ call.location }
    , args{ call.args }
{
}

error_storage prepared_call::prepare(fn_compiler_context& ctx)
{
    unit& u = ctx.u();
    for (auto& arg : args) {
        if (auto const* pue = get<unary_expression_t>(&arg.value()); pue && pue->op == unary_operator_type::ELLIPSIS) {
            named_expression_t const& ellipsis_arg = pue->args.front();
            auto obj = apply_visitor(ct_expression_visitor { ctx }, ellipsis_arg.value());
            if (!obj) return std::move(obj.error());
            
            entity const& metaobject_ent = get_entity(u, *obj);

        }
    }

    return {};
}

}
