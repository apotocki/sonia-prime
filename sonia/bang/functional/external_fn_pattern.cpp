//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "external_fn_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/functions/function_entity.hpp"

namespace sonia::lang::bang {

#if 0
std::expected<entity_identifier, error_storage> external_fn_pattern::apply(fn_compiler_context& ctx, functional::match_descriptor&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("external_fn_pattern::apply");
#if 0
    ctx.append_expression(semantic::invoke_function{ extfnid_ });
    ctx.pop_chain(); // function call chain
    pattern_expression_t const* r = fd_.result_type();
    BOOST_ASSERT(r);

    return fd_.result_type();
#endif
}
#endif

shared_ptr<entity> external_fn_pattern::build(fn_compiler_context& ctx, functional_match_descriptor&, entity_signature&& signature) const
{
    unit& u = ctx.u();
    return make_shared<external_function_entity>(u, fn_qname() / u.new_identifier(), std::move(signature), extfnid_);
}

}
