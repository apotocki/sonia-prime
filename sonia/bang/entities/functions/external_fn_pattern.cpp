//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "external_fn_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/functions/function_entity.hpp"

namespace sonia::lang::bang {

std::expected<entity_identifier, error_storage> external_fn_pattern::const_apply(fn_compiler_context& ctx, functional::match_descriptor&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("external_fn_pattern::const_apply");
}

//std::expected<entity_identifier, error_storage> external_fn_pattern::apply(fn_compiler_context& ctx, functional::match_descriptor&) const
//{
//    THROW_NOT_IMPLEMENTED_ERROR("external_fn_pattern::apply");
//#if 0
//    ctx.append_expression(semantic::invoke_external_function{ extfnid_ });
//    ctx.pop_chain(); // function call chain
//    pattern_expression_t const* r = fd_.result_type();
//    BOOST_ASSERT(r);
//
//    return fd_.result_type();
//#endif
//}

shared_ptr<entity> external_fn_pattern::build(unit& u, functional::match_descriptor& md) const
{
    BOOST_ASSERT(md.result);
    auto pfe = make_shared<external_function_entity>(fn_qname_id(), std::move(md.signature), extfnid_);
    pfe->build_fn_signature(u, md.result);
    return pfe;
    //THROW_NOT_IMPLEMENTED_ERROR("external_fn_pattern::build");
}

}