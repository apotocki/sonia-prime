//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "pattern_matcher.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/auxiliary.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

error_storage pattern_matcher::match(pattern_t const& pattern, annotated_entity_identifier const& type) const
{
    return apply_visitor(make_functional_visitor<error_storage>([this, &pattern, &type](auto const& d) {
        if constexpr (std::is_same_v<placeholder, std::decay_t<decltype(d)>>) {
            return do_match_concepts(pattern.concepts, type); // Placeholder matches any entity, no binding needed
        } else if constexpr (std::is_same_v<context_identifier, std::decay_t<decltype(d)>>) {
            return do_match_context_identifier(d, pattern, type);
        } else if constexpr (std::is_same_v<syntax_expression_result_t, std::decay_t<decltype(d)>>) {
            auto expr_res = apply_visitor(ct_expression_visitor{ ctx_, expressions_ }, d);
            if (!expr_res) { return std::move(expr_res.error()); }
            if (type.value == expr_res->value) { return do_match_concepts(pattern.concepts, type); } // Expression matches the type
            return make_error<type_mismatch_error>(type.location, type.value, expr_res->value, get_start_location(d));
        } else  if constexpr (std::is_same_v<pattern_t::signature_descriptor, std::decay_t<decltype(d)>>) {
            return do_match(d, pattern, type);
        } else {
            // Should not happen, all cases should be handled above
            return make_error<basic_general_error>(get_start_location(pattern), "Internal error: Unknown pattern descriptor type"sv, type.value, type.location);
        }
    }), pattern.descriptor);
}

error_storage pattern_matcher::do_match_context_identifier(context_identifier cid, pattern_t const& pattern, annotated_entity_identifier const& type) const
{
    lex::resource_location const* loc;
    auto optbound = binding_.lookup(cid.name.value, &loc);
    if (optbound) {
        return apply_visitor(make_functional_visitor<error_storage>([this, &cid, &pattern, &type, loc](auto const& v) -> error_storage {
            if constexpr (std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
                if (v != type.value) {
                    return make_error<type_mismatch_error>(cid.name.location, type.value, v, *loc);
                }
                return do_match_concepts(pattern.concepts, type);
            } else if constexpr (std::is_same_v<shared_ptr<entity>, std::decay_t<decltype(v)>>) {
                BOOST_ASSERT((pattern.modifier & parameter_constraint_modifier_t::ellipsis) == parameter_constraint_modifier_t::ellipsis);
                THROW_NOT_IMPLEMENTED_ERROR("do_match_context_identifier for shared_ptr<entity> is not implemented yet");
            } else {
                return make_error<basic_general_error>(get_start_location(pattern), "Internal error: Context identifier is bound to an unexpected type"sv);
            }
        }), *optbound);
    }
    if ((pattern.modifier & parameter_constraint_modifier_t::ellipsis) == parameter_constraint_modifier_t::ellipsis) {
        THROW_NOT_IMPLEMENTED_ERROR("do_match_context_identifier for ellipsis is not implemented yet");
    }
    binding_.emplace_back(cid.name, type.value);
    return do_match_concepts(pattern.concepts, type); // Context identifier matches the type
}

error_storage pattern_matcher::do_match_concepts(span<const syntax_expression_t> concepts, annotated_entity_identifier const& type) const
{
    // Check if the type matches any of the concepts
    for (const auto& concept_expr : concepts) {
        auto concept_res = apply_visitor(ct_expression_visitor{ ctx_, ctx_.expression_store() }, concept_expr);
        if (!concept_res) {
            return concept_res.error();
        }
        // TODO: Process concept matches
    }
    return {}; // All concepts matched successfully
}

