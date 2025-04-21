//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "value_match_visitor.hpp"
#include "signature_matcher.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/errors/value_mismatch_error.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

value_match_visitor::value_match_visitor(fn_compiler_context& caller, fn_compiler_context& callee, syntax_expression_t const& ce, functional_binding& b, bool is_type_matching)
    : caller_ctx{ caller }, callee_ctx{ callee }, cexpr{ ce }, binding{ b }, matching_type{ is_type_matching }
{
}

value_match_visitor::result_type value_match_visitor::operator()(annotated_bool const& bv) const
{
    unit& u = callee_ctx.u();

    ct_expression_visitor sv{ callee_ctx, annotated_entity_identifier{ u.get(builtin_eid::boolean) } };
    auto ent_id = apply_visitor(sv, cexpr);
    if (!ent_id) return std::unexpected(std::move(ent_id.error()));
    entity_identifier expected_ent_id = bv.value ? u.get(builtin_eid::true_) : u.get(builtin_eid::false_);
    if (ent_id->value == expected_ent_id) return expected_ent_id;

    return std::unexpected(make_error<value_mismatch_error>(get_start_location(cexpr), ent_id->value, expected_ent_id, bv.location));
}

value_match_visitor::result_type value_match_visitor::operator()(annotated_qname_identifier const& aqi) const
{
    functional const& fnl = callee_ctx.u().fregistry_resolve(aqi.value);
    return fnl.default_entity(callee_ctx);
    //if (!opteid) return opteid;
    //if (*opteid) {
    //    return this->operator()(*opteid, aqi.location);
    //}

    //return std::unexpected(make_error<basic_general_error>(aqi.location, "not a variable or constant"sv, expr));
}

value_match_visitor::result_type value_match_visitor::operator()(annotated_entity_identifier const& aeid) const
{
    THROW_NOT_IMPLEMENTED_ERROR("value_match_visitor not implemented expression");
}

value_match_visitor::result_type value_match_visitor::operator()(function_call_t const& fc) const
{
    unit& u = callee_ctx.u();

    ct_expression_visitor sv{ callee_ctx, annotated_entity_identifier{ u.get(builtin_eid::qname) } };
    auto qn_ent_id = apply_visitor(sv, fc.fn_object);
    if (!qn_ent_id) return std::unexpected(std::move(qn_ent_id.error()));
    qname_identifier_entity qname_ent = static_cast<qname_identifier_entity const&>(get_entity(u, qn_ent_id->value));

    // check if can evaluate signature_pattern_
    auto match = callee_ctx.find(qname_ent.value(), fc);
    if (match) {
        THROW_NOT_IMPLEMENTED_ERROR("type_constraint_match_visitor not implemented expression");
    }

    // can't evaluate signature_pattern_ as a function, consider as a pattern
    // evaluate cexpr in caller context
    auto res = apply_visitor(ct_expression_visitor{ caller_ctx }, cexpr);
    if (!res) return std::unexpected(std::move(res.error()));

    entity const& type_ent = get_entity(u, res->value);
    entity_signature const* psig = type_ent.signature();
    if (!psig) {
        return std::unexpected(make_error<basic_general_error>(fc.location, "argument mismatch"sv, cexpr));
    }

    if (qname_ent.value() != psig->name) {
        return std::unexpected(make_error<basic_general_error>(fc.location, "argument mismatch"sv, cexpr));
    }

    size_t arg_index = 0;
    for (auto const& arg : fc.args) {
        annotated_identifier const* argname = arg.name();
        field_descriptor const* pfd = argname ? psig->find_field(argname->value) : psig->find_field(arg_index++);
        if (!pfd) {
            return std::unexpected(make_error<basic_general_error>(fc.location, "argument pattern mismatch"sv, cexpr));
        }
        signature_matcher_visitor smvis{ callee_ctx, binding, pfd->entity_id() };
        if (auto err = apply_visitor(smvis, arg.value()); err) return std::unexpected(std::move(err));
    }

    return type_ent.id();
}

value_match_visitor::result_type value_match_visitor::operator()(variable_identifier const& var) const
{
    auto optent = callee_ctx.lookup_entity(var.name);

    return apply_visitor(make_functional_visitor<result_type>([this, &var](auto eid_or_var) -> result_type
        {
            if constexpr (std::is_same_v<std::decay_t<decltype(eid_or_var)>, entity_identifier>) {
                if (!eid_or_var) { // if not defined
                    if (var.implicit) { // var to bind
                        auto res = apply_visitor(ct_expression_visitor{ caller_ctx }, cexpr);
                        if (!res) return std::unexpected(std::move(res.error()));
                        identifier varid = *var.name.value.begin();
                        binding.emplace_back(annotated_identifier{ varid, var.name.location }, res->value);
                        return res->value;
                    }
                    return std::unexpected(make_error<undeclared_identifier_error>(var.name));
                }
                auto res = apply_visitor(ct_expression_visitor{ caller_ctx, annotated_entity_identifier{ eid_or_var }, matching_type }, cexpr);
                if (!res) return std::unexpected(std::move(res.error()));
                return res->value;
            }
            else { // entity_ptr, that is variable_entity
                return std::unexpected(make_error<basic_general_error>(var.name.location, "argument mismatch"sv, var));
            }
        }), optent);

    //if (!*opteid) {
    //    if (var.implicit) {
    //        auto res = apply_visitor(ct_expression_visitor{ caller_ctx }, cexpr);
    //        if (!res) return std::unexpected(std::move(res.error()));
    //        identifier varid = *var.name.value.begin();
    //        binding.emplace_back(annotated_identifier{ varid, var.name.location }, *res);
    //        return res;
    //    }
    //    return std::unexpected(make_error<undeclared_identifier_error>(var.name));
    //}

    //return apply_visitor(ct_expression_visitor{ caller_ctx, annotated_entity_identifier{ *opteid }, matching_type }, cexpr);
}

}
