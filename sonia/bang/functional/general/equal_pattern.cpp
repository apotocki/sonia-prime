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
    syntax_expression_result_t& larg_er = larg->first;
    entity_identifier ltype = larg_er.is_const_result ? get_entity(ctx.u(), larg_er.value()).get_type() : larg_er.type();

    auto rarg = call_session.use_next_positioned_argument(expected_result_t{ ltype } /*, &prarg_expr*/);
    if (!rarg) return std::unexpected(rarg.error());

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    auto pmd = make_shared<functional_match_descriptor>(call);
    pmd->emplace_back(0, larg_er);
    pmd->emplace_back(1, rarg->first);
    pmd->void_spans = std::move(call_session.void_spans);
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage> equal_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto & ler = get<1>(md.matches.front());
    auto & rer = get<1>(md.matches.back());

    syntax_expression_result_t result{
        .temporaries = std::move(ler.temporaries),
        .stored_expressions = el.concat(ler.stored_expressions, rer.stored_expressions),
        .expressions = md.merge_void_spans(el)
    };
    result.temporaries.insert(result.temporaries.end(), rer.temporaries.begin(), rer.temporaries.end());
    if (ler.is_const_result && rer.is_const_result) {
        result.is_const_result = true;
        result.value_or_type = u.make_bool_entity(ler.value() == rer.value()).id;
        return result;
    }

    result.value_or_type = u.get(builtin_eid::boolean);
    result.is_const_result = false;

    if (ler.is_const_result) {
        // Get non-const type for the left argument
        entity_identifier ltype = get_entity(u, ler.value()).get_type();
        
        // Create implicit cast call instead of direct push_value
        pure_call_t cast_call{ md.call_location };
        cast_call.emplace_back(annotated_entity_identifier{ ler.value(), md.call_location });

        // Try to find an implicit cast from const value to non-const type
        auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, el, expected_result_t{ .type = ltype, .location = md.call_location });
        if (!match) {
            return std::unexpected(match.error());
            // Fallback to direct push_value if implicit_cast is not available
            ///u.push_back_expression(el, result.expressions, semantic::push_value{ ler.value() });
        } else {
            // Apply the implicit cast and use its result
            auto cast_result = match->apply(ctx);
            if (!cast_result) {
                return std::unexpected(cast_result.error());
                // Fallback to direct push_value on cast error
                //u.push_back_expression(el, result.expressions, semantic::push_value{ ler.value() });
            } else {
                result.expressions = el.concat(result.expressions, cast_result->expressions);
                result.temporaries.insert(result.temporaries.end(), cast_result->temporaries.begin(), cast_result->temporaries.end());
            }
        }
    } else {
        result.expressions = el.concat(result.expressions, ler.expressions);
    }
    
    if (rer.is_const_result) {
        // Get non-const type for the right argument
        entity_identifier rtype = get_entity(u, rer.value()).get_type();
        
        // Create implicit cast call instead of direct push_value
        pure_call_t cast_call{ md.call_location };
        cast_call.emplace_back(annotated_entity_identifier{ rer.value(), md.call_location });

        // Try to find an implicit cast from const value to non-const type
        auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, el, expected_result_t{ .type = rtype, .location = md.call_location });
        if (!match) {
            // Fallback to direct push_value if implicit_cast is not available
            return std::unexpected(match.error());
            //u.push_back_expression(el, result.expressions, semantic::push_value{ rer.value() });
        } else {
            // Apply the implicit cast and use its result
            auto cast_result = match->apply(ctx);
            if (!cast_result) {
                return std::unexpected(cast_result.error());
                // Fallback to direct push_value on cast error
                //u.push_back_expression(el, result.expressions, semantic::push_value{ rer.value() });
            } else {
                result.expressions = el.concat(result.expressions, cast_result->expressions);
                result.temporaries.insert(result.temporaries.end(), cast_result->temporaries.begin(), cast_result->temporaries.end());
            }
        }
    } else {
        result.expressions = el.concat(result.expressions, rer.expressions);
    }

    u.push_back_expression(el, result.expressions, semantic::invoke_function(u.get(builtin_eid::equal)));

    return std::move(result);
}

}
