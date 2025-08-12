//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "pattern_matcher.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"
#include "sonia/bang/errors/signature_name_mismatch_error.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

error_storage pattern_matcher::match(pattern_t const& pattern, annotated_entity_identifier const& type) const
{
    return apply_visitor(make_functional_visitor<error_storage>([this, &pattern, &type](auto const& d) {
        if constexpr (std::is_same_v<placeholder, std::decay_t<decltype(d)>>) {
            return do_match_concepts(pattern.concepts, type); // Placeholder matches any entity, no binding needed
        } else if constexpr (std::is_same_v<context_identifier, std::decay_t<decltype(d)>>) {
            return do_match_context_identifier(d, pattern, type);
        } else if constexpr (std::is_same_v<syntax_expression_t, std::decay_t<decltype(d)>>) {
            auto expr_res = apply_visitor(ct_expression_visitor{ ctx_, expressions_ }, d);
            if (!expr_res) { return std::move(expr_res.error()); }
            if (type.value == expr_res->value) { return do_match_concepts(pattern.concepts, type); } // Expression matches the type
            return make_error<type_mismatch_error>(type.location, type.value, expr_res->value, get_start_location(d));
        } else { //if constexpr (std::is_same_v<pattern_t::signature_descriptor, std::decay_t<decltype(d)>>) {
            static_assert(std::is_same_v<pattern_t::signature_descriptor, std::decay_t<decltype(d)>>);
            return do_match(d, pattern, type);
        //} else {
            // Should not happen, all cases should be handled above
        //    return make_error<basic_general_error>(get_start_location(pattern), "Internal error: Unknown pattern descriptor type"sv, type.value, type.location);
        }
    }), pattern.descriptor);
}

