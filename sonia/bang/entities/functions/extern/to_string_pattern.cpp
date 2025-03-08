//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "to_string_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/functions/function_entity.hpp"

namespace sonia::lang::bang {

std::expected<functional::pattern::application_result_t, error_storage> to_string_pattern::generic_apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& [strid, _] = md.get_match_result(0).results.front();
    entity const& ent = u.eregistry_get(strid);
    identifier_entity const* pie = dynamic_cast<identifier_entity const*>(&ent);
    BOOST_ASSERT(pie);
    string_literal_entity sle{ ctx.u().print(pie->value()) };
    return u.eregistry_find_or_create(sle, [&u, &sle]() {
        auto result = make_shared<string_literal_entity>(std::move(sle));
        result->set_type(u.get(builtin_eid::string));
        return result;
    }).id();
}

}
