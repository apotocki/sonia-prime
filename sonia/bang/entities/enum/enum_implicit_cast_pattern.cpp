//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "enum_implicit_cast_pattern.hpp"
#include "enum_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/ast/base_expression_visitor.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"
#include "sonia/bang/errors/value_mismatch_error.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> enum_implicit_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    if (!exp.type) {
        return std::unexpected(make_error<basic_general_error>(call.location, "expected an enumeration result"sv));
    }
    unit& u = ctx.u();
    entity const& enum_ent = get_entity(u, exp.type);
    enum_entity const* penum = dynamic_cast<enum_entity const*>(&enum_ent);
    if (!penum) {
        return std::unexpected(make_error<type_mismatch_error>(exp.location, exp.type, "an enumeration"sv));
    }

    auto call_session = call.new_session(ctx);
    prepared_call::argument_descriptor_t arg_expr;
    auto arg = call_session.use_next_positioned_argument(expected_result_t{ .type = u.get(builtin_eid::identifier), .modifier = value_modifier_t::constexpr_value }, &arg_expr);
    if (!arg) {
        if (!arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument"sv));
        }
        return std::unexpected(std::move(arg.error()));
    }
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    syntax_expression_result_t& arg_er = arg->first;
    entity const& ent = get_entity(u, arg_er.value());
    identifier_entity const* pident = dynamic_cast<identifier_entity const*>(&ent);
    BOOST_ASSERT(pident);
    
    // check identifier value
    if (auto optpos = penum->find(pident->value()); !optpos) {
        return std::unexpected(make_error<basic_general_error>(get_start_location(*get<0>(arg_expr)), "not an enumeration identifier"sv, pident->id));
    }

    functional_match_descriptor_ptr pmd = make_shared<functional_match_descriptor>(call);
    pmd->emplace_back(0, arg_er);
    bool is_runtime = can_be_only_runtime(exp.modifier);
    pmd->signature.result.emplace(is_runtime ? exp.type : u.make_string_entity(u.print(pident->value()), exp.type).id, !can_be_only_runtime(exp.modifier));
    return pmd;
}


std::expected<syntax_expression_result_t, error_storage> enum_implicit_cast_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    syntax_expression_result_t& ser = get<1>(md.matches.front());

    auto const& rfd = *md.signature.result;
    syntax_expression_result_t result{
        .value_or_type = rfd.entity_id(),
        .is_const_result = rfd.is_const()
    };
    if (!result.is_const_result) {
        entity const& ent = get_entity(u, ser.value());
        identifier_entity const* pident = dynamic_cast<identifier_entity const*>(&ent);
        BOOST_ASSERT(pident);
        result.temporaries = std::move(ser.temporaries);
        result.expressions = ser.expressions;
        u.push_back_expression(el, result.expressions, semantic::push_value{ smart_blob{ string_blob_result(u.print(pident->value())) } });
    }
    return result;
}

}
