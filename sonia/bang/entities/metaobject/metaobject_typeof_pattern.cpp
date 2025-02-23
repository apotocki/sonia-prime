//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "typeof_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> metaobject_typeof_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    unit& u = ctx.u();
    identifier objid = u.get(builtin_id::object);
    identifier propid = u.get(builtin_id::property);
    named_expression_t const* object_arg = nullptr;
    named_expression_t const* property_arg = nullptr;

    for (auto const& arg : call.args()) {
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
        return std::unexpected(make_error<basic_general_error>(call.location(), "unmatched parameter"sv, objid));
    }
    if (!property_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "unmatched parameter"sv, propid));
    }

    ct_expression_visitor eobjvis{ ctx, annotated_entity_identifier{ u.get(builtin_eid::metaobject), object_arg->name()->location } };
    auto obj = apply_visitor(eobjvis, object_arg->value());
    if (!obj) return std::unexpected(std::move(obj.error()));

    ct_expression_visitor epropvis{ ctx, annotated_entity_identifier{ u.get(builtin_eid::identifier), property_arg->name()->location } };
    auto prop = apply_visitor(epropvis, property_arg->value());
    if (!prop) return std::unexpected(std::move(prop.error()));

    auto pmd = make_shared<functional_match_descriptor>(u);
    pmd->get_match_result(objid).append_result(false, *obj);
    pmd->get_match_result(propid).append_result(false, *prop);
    pmd->location = call.location();
    return pmd;
}

std::expected<entity_identifier, error_storage> metaobject_typeof_pattern::const_apply(fn_compiler_context& ctx, qname_identifier functional_id, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    entity_identifier mobj = md.get_match_result(u.get(builtin_id::object)).result.front();
    entity_identifier prop = md.get_match_result(u.get(builtin_id::property)).result.front();

    entity const& metaobject_ent = u.eregistry_get(mobj);
    entity_signature const* objsignature = metaobject_ent.signature();
    BOOST_ASSERT(objsignature);
    BOOST_ASSERT(objsignature->name == u.get(builtin_qnid::metaobject));
    
    identifier_entity const& prop_ent = static_cast<identifier_entity const&>(u.eregistry_get(prop));

    auto const* fd = objsignature->find_field(prop_ent.value());
    if (!fd) {
        return std::unexpected(make_error<basic_general_error>(md.location, "undefined property"sv, prop_ent.value()));
    }
    return fd->entity_id();
}

}
