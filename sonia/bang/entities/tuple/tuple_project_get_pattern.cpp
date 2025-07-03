//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_project_get_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> tuple_project_get_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);
    
    prepared_call::argument_descriptor_t slf_arg_expr;
    auto slf_arg = call_session.use_named_argument(u.get(builtin_id::self), expected_result_t{}, &slf_arg_expr);
    if (!slf_arg) {
        if (!slf_arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument: `self`"sv));
        }
        return std::unexpected(std::move(slf_arg.error()));
    }

    //std::pair<syntax_expression_t const*, size_t> prop_arg_expr;
    // Note: We only accept integer for property in tuple_project_get
    auto property_arg = call_session.use_named_argument(u.get(builtin_id::property), expected_result_t{ u.get(builtin_eid::integer) });// , & prop_arg_expr);
    if (!property_arg) {
        if (!property_arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument: `property`"sv));
        }
        return std::unexpected(std::move(property_arg.error()));
    }

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    shared_ptr<tuple_project_get_match_descriptor> pmd;
    entity_identifier slftype;
    syntax_expression_result_t& slf_arg_er = slf_arg->first;
    if (slf_arg_er.is_const_result) {
        entity const& slf_entity = get_entity(u, slf_arg_er.value());
        slftype = slf_entity.get_type();
    } else {
        slftype = slf_arg_er.type();
    }

    entity const& tpl_prj_entity = get_entity(u, slftype);
    auto psig = tpl_prj_entity.signature();
    if (!psig || psig->name != u.get(builtin_qnid::tuple_project)) {
        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*get<0>(slf_arg_expr)), slftype, "a tuple_project"sv));
    }
        
    // Extract project name and original tuple from signature
    if (psig->field_count() != 2 || !psig->result || psig->result->entity_id() != u.get(builtin_eid::typename_) ) { // tuple_project, (id, orig_tuple)
        return std::unexpected(make_error<basic_general_error>(get_start_location(*get<0>(slf_arg_expr)), "invalid tuple_project signature"sv, tpl_prj_entity.id));
    }
        
    // Get project name from signature (first field)
    identifier project_name;
    entity const& name_entity = get_entity(u, psig->field(0).entity_id());
    if (identifier_entity const* pident = dynamic_cast<const identifier_entity*>(&name_entity)) {
        project_name = pident->value();
    } else {
        return std::unexpected(make_error<basic_general_error>(get_start_location(*get<0>(slf_arg_expr)), "invalid tuple_project name"sv, name_entity.id));
    }
        
    // Get original tuple entity (third field)
    entity const& orig_tuple = get_entity(u, psig->field(1).entity_id());
    auto orig_sig = orig_tuple.signature();
    if (!orig_sig || orig_sig->name != u.get(builtin_qnid::tuple)) {
        return std::unexpected(make_error<basic_general_error>(get_start_location(*get<0>(slf_arg_expr)), "invalid tuple_project origin tuple"sv, orig_tuple.id));
    }
        
    pmd = make_shared<tuple_project_get_match_descriptor>(call, project_name, orig_tuple, *orig_sig);

    pmd->emplace_back(0, slf_arg_er);
    pmd->void_spans = std::move(call_session.void_spans);
    pmd->emplace_back(1, property_arg->first);
    
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> tuple_project_get_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& tmd = static_cast<tuple_project_get_match_descriptor&>(md);
    auto& slfer = get<1>(md.matches[0]);
    auto& proper = get<1>(md.matches[1]);

    slfer.temporaries.insert(slfer.temporaries.end(), proper.temporaries.begin(), proper.temporaries.end());

    if (proper.is_const_result) {
        const field_descriptor* pfield = nullptr;

        // retrieve index from the property argument
        entity const& proper_entity = get_entity(u, proper.value());
        intptr_t index;
        if (auto int_lit = dynamic_cast<const integer_literal_entity*>(&proper_entity)) {
            index = static_cast<intptr_t>(int_lit->value());
        } else {
            return std::unexpected(make_error<type_mismatch_error>(tmd.call_location, proper.value(), "an integer"sv));
        }
        if (index < 0) {
            return std::unexpected(make_error<basic_general_error>(tmd.call_location, "index cannot be negative"sv));
        }

        intptr_t cur = 0;
        auto frng = tmd.orig_tuple_sig.find_fields(tmd.project_name);
        for (auto it = frng.first, eit = frng.second; it != eit; ++it, ++cur) {
            if (cur != index) continue;
            pfield = &tmd.orig_tuple_sig.field(it->second);
            break;
        }

        if (!pfield) {
            return std::unexpected(make_error<basic_general_error>(tmd.call_location, "tuple_project index out of range"sv, proper.value()));
        }

        // Case 1: Source tuple element is const
        if (pfield->is_const()) {
            return syntax_expression_result_t{
                .temporaries = std::move(slfer.temporaries),
                .expressions = md.merge_void_spans(el),
                .value_or_type = pfield->entity_id(),
                .is_const_result = true
            };
        }

        // Case 2: self is not constant, property is constant
        
        // Count non-const fields
        size_t non_const_count = 0; 
        // Compute runtime index among non-const fields
        size_t runtime_index = 0;

        auto sp = tmd.orig_tuple_sig.fields();
        for (auto it = sp.begin(), eit = sp.end(); it != eit; ++it) {
            const auto& f = *it;
            if (!f.is_const()) {
                ++non_const_count;
                if (pfield != &f) {
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
                .value_or_type = pfield->entity_id(),
                .is_const_result = false
            };
        }

        semantic::expression_span exprs = md.merge_void_spans(el);
        exprs = el.concat(exprs, slfer.expressions);
        u.push_back_expression(el, exprs, semantic::push_value{ smart_blob{ ui64_blob_result(runtime_index) } });
        u.push_back_expression(el, exprs, semantic::invoke_function(u.get(builtin_eid::array_at)));

        return syntax_expression_result_t{
            .temporaries = std::move(slfer.temporaries),
            .stored_expressions = std::move(slfer.stored_expressions),
            .expressions = std::move(exprs),
            .value_or_type = pfield->entity_id(),
            .is_const_result = false
        };
    }
    
    // Case 3: property is not constant (more complex scenarios not implemented yet)
    return std::unexpected(make_error<basic_general_error>(tmd.call_location, "tuple_project_get pattern with non-constant property index is not implemented yet"sv));
}

} // namespace sonia::lang::bang
