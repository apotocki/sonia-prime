//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "mut_pattern.hpp"

#include "sonia/bang/functional/generic_pattern_base.ipp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> mut_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& exp) const
{
    auto call_session = call.new_session(ctx);
    auto arg = call_session.use_next_positioned_argument(exp, false);
    if (!arg) return std::unexpected(arg.error());
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    auto pmd = make_shared<functional_match_descriptor>(call.location);
    pmd->get_match_result(0).append_result(*arg);
    pmd->void_spans = std::move(call_session.void_spans);
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage> mut_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    auto & mr = md.get_match_result(0);
    auto const& ser = mr.results.front();

    semantic::managed_expression_list exprs{ ctx.u() };
    for (semantic::expression_span const& vsp : md.void_spans) {
        exprs.splice_back(el, vsp);
    }
    exprs.splice_back(el, ser.expressions);
    if (ser.is_const_result) {
        ctx.u().push_back_expression(exprs, semantic::push_value{ ser.value() });
        return syntax_expression_result_t{
            .expressions = std::move(exprs),
            .value_or_type = get_entity(ctx.u(), ser.value()).get_type(),
            .is_const_result = false
        };
    }
    return syntax_expression_result_t{ 
        .expressions = std::move(exprs),
        .value_or_type = ser.value(),
        .is_const_result = false
    };
}

}