error_storage pattern_matcher::do_match(pattern_t::signature_descriptor const& sd, pattern_t const& pattern, annotated_entity_identifier const& type) const
{
    entity const& ent_type = get_entity(ctx_.u(), type.value);
    entity_signature const* psig = ent_type.signature();
    if (!psig) {
        return make_error<basic_general_error>(type.location, "Cannot match entity without signature"sv, type.value, get_start_location(pattern));
    }

    THROW_NOT_IMPLEMENTED_ERROR("do_match for signature_descriptor is not implemented yet");
#if 0
    // For signature_descriptor we need to match the name and fields
            
    // First check if the name matches
    auto err = apply_visitor(make_functional_visitor<error_storage>([this, psig](auto const& n) {
        if constexpr (std::is_same_v<placeholder, std::decay_t<decltype(n)>>) {
            return {}; // Placeholder matches any name
        } else if constexpr (std::is_same_v<annotated_qname, std::decay_t<decltype(n)>>) {
            functional const* fn = ctx_.lookup_functional(n.value);
            if (!fn || fn->id != psig->name) {
                return make_error<basic_general_error>(n.location,
                    "Signature name mismatch"sv, n.value, type.location);
            }
            return {};
        } else if constexpr (std::is_same_v<context_identifier, std::decay_t<decltype(n)>>) {
            auto optbound = ctx_.get_bound(n.name.value);
            if (optbound) {
                entity_identifier const* pbent = get<entity_identifier>(&*optbound);
                // Check if the bound entity matches the signature name
                if (!pbent || *pbent != psig->name) {
                    return make_error<basic_general_error>(n.name.location,
                        "Context identifier does not match signature name"sv, n.name.value, type.location);
                }
            } else {
                return make_error<basic_general_error>(n.name.location,
                    "Context identifier is not bound"sv, n.name.value, type.location);
            }
            // Bind the context identifier to the entity name
            // Use the getter method for qname identifier
            entity_identifier qname_entity = ctx_.u().get_qname_entity(psig->name);
            binding_.emplace_back(n.name, qname_entity);
            return {};
        } else if constexpr (std::is_same_v<syntax_expression_t, std::decay_t<decltype(n)>>) {
            // Evaluate the expression and check if it matches the signature name
            auto expr_res = apply_visitor(ct_expression_visitor{ctx_, ctx_.expression_store()}, n);
            if (!expr_res) {
                return expr_res.error();
            }
            
            entity const& expr_ent = get_entity(ctx_.u(), expr_res->value);
            if (expr_ent.get_type() == ctx_.u().get(builtin_eid::qname)) {
                // It's a qname identifier, check if it matches
                if (psig->name != expr_ent.id) {
                    return make_error<basic_general_error>(get_start_location(n), 
                        "Signature name mismatch"sv, expr_ent.id, type.location);
                }
                return {};
            } else {
                return make_error<basic_general_error>(get_start_location(n), 
                    "Expression does not evaluate to a valid signature name"sv, nullptr, type.location);
            }
        } else {
        }
    }), sd.name);
    
    if (err) return err;
    

            
    // Then match each field specified in the pattern
    for (const auto& field : sd.fields) {
        // Find the corresponding field in the entity signature
        const field_descriptor* pfd = nullptr;
        if (auto* pname = get<annotated_identifier>(&field.name)) {
            pfd = psig->find_field(pname->value);
            if (!pfd) {
                return make_error<basic_general_error>(pname->location, 
                    "Field not found in entity"sv, pname->value, type.location);
            }
        } else if (auto* pcontext = get<context_identifier>(&field.name)) {
            pfd = psig->find_field(pcontext->name.value);
            if (!pfd) {
                return make_error<basic_general_error>(pcontext->name.location, 
                    "Field not found in entity"sv, pcontext->name.value, type.location);
            }
        } else if (get<placeholder>(&field.name) || get<nullptr_t>(&field.name)) {
            // Cannot match unnamed fields without an index
            return make_error<basic_general_error>(type.location, 
                "Cannot match unnamed field without an index"sv, type.value, get_start_location(pattern_));
        } else if (auto* pexpr = get<syntax_expression_t>(&field.name)) {
            // Try to evaluate the expression to get a field name or index
            auto expr_res = apply_visitor(ct_expression_visitor{ctx_, ctx_.expression_store()}, *pexpr);
            if (!expr_res) {
                return expr_res.error();
            }
                
            // Check if the expression result is a field name or index
            entity const& expr_ent = get_entity(ctx_.u(), expr_res->value);
            if (expr_ent.get_type() == ctx_.u().get(builtin_eid::integer)) {
                // It's an integer literal, use it as an index
                size_t idx = 0; // Initialize with default
                    
                // TODO: Extract the integer value properly
                // For now, just try to find the field at index 0 as a placeholder
                pfd = psig->get_field(idx);
                if (!pfd) {
                    return make_error<basic_general_error>(get_start_location(*pexpr), 
                        "Field index out of range"sv, nullptr, type.location);
                }
            } else {
                return make_error<basic_general_error>(get_start_location(*pexpr), 
                    "Expression does not evaluate to a field name or index"sv, nullptr, type.location);
            }
        }
            
        if (pfd) {
            // Recursively match the field's pattern against the field's type
            annotated_entity_identifier field_type{pfd->entity_id(), type.location};
            pattern_matcher sub_matcher{ctx_, binding_, field.value};
            if (auto err = sub_matcher.match(field_type)) {
                return err;
            }
        }
    }
    return {};
#endif
}

#if 0
error_storage pattern_matcher::match_signature_name(
    variant<placeholder, annotated_qname, context_identifier, syntax_expression_t> const& name,
    entity_signature const& sig, 
    annotated_entity_identifier const& type) const
{
    
    else if (auto* ctx_id = get<context_identifier>(&name)) {
        // Bind the context identifier to the entity name
        // Use the getter method for qname identifier
        entity_identifier qname_entity = ctx_.u().get_qname_entity(sig.name);
        binding_.emplace_back(ctx_id->name, qname_entity);
        return {};
    }
    else if (auto* expr = get<syntax_expression_t>(&name)) {
        // Evaluate the expression and check if it matches the signature name
        auto expr_res = apply_visitor(ct_expression_visitor{ctx_, ctx_.expression_store()}, *expr);
        if (!expr_res) {
            return expr_res.error();
        }
        
        entity const& expr_ent = get_entity(ctx_.u(), expr_res->value);
        
        // TODO: Check if the entity is a qname identifier and extract its value
        // For now, just assume it matches
        
        return {};
    }
    
    // Should not happen, all cases should be handled above
    return make_error<basic_general_error>(type.location, "Unknown name type in signature descriptor"sv, type.value);
}
#endif
}