//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "equal_pattern.hpp"

#include "sonia/bang/functional/generic_pattern_base.ipp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> equal_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);
    //syntax_expression_t const* plarg_expr, * prarg_expr;
    auto larg = call_session.use_next_positioned_argument(expected_result_t{}/*, &plarg_expr*/);
    if (!larg) return std::unexpected(larg.error());
    entity_identifier ltype = larg->is_const_result ? get_entity(ctx.u(), larg->value()).get_type() : larg->type();

    auto rarg = call_session.use_next_positioned_argument(expected_result_t{ ltype } /*, &prarg_expr*/);
    if (!rarg) return std::unexpected(rarg.error());

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    auto pmd = make_shared<functional_match_descriptor>(call.location);
    pmd->get_match_result(0).append_result(*larg);
    pmd->get_match_result(1).append_result(*rarg);
    pmd->void_spans = std::move(call_session.void_spans);
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage> equal_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto & ler = md.get_match_result(0).results.front();
    auto & rer = md.get_match_result(1).results.front();
    ler.temporaries.insert(ler.temporaries.end(), rer.temporaries.begin(), rer.temporaries.end());

    if (ler.is_const_result && rer.is_const_result) {
        if (ler.value() == rer.value()) {
            return syntax_expression_result_t{
                .temporaries = std::move(ler.temporaries),
                .expressions = md.merge_void_spans(el),
                .value_or_type = u.make_bool_entity(true).id,
                .is_const_result = true
            };
        }
        return syntax_expression_result_t{
            .temporaries = std::move(ler.temporaries),
            .expressions = md.merge_void_spans(el),
            .value_or_type = u.make_bool_entity(false).id,
            .is_const_result = true
        };
    }

    syntax_expression_result_t result{
        .temporaries = std::move(ler.temporaries),
        .stored_expressions = el.concat(ler.stored_expressions, rer.stored_expressions),
        .expressions = md.merge_void_spans(el),
        .value_or_type = u.get(builtin_eid::boolean),
        .is_const_result = false
    };
    
    if (ler.is_const_result) {
        u.push_back_expression(el, result.expressions, semantic::push_value{ ler.value() });
    } else {
        result.expressions = el.concat(result.expressions, ler.expressions);
    }
    if (rer.is_const_result) {
        u.push_back_expression(el, result.expressions, semantic::push_value{ rer.value() });
    } else {
        result.expressions = el.concat(result.expressions, rer.expressions);
    }

    u.push_back_expression(el, result.expressions, semantic::invoke_function(u.get(builtin_eid::equal)));

    return std::move(result);
}

}
