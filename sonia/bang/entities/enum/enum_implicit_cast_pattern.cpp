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

//error_storage enum_implicit_cast_check_argument_type(fn_compiler_context& ctx, annotated_entity_identifier const& argtype, annotated_entity_identifier const& exptype)
//{
//    entity const& ent = ctx.u().eregistry_get(argtype.value);
//    struct_entity const* sent = dynamic_cast<struct_entity const*>(&ent);
//    if (!sent) {
//        return make_error<type_mismatch_error>(argtype.location, argtype.value, "a structure"sv);
//    }
//    auto uteid = sent->underlying_tuple_eid(ctx);
//    if (!uteid) return std::move(uteid.error());
//    if (*uteid != exptype.value) {
//        return make_error<type_mismatch_error>(exptype.location, exptype.value, *uteid);
//    }
//    return {};
//}

std::expected<functional_match_descriptor_ptr, error_storage> enum_implicit_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    unit& u = ctx.u();
    if (!exp) {
        return std::unexpected(make_error<basic_general_error>(call.location, "expected an enumeration result"sv));
    }
    entity const& ent = get_entity(u, exp.type);
    enum_entity const* penum = dynamic_cast<enum_entity const*>(&ent);
    if (!penum) {
        return std::unexpected(make_error<type_mismatch_error>(exp.location, exp.type, "an enumeration"sv));
    }

    functional_match_descriptor_ptr pmd;

    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        auto const& argexpr = arg.value();
        auto res = apply_visitor(ct_expression_visitor{ ctx, call.expressions, expected_result_t{ u.get(builtin_eid::identifier), true } }, argexpr);
        if (!res) {
            return std::unexpected(append_cause(
                make_error<basic_general_error>(pargname ? pargname->location : get_start_location(argexpr), "argument mismatch"sv, argexpr),
                std::move(res.error())));
        }
        //if (get<0>(*res) == u.get(builtin_eid::void_)) continue; // skip void argument

        if (pmd || pargname) {
            return std::unexpected(make_error<basic_general_error>(pargname ? pargname->location : get_start_location(argexpr), "argument mismatch"sv, argexpr));
        }

        entity const& ent = get_entity(u, res->value);
        identifier_entity const* pident = dynamic_cast<identifier_entity const*>(&ent);
        if (!pident) {
            return std::unexpected(make_error<value_mismatch_error>(get_start_location(argexpr), res->value, "an identifier"sv));
        }
        // check identifier value
        if (auto optpos = penum->find(pident->value()); !optpos) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "not an enumeration identifier"sv, res->value));
        }

        pmd = make_shared<functional_match_descriptor>(call);
        pmd->emplace_back(0, syntax_expression_result_t{
            .expressions = res->expressions,
            .value_or_type = res->value,
            .is_const_result = true
        });
    }
    if (!pmd) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter"sv));
    }
    pmd->signature.result.emplace(exp.type);
    return pmd;
}


std::expected<syntax_expression_result_t, error_storage> enum_implicit_cast_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& [_, ser] = md.matches.front();
    entity const& ent = get_entity(u, ser.value());
    identifier_entity const* pie = dynamic_cast<identifier_entity const*>(&ent);
    BOOST_ASSERT(pie);
    
    // return typed by enumeration string
    return syntax_expression_result_t{
        .expressions = md.merge_void_spans(el),
        .value_or_type = u.make_string_entity(u.print(pie->value()), md.signature.result->entity_id()).id,
        .is_const_result = true
    };
}

}
