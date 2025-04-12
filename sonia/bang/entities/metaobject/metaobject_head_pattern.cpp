//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "metaobject_head_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"

namespace sonia::lang::bang {

std::expected<syntax_expression_result_t, error_storage> metaobject_head_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    entity_signature const& objsignature = argument_signature(ctx, md);
    
    auto const* fd = objsignature.get_field(0);
    if (!fd) {
        return std::unexpected(make_error<basic_general_error>(md.location, "metaobject is empty"sv));
    }
    return make_result(ctx.u(), fd->entity_id());
}

}
