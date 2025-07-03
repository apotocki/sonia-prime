//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "metaobject_typeof_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> metaobject_typeof_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();
    identifier objid = u.get(builtin_id::object);
    identifier propid = u.get(builtin_id::property);
    named_expression_t const* object_arg = nullptr;
    named_expression_t const* property_arg = nullptr;

    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        if (!pargname) {
            syntax_expression_t const& e = arg.value();
            return std::unexpected(make_error<basic_general_error>(get_start_location(e), "argument mismatch, unexpected argument"sv, e));
        }
        if (pargname->value == objid && !object_arg) {
            object_arg = &arg;
        } else if (pargname->value == propid && !property_arg) {
            property_arg = &arg;
        } else {
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch, unexpected argument"sv, arg.value()));
        }
    }

    if (!object_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter"sv, objid));
    }
    if (!property_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter"sv, propid));
    }

    ct_expression_visitor eobjvis{ ctx, call.expressions, expected_result_t{ u.get(builtin_eid::metaobject), true, object_arg->name()->location } };
    auto obj = apply_visitor(eobjvis, object_arg->value());
    if (!obj) return std::unexpected(std::move(obj.error()));

    ct_expression_visitor epropvis{ ctx, call.expressions, expected_result_t{ u.get(builtin_eid::identifier), true, property_arg->name()->location } };
    auto prop = apply_visitor(epropvis, property_arg->value());
    if (!prop) return std::unexpected(std::move(prop.error()));

    auto pmd = make_shared<functional_match_descriptor>(call);
    pmd->get_match_result(objid).append_result(*obj);
    pmd->get_match_result(propid).append_result(*prop);
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> metaobject_typeof_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();

    auto& obj = md.get_match_result(u.get(builtin_id::object)).results.front();
    auto& prop = md.get_match_result(u.get(builtin_id::property)).results.front();
    BOOST_ASSERT(!obj.expressions); // not impelemented const value expressions
    BOOST_ASSERT(!prop.expressions);// not impelemented const value expressions
    
    entity const& metaobject_ent = get_entity(u, obj.value());
    entity_signature const* objsignature = metaobject_ent.signature();
    BOOST_ASSERT(objsignature);
    BOOST_ASSERT(objsignature->name == u.get(builtin_qnid::metaobject));
    
    identifier_entity const& prop_ent = static_cast<identifier_entity const&>(get_entity(u, prop.value()));

    auto const* fd = objsignature->find_field(prop_ent.value());
    if (!fd) {
        return std::unexpected(make_error<basic_general_error>(md.call_location, "undefined property"sv, prop_ent.value()));
    }
    return syntax_expression_result_t{
        .expressions = md.merge_void_spans(el),
        .value_or_type = fd->entity_id(),
        .is_const_result = true
    };
}

}
