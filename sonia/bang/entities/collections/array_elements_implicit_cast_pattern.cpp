//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "array_elements_implicit_cast_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/ast/base_expression_visitor.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"
#include "sonia/bang/errors/value_mismatch_error.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> array_elements_implicit_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& e) const
{
    unit& u = ctx.u();
    if (!e) {
        return std::unexpected(make_error<basic_general_error>(call.location, "expected an array result"sv));
    }
    entity const& ent = u.eregistry_get(e.value);
    entity_signature const* psig = ent.signature();
    if (!psig || psig->name != u.get(builtin_qnid::array)) {
        return std::unexpected(make_error<type_mismatch_error>(e.location, e.value, "an array"sv));
    }
    field_descriptor const* peld = psig->find_field(u.get(builtin_id::element));
    field_descriptor const* pszd = psig->find_field(u.get(builtin_id::size));
    BOOST_ASSERT(peld);
    BOOST_ASSERT(pszd);
    entity_identifier arr_element_type_eid = peld->entity_id();
    size_t arrsz = (size_t)static_cast<integer_literal_entity const&>(u.eregistry_get(pszd->entity_id())).value();

    functional_match_descriptor_ptr pmd;

    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        auto const& argexpr = arg.value();
        if (pargname) { // named arguments are not expected
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch"sv, argexpr));
        }
        auto res = apply_visitor(base_expression_visitor{ ctx }, argexpr);
        if (!res) return std::unexpected(std::move(res.error()));
        auto arg_loc = get_start_location(argexpr);
        auto& ser = res->first;
        if (ser.is_const_result) {
            if (pmd) {
                return std::unexpected(make_error<basic_general_error>(arg_loc, "argument mismatch"sv, argexpr));
            }
            entity const& ent = get_entity(u, ser.value());
            // ent should be a metaobject to accept, its type should be an array
            entity const& type_ent = u.eregistry_get(ent.get_type());
            entity_signature const* ptypesig = type_ent.signature();
            if (!ptypesig || ptypesig->name != u.get(builtin_qnid::array)) {
                return std::unexpected(make_error<type_mismatch_error>(arg_loc, ent.id, "an array"sv));
            }
            field_descriptor const* pargszd = ptypesig->find_field(u.get(builtin_id::size));
            BOOST_ASSERT(pargszd);
            size_t arg_arr_sz = (size_t)static_cast<integer_literal_entity const&>(u.eregistry_get(pargszd->entity_id())).value();
            if (arg_arr_sz != arrsz) {
                return std::unexpected(make_error<value_mismatch_error>(arg_loc, ent.id, "an array of the same size"sv));
            }
            entity_signature const* pargsig =  ent.signature();
            BOOST_ASSERT(pargsig);

            small_vector<entity_identifier, 16> ct_element_results;
            ct_expression_visitor cast_vis{ ctx, annotated_entity_identifier{ arr_element_type_eid, e.location} };
            for (size_t i = 0; i < arrsz; ++i) {
                field_descriptor const* pargeld = pargsig->find_field(i);
                BOOST_ASSERT(pargeld);
                auto res = cast_vis(annotated_entity_identifier{ pargeld->entity_id(), arg_loc });
                if (!res) return std::unexpected(std::move(res.error()));
                if (res->expressions) THROW_NOT_IMPLEMENTED_ERROR("array_elements_implicit_cast_pattern::try_match");
                ct_element_results.push_back(res->value);
            }

            pmd = make_shared<functional_match_descriptor>();
            pmd->result = field_descriptor{ u.make_array_entity(arr_element_type_eid, ct_element_results).id, true };
        } else {
            return std::unexpected(make_error<basic_general_error>(arg_loc, "argument mismatch"sv, argexpr));
        }
    }

    if (!pmd) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter $0"sv));
    }
    return pmd;
}


std::expected<syntax_expression_result_t, error_storage> array_elements_implicit_cast_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t&, functional_match_descriptor& md) const
{
    return make_result(ctx.u(), md.result.entity_id());
}

}
