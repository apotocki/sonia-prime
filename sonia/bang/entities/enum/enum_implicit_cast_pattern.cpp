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

std::expected<functional_match_descriptor_ptr, error_storage> enum_implicit_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& e) const
{
    unit& u = ctx.u();
    if (!e) {
        return std::unexpected(make_error<basic_general_error>(call.location, "expected an enumeration result"sv));
    }
    entity const& ent = u.eregistry_get(e.value);
    enum_entity const* penum = dynamic_cast<enum_entity const*>(&ent);
    if (!penum) {
        return std::unexpected(make_error<type_mismatch_error>(e.location, e.value, "an enumeration"sv));
    }

    functional_match_descriptor_ptr pmd;

    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        auto const& argexpr = arg.value();
        auto res = apply_visitor(ct_expression_visitor{ ctx, annotated_entity_identifier{ u.get(builtin_eid::identifier) } }, argexpr);
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

        pmd = make_shared<functional_match_descriptor>();
        pmd->get_match_result(0).append_const_result(*res);
    }
    if (!pmd) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter"sv));
    }
    pmd->result = field_descriptor{ e.value };
    return pmd;
}


std::expected<syntax_expression_result_t, error_storage> enum_implicit_cast_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t&, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& ser = md.get_match_result(0).results.front();
    entity const& ent = get_entity(u, ser.value());
    identifier_entity const* pie = dynamic_cast<identifier_entity const*>(&ent);
    BOOST_ASSERT(pie);
    
    // return typed by enumeration string
    return make_result(u, u.make_string_entity(u.print(pie->value()), md.result.entity_id()).id());
}

}
