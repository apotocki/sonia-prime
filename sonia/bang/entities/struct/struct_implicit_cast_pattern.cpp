//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "struct_implicit_cast_pattern.hpp"
#include "struct_entity.hpp"

#include "sonia/bang/entities/prepared_call.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/ast/base_expression_visitor.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

error_storage struct_implicit_cast_check_argument_type(fn_compiler_context& ctx, annotated_entity_identifier const& argtype, expected_result_t const& exptype)
{
    entity const& ent = ctx.u().eregistry_get(argtype.value);
    struct_entity const* sent = dynamic_cast<struct_entity const*>(&ent);
    if (!sent) {
        return make_error<type_mismatch_error>(argtype.location, argtype.value, "a structure"sv);
    }
    auto uteid = sent->underlying_tuple_eid(ctx);
    if (!uteid) return std::move(uteid.error());
    if (*uteid != exptype.type) {
        return make_error<type_mismatch_error>(exptype.location, exptype.type, *uteid);
    }
    return {};
}

std::expected<functional_match_descriptor_ptr, error_storage> struct_implicit_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    if (!exp) {
        return std::unexpected(make_error<basic_general_error>(call.location, "expected a tuple result"sv));
    }
    unit& u = ctx.u();
    entity const& ent = get_entity(u, exp.type);
    entity_signature const* psig = ent.signature();
    if (!psig || psig->name != u.get(builtin_qnid::tuple)) {
        return std::unexpected(make_error<type_mismatch_error>(exp.location, exp.type, "a tuple"sv));
    }

    functional_match_descriptor_ptr pmd;

    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        auto const& argexpr = arg.value();
        if (pargname) { // named arguments are not expected
            auto res = apply_visitor(ct_expression_visitor{ ctx, call.expressions }, argexpr);
            if (!res || res->value != u.get(builtin_eid::void_)) {
                return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch"sv, argexpr));
            }
            continue; // skip void argument
        }
        auto res = apply_visitor(base_expression_visitor{ ctx, call.expressions }, argexpr);
        if (!res) return std::unexpected(std::move(res.error()));
        auto& ser = res->first;

        if (pmd) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "argument mismatch"sv, argexpr));
        }
        if (ser.is_const_result) {
            auto err = struct_implicit_cast_check_argument_type(ctx, annotated_entity_identifier{ ser.value(), get_start_location(argexpr) }, exp);
            if (err) return std::unexpected(std::move(err));
        } else {
            auto err = struct_implicit_cast_check_argument_type(ctx, annotated_entity_identifier{ ser.type(), get_start_location(argexpr) }, exp);
            if (err) return std::unexpected(std::move(err));
        }
        pmd = make_shared<functional_match_descriptor>(call);
        pmd->emplace_back(0, ser);
    }

    if (!pmd) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter $0"sv));
    }
    pmd->signature.result.emplace(exp.type);
    return pmd;
}


std::expected<syntax_expression_result_t, error_storage> struct_implicit_cast_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    syntax_expression_result_t result{
        .expressions = md.merge_void_spans(el),
        .value_or_type = md.signature.result->entity_id(),
        .is_const_result = false
    };
    for (auto& [_, mr] : md.matches) {
        result.temporaries.insert(result.temporaries.end(), mr.temporaries.begin(), mr.temporaries.end());
        result.stored_expressions = el.concat(result.stored_expressions, mr.stored_expressions);
        result.expressions = el.concat(result.expressions, mr.expressions);
        if (mr.is_const_result) {
            u.push_back_expression(el, result.expressions, semantic::push_value{ mr.value() });
        }
    }

    return result;
}

}
