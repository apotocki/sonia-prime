//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "create_identifier_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

// __id(const string)
std::expected<functional_match_descriptor_ptr, error_storage> create_identifier_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);
    std::pair<syntax_expression_t const*, size_t> arg_expr;
    auto arg = call_session.use_next_positioned_argument(expected_result_t{ .type = ctx.u().get(builtin_eid::string), .modifier = parameter_constraint_modifier_t::const_type }, &arg_expr);
    if (!arg) {
        if (!arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing argument"sv));
        }
        return std::unexpected(arg.error());
    }
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    auto pmd = make_shared<functional_match_descriptor>(call);
    pmd->emplace_back(0, arg->first);
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> create_identifier_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& [_, ser] = md.matches.front();
    entity_identifier strid = ser.value();
    string_literal_entity const& str_ent = static_cast<string_literal_entity const&>(get_entity(u, ser.value()));
    identifier id = u.slregistry().resolve(str_ent.value());
    semantic::managed_expression_list rel{ u };
    rel.splice_back(el, ser.expressions);
        
    return syntax_expression_result_t{
        .expressions = std::move(rel),
        .value_or_type = u.make_identifier_entity(id).id,
        .is_const_result = true
    };
}

}
