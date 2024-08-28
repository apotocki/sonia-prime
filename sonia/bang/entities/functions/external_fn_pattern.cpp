//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "external_fn_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

namespace sonia::lang::bang {

std::expected<entity_identifier, error_storage> external_fn_pattern::const_apply(fn_compiler_context& ctx) const
{
    THROW_NOT_IMPLEMENTED_ERROR("external_fn_pattern::const_apply");
}

std::expected<entity_identifier, error_storage> external_fn_pattern::apply(fn_compiler_context& ctx) const
{
    ctx.append_expression(semantic::invoke_external_function{ extfnid_ });
    ctx.pop_chain(); // function call chain
    return fd_.result_type();
}

}
