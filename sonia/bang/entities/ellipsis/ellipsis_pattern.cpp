//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "ellipsis_pattern.hpp"

#include "sonia/bang/entities/entity_signature.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "pack_entity.hpp"

namespace sonia::lang::bang {

// opeator ...(typename)
error_storage ellipsis_pattern::is_matched(fn_compiler_context& ctx, pure_call_t& call) const
{
    if (!call.named_args.empty()) {
        return make_error<basic_general_error>(std::get<0>(call.named_args.front()).location, "unexpected named argument"sv);
    }
    if (call.positioned_args.empty()) {
        return make_error<basic_general_error>(call.location(), "an argument is expected"sv);
    }
    if (call.positioned_args.size() != 1) {
        return make_error<basic_general_error>(get_start_location(call.positioned_args[1]), "unexpected argument"sv);
    }
    auto const& expr = call.positioned_args.front();

    expression_visitor evis{ ctx, expected_result_t{ ctx.u().get_typename_entity_identifier(), get_start_location(expr) } };
    if (auto opterr = apply_visitor(evis, expr); opterr) return std::move(opterr);

    return {};
}

std::expected<entity_identifier, error_storage> ellipsis_pattern::const_apply(fn_compiler_context& ctx) const
{
    std::span<semantic::expression_t> args = ctx.expressions();
    for (;;) {
        if (args.size() != 1) break;
        semantic::push_value const* pv = get<semantic::push_value>(&args.front());
        if (!pv) break;
        entity_identifier const* eid = get<entity_identifier>(&pv->value);
        BOOST_ASSERT(eid); // must be entity_identifier
        
        // signature: ellipsis_qname, string_entity_id
        pack_entity smpl{ ctx.u().get_ellipsis_qname_identifier(), *eid, ctx.u().get_typename_entity_identifier() };
        
        entity const& entres = ctx.u().eregistry().find_or_create(smpl, [&ctx, eid, &smpl]() {
            // create entity for pack
            return make_shared<pack_entity>(std::move(smpl));
        });
        ctx.pop_chain();
        return entres.id();
    }

    THROW_NOT_IMPLEMENTED_ERROR("ellipsis_pattern::apply, complex argument");
}

}
