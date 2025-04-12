//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_empty_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/expression_visitor.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> tuple_empty_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& exp) const
{
    unit& u = ctx.u();
    auto opt_arg_expr = try_match_single_unnamed(ctx, call);
    if (!opt_arg_expr) return std::unexpected(std::move(opt_arg_expr.error()));
    auto estate = ctx.expressions_state();

    expression_visitor eobjvis{ ctx };
    auto obj = apply_visitor(eobjvis, **opt_arg_expr);
    if (!obj) return std::unexpected(std::move(obj.error()));
    if (ctx.context_type) {
        entity const& tpl_entity = u.eregistry_get(ctx.context_type);
        if (auto psig = tpl_entity.signature(); psig && psig->name == u.get(builtin_qnid::tuple)) {
            auto pmd = make_shared<functional_match_descriptor>(u);
            pmd->result = field_descriptor{ u.get(tpl_entity.signature()->fields().empty() ? builtin_eid::true_ : builtin_eid::false_), true };
            pmd->location = call.location;
            return pmd;

            /*
            integer_literal_entity int_ent{ psig->field_count() };
            entity_identifier entid = u.eregistry_find_or_create(int_ent, [&u, &int_ent]() {
                auto result = make_shared<integer_literal_entity>(std::move(int_ent));
                result->set_type(u.get(builtin_eid::integer));
                return result;
            }).id();
            */
        }
    }
    return std::unexpected(make_error<type_mismatch_error>(get_start_location(**opt_arg_expr), ctx.context_type, u.get(builtin_qnid::tuple)));
}

std::expected<syntax_expression_result_t, error_storage> tuple_empty_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    return make_result(ctx.u(), md.result.entity_id());
}

}
