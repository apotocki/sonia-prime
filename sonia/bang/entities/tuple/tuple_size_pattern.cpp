//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_size_pattern.hpp"

#include "sonia/bang/functional/generic_pattern_base.ipp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {
#if 0
error_storage size_pattern::accept_argument(std::nullptr_t, functional_match_descriptor_ptr& pmd, arg_context_type& argctx, syntax_expression_result_t& v) const
{
    if (!v.is_const_result || pmd || argctx.pargname)
        return argctx.make_argument_mismatch_error();

    fn_compiler_context& ctx = argctx.ctx;
    prepared_call const& call = argctx.call;
    unit& u = ctx.u();

    signatured_entity const* arg_entity = dynamic_cast<signatured_entity const*>(&get_entity(u, v.value()));
    if (!arg_entity) {
        return argctx.make_argument_mismatch_error();
    }
    pmd = make_shared<functional_match_descriptor>(call.location);
    pmd->result = field_descriptor{ u.make_integer_entity(arg_entity->signature()->positioned_fields_indices().size()).id(), true };
    return {};
}
#endif

std::expected<functional_match_descriptor_ptr, error_storage> tuple_size_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& exp) const
{
    auto call_session = call.new_session(ctx);
    syntax_expression_t const* parg_expr;
    auto arg = call_session.use_next_positioned_argument(&parg_expr);
    if (!arg) return std::unexpected(arg.error());

    entity_identifier argtype = arg->is_const_result ? get_entity(ctx.u(), arg->value()).get_type() : arg->type();

    auto argerror = [parg_expr, arg] {
        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*parg_expr), arg->value_or_type, "a tuple"sv));
        //return std::unexpected(make_error<basic_general_error>(get_start_location(*parg_expr), "argument mismatch"sv, *parg_expr));
    };

    
    entity const& argent = get_entity(ctx.u(), argtype);
    
    entity_signature const* arg_sig = argent.signature();
    if (!arg_sig) return argerror();
    if (arg_sig->name != ctx.u().get(builtin_qnid::tuple)) return argerror();

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    auto pmd = make_shared<functional_match_descriptor>(call.location);
    pmd->result = field_descriptor{ ctx.u().make_integer_entity(arg_sig->positioned_fields_indices().size()).id, true };
    pmd->void_spans = std::move(call_session.void_spans);
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage> tuple_size_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    semantic::managed_expression_list exprs{ u };
    for (semantic::expression_span const& vsp : md.void_spans) {
        exprs.splice_back(el, vsp);
    }
    return syntax_expression_result_t{
        .expressions = std::move(exprs),
        .value_or_type = md.result.entity_id(),
        .is_const_result = true
    };
}

}
