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

std::expected<functional_match_descriptor_ptr, error_storage> deref_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    auto call_session = call.new_session(ctx);
    std::pair<syntax_expression_t const*, size_t> arg_expr;
    auto arg = call_session.use_next_positioned_argument(exp, &arg_expr);
    if (!arg) return std::unexpected(arg.error());

    auto argerror = [&arg_expr] {
        return std::unexpected(make_error<basic_general_error>(get_start_location(*get<0>(arg_expr)), "argument mismatch"sv, *get<0>(arg_expr)));
    };
    syntax_expression_result_t& arg_er = arg->first;
    if (!arg_er.is_const_result) return argerror();
    entity const& argent = get_entity(ctx.u(), arg_er.value());
    if (argent.get_type() != ctx.u().get(builtin_eid::qname)) return argerror();

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    auto pmd = make_shared<functional_match_descriptor>(call);
    pmd->emplace_back(0, arg_er);
    pmd->void_spans = std::move(call_session.void_spans);
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage> deref_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto & [_, ser] = md.matches.front();

    ser.expressions = el.concat(md.merge_void_spans(el), ser.expressions);
    
    BOOST_ASSERT(ser.is_const_result);
    qname_entity const& argent = static_cast<qname_entity const&>(get_entity(ctx.u(), ser.value()));

    auto res = base_expression_visitor{ ctx, el }(
        variable_reference{ annotated_qname{ argent.value(), md.call_location }, false }
    );
    if (!res) return std::unexpected(res.error());
    auto& er = res->first;
    er.expressions = el.concat(ser.expressions, er.expressions);
    er.temporaries.insert(er.temporaries.begin(), ser.temporaries.begin(), ser.temporaries.end());
    return std::move(er);
}

}
