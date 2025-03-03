//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "metaobject_empty_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"

namespace sonia::lang::bang {

std::expected<entity_identifier, error_storage> metaobject_empty_pattern::const_apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    entity_signature const& objsignature = argument_signature(ctx, md);
    return ctx.u().get(objsignature.fields().empty() ? builtin_eid::true_ : builtin_eid::false_);
}

}
