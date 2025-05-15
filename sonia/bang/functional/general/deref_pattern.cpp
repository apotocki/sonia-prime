//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "deref_pattern.hpp"

#include "sonia/bang/functional/generic_pattern_base.ipp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> deref_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& exp) const
{
    auto call_session = call.new_session(ctx);
    syntax_expression_t const* parg_expr;
    auto arg = call_session.use_next_positioned_argument(exp, true, &parg_expr);
    if (!arg) return std::unexpected(arg.error());

    auto argerror = [parg_expr] {
        return std::unexpected(make_error<basic_general_error>(get_start_location(*parg_expr), "argument mismatch"sv, *parg_expr));
    };

    if (!arg->is_const_result) return argerror();
    entity const& argent = get_entity(ctx.u(), arg->value());
    if (argent.get_type() != ctx.u().get(builtin_eid::qname)) return argerror();

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    auto pmd = make_shared<functional_match_descriptor>(call.location);
    pmd->get_match_result(0).append_result(*arg);
    pmd->void_spans = std::move(call_session.void_spans);
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage> deref_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto & mr = md.get_match_result(0);
    auto const& ser = mr.results.front();

    semantic::expression_span exprs;
    for (semantic::expression_span const& vsp : md.void_spans) {
        exprs = el.concat(exprs, vsp);
    }
    exprs = el.concat(exprs, ser.expressions);
    BOOST_ASSERT(ser.is_const_result);
    qname_entity const& argent = static_cast<qname_entity const&>(get_entity(ctx.u(), ser.value()));

    auto res = base_expression_visitor{ ctx, el }(
        variable_reference{ annotated_qname{ argent.value(), md.location }, false }
    );
    if (!res) return std::unexpected(res.error());
    auto& er = res->first;
    er.expressions = el.concat(exprs, er.expressions);
    return std::move(er);
}

}
