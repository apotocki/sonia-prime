//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "array_implicit_cast_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"

#include "sonia/bang/auxiliary.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/ast/base_expression_visitor.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

//error_storage array_implicit_cast_check_argument_type(fn_compiler_context& ctx, annotated_entity_identifier const& argtype, annotated_entity_identifier vec_elem_type, parameter_match_result& mr)
error_storage array_implicit_cast_check_argument_type(fn_compiler_context& ctx, entity_identifier& argeid, lex::resource_location const& argloc, entity_identifier vec_element_type_eid, annotated_entity_identifier const& vec_type, functional_match_descriptor& md)
{
    unit& u = ctx.u();
    parameter_match_result& mr = md.get_match_result(0);
    entity const& ent = get_entity(u, argeid);
    entity_signature const* psig = ent.signature();

    auto wrong_arg_error = [&argeid, &argloc] { return make_error<type_mismatch_error>(argloc, argeid, "an array"sv); };

    if (!psig || psig->name != u.get(builtin_qnid::metaobject) || !psig->result) {
        return wrong_arg_error();
    }
    entity const& argtype_ent = get_entity(u, psig->result->entity_id());
    if (entity_signature const* ptpsig = argtype_ent.signature(); !ptpsig || ptpsig->name != u.get(builtin_qnid::array)) {
        return wrong_arg_error();
    }

    optional<semantic::managed_expression_list> optexprs;
    small_vector<entity_identifier, 16> ct_element_results;
    small_vector<semantic::expression_span, 16> rt_element_results;

    size_t elem_count = psig->positioned_fields_indices().size();
    base_expression_visitor vis{ ctx, annotated_entity_identifier{ vec_element_type_eid } };
    for (size_t i = 0; i < elem_count; ++i) {
        entity_identifier elem_eid = psig->find_field(i)->entity_id();
        auto res = vis(annotated_entity_identifier{ elem_eid, argloc });
        if (!res) return std::move(res.error());
        auto& [el, reid] = res->first;
        if (!el) {
            if (rt_element_results.empty()) {
                ct_element_results.push_back(reid);
            } else {
                u.push_back_expression(*optexprs, semantic::push_value{ reid });
                rt_element_results.emplace_back(&optexprs->back_entry());
            }
        } else {
            semantic::expression_span argspan = el;
            if (!optexprs) {
                optexprs.emplace(std::move(el));
                if (!ct_element_results.empty()) {
                    for (entity_identifier eid : ct_element_results) {
                        u.push_back_expression(*optexprs, semantic::push_value{ eid });
                        rt_element_results.emplace_back(&optexprs->back_entry());
                    }
                }
            } else {
                optexprs->splice_back(el);
            }
            rt_element_results.push_back(std::move(argspan));
        }
    }

    if (rt_element_results.empty()) {
        entity const& vec_ent = u.make_vector_entity(vec_element_type_eid, ct_element_results);
        mr.append_result(vec_ent.id());
        return {};
    }

    THROW_NOT_IMPLEMENTED_ERROR("array_implicit_cast_check_argument_type");
}

error_storage array_implicit_cast_check_argument_type(fn_compiler_context& ctx, syntax_expression_result_t & er, lex::resource_location const& argloc, entity_identifier vec_element_type_eid, annotated_entity_identifier const& vec_type, functional_match_descriptor& md)
{
    semantic::managed_expression_list& argel = er.first;
    entity_identifier type = er.second;
    if (!argel) {
        return array_implicit_cast_check_argument_type(ctx, type, argloc, vec_element_type_eid, vec_type, md);
    }
    
    unit& u = ctx.u();
    
    entity const& argtype_ent = get_entity(u, type);
    entity_signature const* ptpsig = argtype_ent.signature();
    if (!ptpsig || ptpsig->name != u.get(builtin_qnid::array)) {
        return make_error<type_mismatch_error>(argloc, type, "an array"sv);
    }
    
    parameter_match_result& mr = md.get_match_result(0);
    md.call_expressions = std::move(argel);
    mr.append_result(type, md.call_expressions.end(), md.call_expressions);

    entity_identifier elem_type_eid = ptpsig->find_field(u.get(builtin_id::element))->entity_id();
    if (elem_type_eid == vec_element_type_eid) return {};

    // check if element cast exists
    pure_call_t cast_call{ vec_type.location };
    cast_call.emplace_back(indirect_value{
        .location = argloc,
        .type = elem_type_eid,
        .store = indirect_value_store_t{ in_place_type<semantic::indirect_expression_list>, std::move(md.call_expressions) }
    });

    auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, annotated_entity_identifier{ vec_element_type_eid });
    if (!match) {
        return make_error<cast_error>(vec_type.location, type, vec_type.value);
    }
    return {};
}


std::expected<functional_match_descriptor_ptr, error_storage> array_implicit_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& e) const
{
    if (!e) {
        return std::unexpected(make_error<basic_general_error>(call.location, "expected a vector result"sv));
    }

    unit& u = ctx.u();
    entity const& ent = get_entity(u, e);
    
    entity_signature const* psig = ent.signature();
    if (!psig || psig->name != u.get(builtin_qnid::vector)) {
        return std::unexpected(make_error<type_mismatch_error>(e.location, e.value, "a vector"sv));
    }
    entity_identifier vec_element_type_eid = psig->find_field(u.get(builtin_id::element))->entity_id();

    functional_match_descriptor_ptr pmd;

    for (auto const& arg : call.args) {
        if (annotated_identifier const* pargname = arg.name(); pargname) { // named arguments are not expected
            return std::unexpected(make_error<basic_general_error>(pargname->location, "unexpected argument"sv, *pargname));
        }
        auto const& argexpr = arg.value();
        if (pmd) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "argument mismatch"sv, argexpr));
        }
        auto res = apply_visitor(base_expression_visitor{ ctx }, argexpr);
        if (!res) return std::unexpected(std::move(res.error()));
        pmd = make_shared<functional_match_descriptor>(u);
        auto err = array_implicit_cast_check_argument_type(ctx, res->first, get_start_location(argexpr), vec_element_type_eid, e, *pmd);
        if (err) return std::unexpected(std::move(err));
    }

    if (!pmd) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter $0"sv));
    }
    pmd->result = field_descriptor{ e.value };
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> array_implicit_cast_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    semantic::managed_expression_list exprs{ u };
    parameter_match_result const& mr = md.get_match_result(0);
    auto const& [eid, optspan] = mr.results.front();
    if (optspan) {
        //THROW_NOT_IMPLEMENTED_ERROR("array_implicit_cast_pattern::apply");
        return syntax_expression_result_t{ std::move(md.call_expressions), md.result.entity_id() };
    } else {
        return make_result(u, eid);
    }
}

}
