//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "ct_expression_visitor.hpp"

#include "fn_compiler_context.hpp"

namespace sonia::lang::bang {

inline unit& ct_expression_visitor::u() const noexcept { return ctx.u(); }

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_entity_identifier const& ee) const
{
    return ee.value;
}
    
}
