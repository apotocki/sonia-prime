//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "to_string_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> to_string_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    auto call_session = call.new_session(ctx);
    auto arg = call_session.use_next_positioned_argument(exp);
    if (!arg) return std::unexpected(arg.error());
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    auto pmd = make_shared<functional_match_descriptor>(call.location);
    pmd->get_match_result(0).append_result(*arg);
    pmd->void_spans = std::move(call_session.void_spans);
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage> to_string_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto & mr = md.get_match_result(0);
    auto & er = mr.results.front();
    er.expressions = el.concat(md.merge_void_spans(el), er.expressions);
    if (er.is_const_result) {
        entity const& ent = get_entity(u, er.value());
        std::ostringstream oss;
        ent.print_to(oss, u);
        er.value_or_type = u.make_string_entity(oss.str()).id;
    } else {
        u.push_back_expression(el, er.expressions, semantic::invoke_function(u.get(builtin_eid::to_string)));
        er.value_or_type = u.get(builtin_eid::to_string);
    }
    return std::move(er);
}

}
