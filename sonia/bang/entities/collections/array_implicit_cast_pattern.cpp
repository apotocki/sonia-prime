//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "array_implicit_cast_pattern.hpp"

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
        apply_visitor(make_functional_visitor<void>([&u, &ct_element_results, &rt_element_results, &optexprs](auto & v) {
            if constexpr (std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
                if (rt_element_results.empty()) {
                    ct_element_results.push_back(v);
                } else {
                    u.push_back_expression(*optexprs, semantic::push_value{ v });
                    rt_element_results.emplace_back(&optexprs->back_entry());
                }
            } else {
                semantic::expression_span argspan = v;
                if (!optexprs) {
                    optexprs.emplace(std::move(v));
                    if (!ct_element_results.empty()) {
                        for (entity_identifier eid : ct_element_results) {
                            u.push_back_expression(*optexprs, semantic::push_value{ eid });
                            rt_element_results.emplace_back(&optexprs->back_entry());
                        }
                    }
                }
                rt_element_results.push_back(std::move(argspan));
                optexprs->splice_back(v);
            }
        }), res->first);
    }

    if (rt_element_results.empty()) {
        entity const& vec_ent = u.make_vector_entity(vec_element_type_eid, ct_element_results);
        mr.append_result(vec_ent.id());
        return {};
    }

    THROW_NOT_IMPLEMENTED_ERROR("array_implicit_cast_check_argument_type");
}

error_storage array_implicit_cast_check_argument_type(fn_compiler_context& ctx, semantic::managed_expression_list & argel, lex::resource_location const& argloc, entity_identifier vec_element_type_eid, annotated_entity_identifier const& vec_type, functional_match_descriptor& md)
{
    unit& u = ctx.u();
    
    entity const& argtype_ent = get_entity(u, ctx.context_type);
    entity_signature const* ptpsig = argtype_ent.signature();
    if (!ptpsig || ptpsig->name != u.get(builtin_qnid::array)) {
        return make_error<type_mismatch_error>(argloc, ctx.context_type, "an array"sv);
    }
    
    parameter_match_result& mr = md.get_match_result(0);
    md.call_expressions = std::move(argel);
    mr.append_result(ctx.context_type, md.call_expressions.end(), md.call_expressions);

    entity_identifier elem_type_eid = ptpsig->find_field(u.get(builtin_id::element))->entity_id();
    if (elem_type_eid == vec_element_type_eid) return {};

    // check if element cast exists
    pure_call_t cast_call{ vec_type.location };
    cast_call.emplace_back(context_value{ elem_type_eid, argloc });

    auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, annotated_entity_identifier{ vec_element_type_eid });
    if (!match) {
        return make_error<cast_error>(vec_type.location, ctx.context_type, vec_type.value);
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

        auto err = apply_visitor(make_functional_visitor<error_storage>(
            [&ctx, &e, &pmd, argloc = get_start_location(argexpr), vec_element_type_eid]
                (auto& v) -> error_storage {
                    return array_implicit_cast_check_argument_type(ctx, v, argloc, vec_element_type_eid, e, *pmd);
                }
            ), res->first);
        if (err) return std::unexpected(std::move(err));
    }

    if (!pmd) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter $0"sv));
    }
    pmd->result = field_descriptor{ e.value };
    return pmd;
}

std::expected<functional::pattern::application_result_t, error_storage> array_implicit_cast_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    semantic::managed_expression_list exprs{ u };
    parameter_match_result const& mr = md.get_match_result(0);
    auto const& [eid, optspan] = mr.results.front();
    if (optspan) {
        ctx.context_type = md.result.entity_id();
        //THROW_NOT_IMPLEMENTED_ERROR("array_implicit_cast_pattern::apply");
        return std::move(md.call_expressions);
    } else {
        return eid;
    }
}

}