error_storage pattern_matcher::do_match_context_identifier(context_identifier cid, pattern_t const& pattern, annotated_entity_identifier const& type) const
{
    lex::resource_location const* loc;
    auto optbound = bindings_.lookup(cid.name.value, &loc);
    if (optbound) {
        return apply_visitor(make_functional_visitor<error_storage>([this, &cid, &pattern, &type, loc](auto const& v) -> error_storage {
            if constexpr (std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
                if (v != type.value) {
                    return make_error<type_mismatch_error>(cid.name.location, type.value, v, *loc);
                }
                return do_match_concepts(pattern.concepts, type);
            } else if constexpr (std::is_same_v<shared_ptr<entity>, std::decay_t<decltype(v)>>) {
                //BOOST_ASSERT((pattern.modifier & parameter_constraint_modifier_t::ellipsis) == parameter_constraint_modifier_t::ellipsis);
                THROW_NOT_IMPLEMENTED_ERROR("do_match_context_identifier for shared_ptr<entity> is not implemented yet");
            } else {
                return make_error<basic_general_error>(get_start_location(pattern), "Internal error: Context identifier is bound to an unexpected type"sv);
            }
        }), *optbound);
    }
    //if ((pattern.modifier & parameter_constraint_modifier_t::ellipsis) == parameter_constraint_modifier_t::ellipsis) {
    //    THROW_NOT_IMPLEMENTED_ERROR("do_match_context_identifier for ellipsis is not implemented yet");
    //}
    bindings_.emplace_back(cid.name, type.value);
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

    auto err = apply_visitor(make_functional_visitor<error_storage>([this, &type, psig](auto const& n) -> error_storage {
        if constexpr (std::is_same_v<annotated_qname, std::decay_t<decltype(n)>>) {
            functional const* fn = ctx_.lookup_functional(n.value);
            if (fn && fn->id() == psig->name) return {}; // Function matches the signature
            return make_error<signature_name_mismatch_error>(n.location, psig->name, fn->id(), type.location);
        } else {
            THROW_NOT_IMPLEMENTED_ERROR("do_match for signature_descriptor is not implemented yet");
        }
    }), sd.name);
    if (err) return err;

    fld_bit = sd.fields.begin();
    fld_end = sd.fields.end();
    span<const field_descriptor> smplfields = psig->fields();

    for (fld_it = fld_bit; fld_it != fld_end; ++fld_it) {
        pattern_t::field const& field = *fld_it;
        if (field.bound_variable) {
            lex::resource_location const* pl;
            auto const* pval = bindings_.lookup(field.bound_variable.value, &pl);
            if (pval) {
                return make_error<basic_general_error>(field.bound_variable.location,
                    "identifier is already bound"sv, field.bound_variable, *pl);
            }
        }
        if (field.ellipsis) {
            err = apply_visitor(make_functional_visitor<error_storage>([this, psig, &smplfields, &type](auto const& field_name) -> error_storage {
                if constexpr (std::is_same_v<nullptr_t, std::decay_t<decltype(field_name)>> || std::is_same_v<placeholder, std::decay_t<decltype(field_name)>>) {
                    return do_match_positioned_ellipsis_field(fld_it->bound_variable, smplfields);
                //} else if constexpr (std::is_same_v<context_identifier, std::decay_t<decltype(field_name)>>) {
                //    return do_match_positioned_ellipsis_field(field_name.name, smplfields);
                    //const field_descriptor* pfd = psig->find_field(field_name.name.value);
                    //return {}; // Field matches
                } else {
                    THROW_NOT_IMPLEMENTED_ERROR("do_match for signature_descriptor with annotated identifier is not implemented yet");
                }
            }), field.name);
            
        } else {
            err = apply_visitor(make_functional_visitor<error_storage>([this, &field, psig, &smplfields, &type](auto const& field_name) -> error_storage {
                if constexpr (std::is_same_v<nullptr_t, std::decay_t<decltype(field_name)>>) {
                    if (smplfields.empty()) {
                        return make_error<basic_general_error>(lex::resource_location{}, "Cannot match unnamed field in signature"sv, nullptr, type.location);
                    }
                    if (smplfields.front().name()) {
                        return make_error<basic_general_error>(lex::resource_location{}, "Field name mismatch in signature, expected unnamed"sv, smplfields.front().name(), type.location);
                    }
                    pattern_matcher field_pattern_matcher{ ctx_, bindings_, expressions_ };
                    return field_pattern_matcher.match(field.value, annotated_entity_identifier{ smplfields.front().entity_id() });
                } else if constexpr (std::is_same_v<placeholder, std::decay_t<decltype(field_name)>>) {
                    if (smplfields.empty()) {
                        return make_error<basic_general_error>(field_name.location, "Cannot match placeholder in signature"sv, nullptr, type.location);
                    }
                    pattern_matcher field_pattern_matcher{ ctx_, bindings_, expressions_ };
                    return field_pattern_matcher.match(field.value, annotated_entity_identifier{ smplfields.front().entity_id() });
                } else if constexpr (std::is_same_v<annotated_identifier, std::decay_t<decltype(field_name)>>) {
                    if (smplfields.empty()) {
                        return make_error<basic_general_error>(field_name.location, "Cannot match named field in signature"sv, field_name.value, type.location);
                    }
                    if (smplfields.front().name() != field_name.value) {
                        return make_error<basic_general_error>(field_name.location, "Field name mismatch in signature"sv, field_name.value, type.location);
                    }
                    pattern_matcher field_pattern_matcher{ ctx_, bindings_, expressions_ };
                    return field_pattern_matcher.match(field.value, annotated_entity_identifier{ smplfields.front().entity_id() });
                } else {
                    THROW_NOT_IMPLEMENTED_ERROR("do_match for signature_descriptor with non-ellipsis field is not implemented yet");
                }
            }), field.name);
            if (!err && field.bound_variable) {
                bindings_.emplace_back(field.bound_variable, smplfields.front().entity_id());
            }
            smplfields = smplfields.subspan(1); // Move to the next field
        }
        if (err) return err;
    }
#if 0
    // Then match each field specified in the pattern
    for (const auto& field : sd.fields) {
        err = apply_visitor(make_functional_visitor<error_storage>([this, psig, &type](auto const& field_name) -> error_storage {
            if constexpr (std::is_same_v<placeholder, std::decay_t<decltype(field_name)>> || std::is_same_v<nullptr_t, std::decay_t<decltype(field_name)>>) {
                return {}; // Placeholder or nullptr matches any field name
            } else if constexpr (std::is_same_v<annotated_identifier, std::decay_t<decltype(field_name)>>) {
                const field_descriptor* pfd = psig->find_field(field_name.value);
                if (!pfd) {
                    return make_error<basic_general_error>(field_name.location,
                        "Field not found in entity signature"sv, field_name.value, type.location);
                }
                return {}; // Field matches
            } else if constexpr (std::is_same_v<context_identifier, std::decay_t<decltype(field_name)>>) {
                lex::resource_location const* pl;
                auto const* pval = binding_.lookup(field_name.name.value, &pl);
                if (pval) {
                    return make_error<basic_general_error>(field_name.name.location,
                        "identifier is already bound"sv, field_name.name.value, *pl);
                }
                return do_match_field_value(field_name.name.value);
                THROW_NOT_IMPLEMENTED_ERROR("do_match for signature_descriptor with context identifier is not implemented yet");
                const field_descriptor* pfd = psig->find_field(field_name.name.value);
                return {}; // Field matches
            } else  if constexpr (std::is_same_v<syntax_expression_t, std::decay_t<decltype(field_name)>>) {
                auto expr_res = apply_visitor(base_expression_visitor{ ctx_, ctx_.expression_store(),
                    expected_result_t { .type = ctx_.u().get(builtin_eid::qname), .location = get_start_location(field_name), .modifier = value_modifier_t::constexpr_value } }, field_name);
                if (!expr_res) return expr_res.error();
                THROW_NOT_IMPLEMENTED_ERROR("do_match for signature_descriptor with syntax expression is not implemented yet");
                // const field_descriptor* pfd = psig->find_field(field_name.value);
                //if (!pfd) {
                //    return make_error<basic_general_error>(field_name.location,
                //        "Field not found in entity signature"sv, field_name.value, type.location);
               // }
                return {}; // Field matches
            }
        }), field.name);
        if (err) return err;
    }

    if (!sd.fields.empty()) {
        THROW_NOT_IMPLEMENTED_ERROR("do_match for signature_descriptor with fields is not implemented yet");
    }
#endif
    return do_match_concepts(pattern.concepts, type); // Context identifier matches the type
}

