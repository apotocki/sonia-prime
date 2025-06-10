//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "metaobject_argument_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> metaobject_argument_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();
    syntax_expression_t const* object_arg = nullptr;

    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        if (pargname) {
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch, unexpected argument"sv, *pargname));
        }
        syntax_expression_t const& e = arg.value();
        if (object_arg) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(e), "argument mismatch, unexpected argument"sv, e));
        }
        object_arg = &e;
    }

    if (!object_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter"sv));
    }
    
    ct_expression_visitor eobjvis{ ctx, call.expressions, expected_result_t{ u.get(builtin_eid::metaobject), get_start_location(*object_arg) } };
    auto obj = apply_visitor(eobjvis, *object_arg);
    if (!obj) return std::unexpected(std::move(obj.error()));

    auto pmd = make_shared<functional_match_descriptor>(call.location);
    pmd->get_match_result(0).append_result(*obj);
    return pmd;
}

entity_signature const& metaobject_argument_pattern::argument_signature(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& ser = md.get_match_result(0).results.front();
    entity const& metaobject_ent = get_entity(u, ser.value());
    entity_signature const* objsignature = metaobject_ent.signature();
    BOOST_ASSERT(objsignature);
    BOOST_ASSERT(objsignature->name == u.get(builtin_qnid::metaobject));
    
    return *objsignature;
}

}
