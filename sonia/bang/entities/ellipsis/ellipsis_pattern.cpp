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
std::expected<int, error_storage> ellipsis_pattern::is_matched(fn_compiler_context& ctx, pure_call_t const& call, functional::match_descriptor& md) const
{
    if (!call.named_args.empty()) {
        return std::unexpected(make_error<basic_general_error>(std::get<0>(call.named_args.front()).location, "unexpected named argument"sv));
    }
    if (call.positioned_args.empty()) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "an argument is expected"sv));
    }
    if (call.positioned_args.size() != 1) {
        return std::unexpected(make_error<basic_general_error>(get_start_location(call.positioned_args[1]), "unexpected argument"sv));
    }
    auto const& expr = call.positioned_args.front();

    expression_visitor evis{ ctx, expected_result_t{ ctx.u().get_typename_entity_identifier(), get_start_location(expr) } };
    if (auto res = apply_visitor(evis, expr); !res) return std::unexpected(std::move(res.error()));

    md.signature.set_name(ctx.u().get_ellipsis_qname_identifier());

    std::span<semantic::expression_t> args = ctx.expressions();
    semantic::push_value const* pv = get<semantic::push_value>(&args.back());
    BOOST_ASSERT(pv);
    entity_identifier const* peid = get<entity_identifier>(&pv->value);
    BOOST_ASSERT(peid); // must be entity_identifier
    md.signature.push({*peid, true});
    return 0;
}

std::expected<entity_identifier, error_storage> ellipsis_pattern::const_apply(fn_compiler_context& ctx, functional::match_descriptor& md) const
{
    BOOST_ASSERT(md.signature.named_fields().empty());
    BOOST_ASSERT(md.signature.positioned_fields().size() == 1);

    entity const& entres = ctx.u().eregistry().find_or_create(indirect_signatured_entity{ md.signature }, [&ctx, &md]() {
        return make_shared<pack_entity>(ctx.u().get_typename_entity_identifier(), std::move(md.signature));
    });
    ctx.pop_chain();
    return entres.id();
}

}