error_storage pattern_matcher::do_match_positioned_ellipsis_field(annotated_identifier boundid, span<const field_descriptor>& smplfields) const
{
    unit& u = ctx_.u();
    // get next not-ellipsis field or named field
    //small_vector<field_descriptor, 8> accumulated_results;
    entity_signature accumulated_results(u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_));

    auto finalize_ellipsis = [this, &u, &boundid, &accumulated_results]() {
        if (boundid) {
            bindings_.emplace_back(boundid, u.make_empty_entity(u.make_basic_signatured_entity(std::move(accumulated_results))).id);
        }
    };

    fields_t::const_iterator next_field_it = fld_it;
    for (++next_field_it;; ++next_field_it) {
        if (next_field_it == fld_end) {
            --next_field_it;
            break;
        }
        pattern_t::field const& fld = *next_field_it;
        if (!fld.ellipsis) {
            break; // Found a non-ellipsis field
        }
        THROW_NOT_IMPLEMENTED_ERROR("do_match_positioned_ellipsis_field : ellipsis with next field is not implemented yet");
#if 0
        if (fld.value) {
            syntax_expression_t const* pconstraint = get<syntax_expression_t>(&fld.value);
            entity_identifier pconstraint_value_eid;
            annotated_identifier const& param_name = pd.ename.self_or(pd.inames.front());
            auto argexp_res = resolve_expression_expected_result(callee_ctx, param_name, pd.modifier, *pconstraint, pconstraint_value_eid);
            if (!argexp_res) { // error resolving constraint, rollback
                --next_param_it;
                break;
            }
            param_exps.emplace_back(std::move(*argexp_res));
            if (ellipsis && pconstraint_value_eid) {
                return make_error<basic_general_error>(
                    call.location, "Ellipsis with constraint value is not supported"sv, param_name.value, param_name.location);
            }
        }
        else {
            param_exps.emplace_back();
        }
        param_descriptors.emplace_back(&pd);
        if (pd.ename || !ellipsis) break;
#endif
        //bool ellipsis = apply_visitor(make_functional_visitor<bool>([](auto const& v) {
        //    return v.ellipsis;
        //}), pd.constraint);
    }
    for (auto group_field_it = next_field_it;;) {
        if (smplfields.empty()) {
            finalize_ellipsis();
            break;
        }
        pattern_t::field const& fld = *group_field_it;

        field_descriptor const& fd = smplfields.front();
        
        pattern_matcher field_pattern_matcher{ ctx_, bindings_, expressions_ };
        error_storage err = field_pattern_matcher.match(fld.value, annotated_entity_identifier{ fd.entity_id() });
        if (err) {
            if (group_field_it != fld_it) {
                --group_field_it;
                continue;
            }
            return append_cause(
                make_error<basic_general_error>(get_start_location(fld.value), "Cannot match ellipsis argument pattern"sv, nullptr),
                std::move(err)
            );
        }
            
        if (group_field_it == fld_it) {
            accumulated_results.emplace_back(fd.name(), fd.entity_id(), true);
            //if (fd.is_const()) {
            //    // check that fd.entity_id() is not a type
            //    entity const& fdent = get_entity(u, fd.entity_id());
            //    if (fdent.get_type() == u.get(builtin_eid::typename_)) {
            //        THROW_NOT_IMPLEMENTED_ERROR("do_match_positioned_ellipsis_field : const field with typename is not implemented yet");
            //    }
            //}
            group_field_it = next_field_it;
            smplfields = smplfields.subspan(1); // Remove the matched field from the span
        } else { // matched by next to ellipsis parameter
            finalize_ellipsis();
            break;
        }
    }

    return {};
}

//void pattern_matcher::finalize_ellipsis(identifier boundid, span<field_descriptor> ellipsis_span) const
//{
//    entity_signature ellipsis_type_sig(u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_));
//    for (field_descriptor const& fd : ellipsis_span) {
//
//    }
//
//    THROW_NOT_IMPLEMENTED_ERROR("finalize_ellipsis is not implemented yet");
//}

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