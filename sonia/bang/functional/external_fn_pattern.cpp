//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "external_fn_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/functions/function_entity.hpp"

namespace sonia::lang::bang {

std::expected<syntax_expression_result_t, error_storage> external_fn_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto [result, _] = apply_arguments(ctx, el, md);

    u.push_back_expression(el, result.expressions, semantic::invoke_function(ent_->id));

    return result;
}

//shared_ptr<entity> external_fn_pattern::build(fn_compiler_context& ctx, functional_match_descriptor&, entity_signature&& signature) const
//{
//    THROW_NOT_IMPLEMENTED_ERROR("external_fn_pattern::build");
//    //unit& u = ctx.u();
//    //return make_shared<external_function_entity>(u, fnl_.name() / u.new_identifier(), std::move(signature), extfnid_);
//}

}
