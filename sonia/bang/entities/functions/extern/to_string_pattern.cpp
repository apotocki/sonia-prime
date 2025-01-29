//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "to_string_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/functions/function_entity.hpp"

namespace sonia::lang::bang {

error_storage to_string_pattern::apply(fn_compiler_context& ctx, qname_identifier fid, functional_match_descriptor& md) const
{
    auto r = const_apply(ctx, fid, md);
    if (!r) return std::move(r.error());
    ctx.append_expression(semantic::push_value{ *r });
    entity const& e = ctx.u().eregistry_get(*r);
    ctx.context_type = e.get_type();
    return {};
}

std::expected<entity_identifier, error_storage> to_string_pattern::const_apply(fn_compiler_context& ctx, qname_identifier fid, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    entity_identifier strid = md.get_match_result(0).result.front();
    entity const& ent = u.eregistry_get(strid);
    if (identifier_entity const* pie = dynamic_cast<identifier_entity const*>(&ent); pie) {
        string_literal_entity sle{ ctx.u().print(pie->value()) };
        return u.eregistry_find_or_create(sle, [&u, &sle]() {
            auto result = make_shared<string_literal_entity>(std::move(sle));
            result->set_type(u.get(builtin_eid::string));
            return result;
        }).id();
    }
    THROW_NOT_IMPLEMENTED_ERROR("to_string_pattern::const_apply");
}

}
