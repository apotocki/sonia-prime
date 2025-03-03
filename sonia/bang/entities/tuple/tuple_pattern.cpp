//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_pattern.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
//#include "pack_entity.hpp"

namespace sonia::lang::bang {

// operator ...(typename)
std::expected<functional_match_descriptor_ptr, error_storage> tuple_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("tuple_pattern::try_match");
#if 0
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
#endif
    
}

std::expected<entity_identifier, error_storage> tuple_pattern::const_apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    THROW_NOT_IMPLEMENTED_ERROR("tuple_pattern::const_apply");
#if 0
    BOOST_ASSERT(md.signature.named_fields().empty());
    BOOST_ASSERT(md.signature.positioned_fields().size() == 1);

    entity const& entres = ctx.u().eregistry_find_or_create(indirect_signatured_entity{ md.signature }, [&ctx, &md]() {
        return make_shared<pack_entity>(ctx.u().get_typename_entity_identifier(), std::move(md.signature));
    });
    ctx.pop_chain();
    return entres.id();
#endif
}

}
