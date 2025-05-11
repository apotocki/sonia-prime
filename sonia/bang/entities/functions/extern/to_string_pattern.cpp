//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "to_string_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/functions/function_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<syntax_expression_result_t, error_storage> to_string_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& ser = md.get_match_result(0).results.front();
    entity const& ent = get_entity(u, ser.value_or_type);
    identifier_entity const* pie = dynamic_cast<identifier_entity const*>(&ent);
    BOOST_ASSERT(pie);
    return syntax_expression_result_t{
        .expressions = md.merge_void_spans(el),
        .value_or_type = u.make_string_entity(u.print(pie->value())).id,
        .is_const_result = true
    };
}

}
