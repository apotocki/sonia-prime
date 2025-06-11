//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_get_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"
//#include "sonia/bang/entities/functions/match_utility.ipp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> tuple_get_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);
    syntax_expression_t const* pslf_arg_expr;
    auto slf_arg = call_session.use_named_argument(u.get(builtin_id::self), expected_result_t{}, &pslf_arg_expr);
    if (!slf_arg) {
        if (!slf_arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument: `self`"sv));
        }
        return std::unexpected(std::move(slf_arg.error()));
    }

    syntax_expression_t const* pprop_arg_expr;
    alt_error prop_errors;
    auto property_arg = call_session.use_named_argument(u.get(builtin_id::property), expected_result_t{ u.get(builtin_eid::integer) }, &pprop_arg_expr);
    if (!property_arg && property_arg.error()) {
        prop_errors.alternatives.emplace_back(std::move(property_arg.error()));
        property_arg = call_session.use_named_argument(u.get(builtin_id::property), expected_result_t{ u.get(builtin_eid::identifier) }, &pprop_arg_expr);
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

    shared_ptr<tuple_get_match_descriptor> pmd;
    entity_identifier slftype;
    if (slf_arg->is_const_result) {
        entity const& slf_entity = get_entity(u, slf_arg->value());
        if (auto psig = slf_entity.signature(); psig && psig->name == u.get(builtin_qnid::tuple)) {
            if (psig->empty()) {
                return std::unexpected(make_error<type_mismatch_error>(get_start_location(*pslf_arg_expr), slf_arg->value(), "a not empty tuple type"sv));
            }
            pmd = make_shared<tuple_get_match_descriptor>(slf_entity, *psig, true, call.location);
        } else {
            slftype = slf_entity.get_type();
        }
    } else {
        slftype = slf_arg->type();
    }
    if (!pmd) {
        entity const& tpl_entity = get_entity(u, slftype);
        entity_signature const* psig = tpl_entity.signature();
        if (!psig || psig->name != u.get(builtin_qnid::tuple)) {
            return std::unexpected(make_error<type_mismatch_error>(get_start_location(*pslf_arg_expr), slftype, "a tuple"sv));
        }
        if (psig->empty()) {
            return std::unexpected(make_error<type_mismatch_error>(get_start_location(*pslf_arg_expr), slftype, "a not empty tuple"sv));
        }
        pmd = make_shared<tuple_get_match_descriptor>(tpl_entity, *tpl_entity.signature(), false, call.location);
    }
    pmd->get_match_result(0).append_result(*slf_arg);
    pmd->void_spans = std::move(call_session.void_spans);

    pmd->get_match_result(1).append_result(*property_arg);
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> tuple_get_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& tmd = static_cast<tuple_get_match_descriptor&>(md);
    auto& slfer = md.get_match_result(0).results.front();
    auto& proper = md.get_match_result(1).results.front();

    slfer.temporaries.insert(slfer.temporaries.end(), proper.temporaries.begin(), proper.temporaries.end());

    // Helper to build tuple type (identifier, value)
    auto make_named_tuple_type = [&](field_descriptor const& field) -> entity_identifier {
        entity_signature rsig{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) };
        rsig.emplace_back(u.make_identifier_entity(field.name()).id, true);
        rsig.emplace_back(field.entity_id(), field.is_const());
        return u.make_basic_signatured_entity(std::move(rsig)).id;
    };

    auto make_tuple_project_type = [&](identifier id_name) -> entity_identifier {
        entity_signature project_sig{ u.get(builtin_qnid::tuple_project), u.get(builtin_eid::typename_) };
        project_sig.emplace_back(u.make_identifier_entity(id_name).id, true);
        project_sig.emplace_back(tmd.tpl_entity.id, true);
        return u.make_basic_signatured_entity(std::move(project_sig)).id;
    };

    // Case 1: Both self and property are constant
    if (slfer.is_const_result && proper.is_const_result) {
        entity const& property_entity = get_entity(u, proper.value());
        if (auto int_lit = dynamic_cast<const integer_literal_entity*>(&property_entity)) {
            size_t idx = static_cast<size_t>(int_lit->value());
            if (auto* field = tmd.arg_sig.get_field(idx)) {
                entity_identifier result_type = field->name() ? make_named_tuple_type(*field) : field->entity_id();
                return syntax_expression_result_t{
                    .temporaries = std::move(slfer.temporaries),
                    .expressions = md.merge_void_spans(el),
                    .value_or_type = field->name() ? u.make_empty_entity(result_type).id : result_type,
                    .is_const_result = true
                };
            } else {
                return std::unexpected(make_error<basic_general_error>(tmd.call_location, "tuple index out of range"sv));
            }
        } else if (auto id_lit = dynamic_cast<const identifier_entity*>(&property_entity)) {
            auto frng = tmd.arg_sig.find_fields(id_lit->value());
            if (frng.first == frng.second) {
                return std::unexpected(make_error<basic_general_error>(tmd.call_location, "no such field in tuple"sv, id_lit->value()));
            }
            
            if (auto second = frng.first; ++second == frng.second) {
                // Single field case - return the field value directly
                field_descriptor const& field = tmd.arg_sig.field(frng.first->second);
                return syntax_expression_result_t{
                    .temporaries = std::move(slfer.temporaries),
                    .expressions = md.merge_void_spans(el),
                    .value_or_type = field.entity_id(),
                    .is_const_result = true
                };
            } else {
                return syntax_expression_result_t{
                    .temporaries = std::move(slfer.temporaries),
                    .expressions = md.merge_void_spans(el),
                    .value_or_type = u.make_empty_entity(make_tuple_project_type(id_lit->value())).id,
                    .is_const_result = true
                };
            }
        } else {
            return std::unexpected(make_error<type_mismatch_error>(tmd.call_location, proper.value(), "an integer or identifier"sv));
        }
    }

    // Case 2: self is not constant, property is constant
    if (!slfer.is_const_result && proper.is_const_result) {
        entity const& property_entity = get_entity(u, proper.value());
        entity_identifier result_type;
        const field_descriptor* field = nullptr;

        if (auto int_lit = dynamic_cast<const integer_literal_entity*>(&property_entity)) {
            size_t idx = static_cast<size_t>(int_lit->value());
            field = tmd.arg_sig.get_field(idx);
            if (!field) {
                return std::unexpected(make_error<basic_general_error>(tmd.call_location, "tuple index out of range"sv, property_entity.id));
            }
            result_type = field->name() ? make_named_tuple_type(*field) : field->entity_id();
        } else if (auto id_lit = dynamic_cast<const identifier_entity*>(&property_entity)) {
            // Use find_fields to get all fields with this name
            auto frng = tmd.arg_sig.find_fields(id_lit->value());
            if (frng.first == frng.second) {
                return std::unexpected(make_error<basic_general_error>(tmd.call_location, "no such field in tuple"sv, id_lit->value()));
            }
            
            if (auto second = frng.first; ++second == frng.second) {
                // Only one field with this name
                field = &tmd.arg_sig.field(frng.first->second);
                result_type = field->entity_id();
            } else {
                // More than one field with this name: return tuple_project type
                return syntax_expression_result_t{
                    .temporaries = std::move(slfer.temporaries),
                    .stored_expressions = std::move(slfer.stored_expressions),
                    .expressions = el.concat(md.merge_void_spans(el), slfer.expressions),
                    .value_or_type = make_tuple_project_type(id_lit->value()),
                    .is_const_result = false
                };
            }
        } else {
            return std::unexpected(make_error<type_mismatch_error>(tmd.call_location, proper.value(), "an integer or identifier"sv));
        }

        // If the field is const, return as a constant result
        if (field->is_const()) {
            return syntax_expression_result_t{
                .temporaries = std::move(slfer.temporaries),
                .expressions = md.merge_void_spans(el),
                .value_or_type = field->name() ? u.make_empty_entity(result_type).id : result_type,
                .is_const_result = true
            };
        }

        // Count non-const fields
        size_t non_const_count = 0; 
        // Compute runtime index among non-const fields
        size_t runtime_index = 0;

        auto sp = tmd.arg_sig.fields();
        for (auto it = sp.begin(), eit = sp.end(); it != eit; ++it) {
            const auto& f = *it;
            if (!f.is_const()) {
                ++non_const_count;
                if (field != &f) {
                    ++runtime_index;
                } else if (non_const_count > 1) {
                    break;
                } else {
                    for(++it; it != eit; ++it) {
                        if (!it->is_const()) {
                            ++non_const_count;
                            break;
                        }
                    }
                    break;
                }
            }
        }
        
        // Optimization: if only one runtime field, just return 'self' with the requested type
        if (non_const_count == 1) {
            return syntax_expression_result_t{
                .temporaries = std::move(slfer.temporaries),
                .stored_expressions = std::move(slfer.stored_expressions),
                .expressions = el.concat(md.merge_void_spans(el), slfer.expressions),
                .value_or_type = result_type,
                .is_const_result = false
            };
        }

        semantic::expression_span exprs = md.merge_void_spans(el);
        exprs = el.concat(exprs, slfer.expressions);
        u.push_back_expression(el, exprs, semantic::push_value{ runtime_index });
        u.push_back_expression(el, exprs, semantic::invoke_function(u.get(builtin_eid::array_at)));

        return syntax_expression_result_t{
            .temporaries = std::move(slfer.temporaries),
            .stored_expressions = std::move(slfer.stored_expressions),
            .expressions = std::move(exprs),
            .value_or_type = result_type,
            .is_const_result = false
        };
    }

    // Case 3: self is constant, property is not constant
    if (slfer.is_const_result && !proper.is_const_result) {
        // The tuple structure is known, but the property is dynamic.
        // We need to produce a union of all possible result types.
        std::vector<entity_identifier> possible_types;
        bool any_named = false;
        for (const auto& field : tmd.arg_sig.fields()) {
            if (field.name()) {
                any_named = true;
                // (identifier, value) tuple type
                entity_signature rsig{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) };
                rsig.emplace_back(u.make_identifier_entity(field.name()).id, true);
                rsig.emplace_back(field.entity_id(), field.is_const());
                possible_types.push_back(u.make_basic_signatured_entity(std::move(rsig)).id);
            } else {
                possible_types.push_back(field.entity_id());
            }
        }

        // For simplicity, use the first type if only one, or create a union otherwise
        entity_identifier result_type;
        if (possible_types.size() == 1) {
            result_type = possible_types.front();
        } else {
            result_type = u.make_union_type_entity(possible_types).id;
        }

        // Merge expressions
        semantic::expression_span exprs = el.concat(md.merge_void_spans(el), slfer.expressions);
        exprs = el.concat(exprs, proper.expressions);

        // TODO: Insert runtime selection logic here if needed
        THROW_NOT_IMPLEMENTED_ERROR("Tuple get pattern with const `self` and non-const `property` is not implemented yet."sv);
        return syntax_expression_result_t{
            .temporaries = std::move(slfer.temporaries),
            .expressions = std::move(exprs),
            .value_or_type = result_type,
            .is_const_result = false
        };
    }

    THROW_NOT_IMPLEMENTED_ERROR("Tuple get pattern with non-const `self` and non-const `property` is not implemented yet."sv);
}

}
