//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_typename_get_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> tuple_typename_get_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);
    std::pair<syntax_expression_t const*, size_t> slf_arg_expr;
    auto slf_arg = call_session.use_named_argument(u.get(builtin_id::self), expected_result_t{ .type = u.get(builtin_eid::typename_), .modifier = value_modifier_t::constexpr_value }, &slf_arg_expr);
    if (!slf_arg) {
        if (!slf_arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument: `self`"sv));
        }
        return std::unexpected(std::move(slf_arg.error()));
    }

    std::pair<syntax_expression_t const*, size_t> prop_arg;
    alt_error prop_errors;
    auto property_arg = call_session.use_named_argument(u.get(builtin_id::property), expected_result_t{ .type = u.get(builtin_eid::integer), .modifier = value_modifier_t::constexpr_value }, &prop_arg);
    if (!property_arg && property_arg.error()) {
        prop_errors.alternatives.emplace_back(std::move(property_arg.error()));
        call_session.reuse_argument(get<1>(prop_arg));
        property_arg = call_session.use_named_argument(u.get(builtin_id::property), expected_result_t{ .type = u.get(builtin_eid::identifier), .modifier = value_modifier_t::constexpr_value }, &prop_arg);
    }
    if (!property_arg) {
        if (!property_arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument: `property`"sv));
        }
        if (prop_errors.alternatives.empty()) {
            return std::unexpected(std::move(property_arg.error()));
        } else {
            prop_errors.alternatives.emplace_back(std::move(property_arg.error()));
            return std::unexpected(make_error<alt_error>(std::move(prop_errors)));
        }
    }

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    syntax_expression_result_t& slf_arg_er = slf_arg->first;
    
    // This pattern only handles const results (tuple typename)
    if (!slf_arg_er.is_const_result) {
        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*get<0>(slf_arg_expr)), slf_arg_er.type(), "a constexpr tuple typename"sv));
    }

    entity const& slf_entity = get_entity(u, slf_arg_er.value());
    auto psig = slf_entity.signature();
    if (!psig || psig->name != u.get(builtin_qnid::tuple)) {
        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*get<0>(slf_arg_expr)), slf_arg_er.value(), "a tuple typename"sv));
    }
    if (psig->empty()) {
        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*get<0>(slf_arg_expr)), slf_arg_er.value(), "a not empty tuple typename"sv));
    }

    auto pmd = make_shared<tuple_typename_get_match_descriptor>(call, slf_entity, *psig);
    pmd->emplace_back(0, slf_arg_er);
    pmd->emplace_back(1, property_arg->first);
    
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> tuple_typename_get_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& tmd = static_cast<tuple_typename_get_match_descriptor&>(md);
    auto& slfer = get<1>(md.matches[0]);
    auto& proper = get<1>(md.matches[1]);

    slfer.temporaries.insert(slfer.temporaries.end(), proper.temporaries.begin(), proper.temporaries.end());
    syntax_expression_result_t result{
        .temporaries = std::move(slfer.temporaries),
        .is_const_result = true
    };

    // Helper to build tuple type (identifier, value)
    //auto make_named_tuple_type = [&](field_descriptor const& field) -> entity_identifier {
    //    entity_signature rsig{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) };
    //    rsig.emplace_back(u.make_identifier_entity(field.name()).id, true);
    //    rsig.emplace_back(field.entity_id(), field.is_const());
    //    return u.make_basic_signatured_entity(std::move(rsig)).id;
    //};

    //auto make_tuple_project_type = [&](identifier id_name) -> entity_identifier {
    //    entity_signature project_sig{ u.get(builtin_qnid::tuple_project), u.get(builtin_eid::typename_) };
    //    project_sig.emplace_back(u.make_identifier_entity(id_name).id, true);
    //    project_sig.emplace_back(tmd.tpl_entity.id, true);
    //    return u.make_basic_signatured_entity(std::move(project_sig)).id;
    //};

    // Case 1: property is constant
    BOOST_ASSERT(proper.is_const_result);
    entity const& property_entity = get_entity(u, proper.value());
    if (auto int_lit = dynamic_cast<const generic_literal_entity*>(&property_entity)) {
        size_t idx = int_lit->value().as<size_t>();
        if (auto* field = tmd.arg_sig.get_field(idx)) {
            if (field->name()) {
                entity_signature rsig{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) };
                rsig.emplace_back(u.make_identifier_entity(field->name()).id, true);
                rsig.emplace_back(field->entity_id(), field->is_const());
                result.value_or_type = u.make_basic_signatured_entity(std::move(rsig)).id;
            } else {
                result.value_or_type = field->entity_id();
            }
        } else {
            return std::unexpected(make_error<basic_general_error>(tmd.call_location, "tuple index out of range"sv, property_entity.id));
        }
    } else if (auto id_lit = dynamic_cast<const identifier_entity*>(&property_entity)) {
        auto frng = tmd.arg_sig.find_fields(id_lit->value());
        if (frng.first == frng.second) {
            return std::unexpected(make_error<basic_general_error>(tmd.call_location, "no such field in tuple"sv, id_lit->value()));
        }
        
        if (auto second = frng.first; ++second == frng.second) {
            // Single field case - return the field type directly
            field_descriptor const& field = tmd.arg_sig.field(frng.first->second);
            result.value_or_type = field.entity_id();
        } else {
            // Multiple fields with the same name: create a tuple type with unnamed elements
            // containing all matching field types in the same order as in the original tuple
            entity_signature result_sig{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) };

            for (auto it = frng.first; it != frng.second; ++it) {
                field_descriptor const& field = tmd.arg_sig.field(it->second);
                // Add field without name (unnamed element) but preserve constness
                result_sig.emplace_back(field.entity_id(), field.is_const());
            }

            result.value_or_type = u.make_basic_signatured_entity(std::move(result_sig)).id;
        }
    } else {
        return std::unexpected(make_error<type_mismatch_error>(tmd.call_location, proper.value(), "an integer or identifier"sv));
    }
    
    return result;
}

}
