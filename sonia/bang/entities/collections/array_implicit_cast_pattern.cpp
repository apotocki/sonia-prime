//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "array_implicit_cast_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/ast/base_expression_visitor.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

error_storage array_implicit_cast_check_argument_type(fn_compiler_context& ctx, annotated_entity_identifier const& argtype, annotated_entity_identifier vec_elem_type, parameter_match_result& mr)
{
    unit& u = ctx.u();

    entity const& ent = u.eregistry_get(argtype.value);
    entity_signature const* psig = ent.signature();

    auto wrong_arg_error = [&argtype] { return make_error<type_mismatch_error>(argtype.location, argtype.value, "an array"sv); };

    if (!psig || psig->name != u.get(builtin_qnid::metaobject) || !psig->result) {
        return wrong_arg_error();
    }
    entity const& argtype_ent = u.eregistry_get(psig->result->entity_id());
    if (entity_signature const* ptpsig = argtype_ent.signature(); !ptpsig || ptpsig->name != u.get(builtin_qnid::array)) {
        return wrong_arg_error();
    }

    optional<semantic::managed_expression_list> optexprs;
    small_vector<entity_identifier, 16> ct_element_results;
    small_vector<semantic::expression_span, 16> rt_element_results;

    size_t elem_count = psig->positioned_fields_indices().size();
    base_expression_visitor vis{ ctx, vec_elem_type };
    for (size_t i = 0; i < elem_count; ++i) {
        entity_identifier elem_eid = psig->find_field(i)->entity_id();
        auto res = vis(annotated_entity_identifier{ elem_eid, argtype.location });
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
        entity const& vec_ent = u.make_vector_entity(vec_elem_type.value, ct_element_results);
        mr.append_result(vec_ent.id());
        return {};
    }

    THROW_NOT_IMPLEMENTED_ERROR("array_implicit_cast_check_argument_type");


}

std::expected<functional_match_descriptor_ptr, error_storage> array_implicit_cast_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const& e) const
{
    unit& u = ctx.u();
    if (!e) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "expected a vector result"sv));
    }
    entity const& ent = u.eregistry_get(e.value);
    entity_signature const* psig = ent.signature();
    if (!psig || psig->name != u.get(builtin_qnid::vector)) {
        return std::unexpected(make_error<type_mismatch_error>(e.location, e.value, "a vector"sv));
    }
    entity_identifier vec_element_type_eid = psig->find_field(u.get(builtin_id::element))->entity_id();

    functional_match_descriptor_ptr pmd;

    for (auto const& arg : call.args()) {
        annotated_identifier const* pargname = arg.name();
        auto const& argexpr = arg.value();
        if (pargname) { // named arguments are not expected
            auto res = apply_visitor(ct_expression_visitor{ ctx }, argexpr);
            if (!res || *res != u.get(builtin_eid::void_)) {
                return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch"sv, argexpr));
            }
            continue; // skip void argument
        }
        auto res = apply_visitor(base_expression_visitor{ ctx }, argexpr);
        if (!res) return std::unexpected(std::move(res.error()));
        if (auto const* peid = get<entity_identifier>(&res->first); peid) {
            if (*peid == u.get(builtin_eid::void_)) continue; // skip void argument
            if (pmd) {
                return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "argument mismatch"sv, argexpr));
            }

            pmd = make_shared<functional_match_descriptor>(u);
            auto err = array_implicit_cast_check_argument_type(ctx,
                annotated_entity_identifier{ *peid, get_start_location(argexpr) },
                annotated_entity_identifier{ vec_element_type_eid, e.location },
                pmd->get_match_result(0));

            if (err) return std::unexpected(std::move(err));

        } else {
            return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "argument mismatch"sv, argexpr));

            //if (ctx.context_type == u.get(builtin_eid::void_)) continue; // skip void argument
            //if (pmd) {
            //    return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "argument mismatch"sv, argexpr));
            //}
            //auto err = array_implicit_cast_check_argument_type(ctx, annotated_entity_identifier{ ctx.context_type, get_start_location(argexpr) }, e);
            //if (err) return std::unexpected(std::move(err));
            //
            //pmd = make_shared<functional_match_descriptor>(u);
            //semantic::managed_expression_list& il = get<semantic::managed_expression_list>(res->first);
            //pmd->call_expressions = std::move(il);
            //pmd->get_match_result(0).append_result(ctx.context_type, pmd->call_expressions.end(), pmd->call_expressions);
        }
    }

    if (!pmd) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "unmatched parameter $0"sv));
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
        return std::move(md.call_expressions);
    } else {
        return eid;
    }
}

}
