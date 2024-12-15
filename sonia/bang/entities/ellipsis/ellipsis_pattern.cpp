//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "ellipsis_pattern.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "pack_entity.hpp"

namespace sonia::lang::bang {

// operator ...(typename)
std::expected<functional_match_descriptor_ptr, error_storage> ellipsis_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    if (call.args().empty()) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "an argument is expected"sv));
    }
    auto const& opt_named_expr = call.args().front();
    if (annotated_identifier const* optname = opt_named_expr.name(); optname) {
        return std::unexpected(make_error<basic_general_error>(optname->location, "unexpected named argument"sv, optname->value));
    }

    if (call.args().size() != 1) {
        return std::unexpected(make_error<basic_general_error>(get_start_location(call.args()[1].value()), "unexpected argument"sv));
    }
    
    auto const& expr = opt_named_expr.value();

    expression_visitor evis{ ctx, annotated_entity_identifier{ ctx.u().get_typename_entity_identifier(), get_start_location(expr) } };
    if (auto res = apply_visitor(evis, expr); !res) return std::unexpected(std::move(res.error()));

    auto pmd = make_shared<functional_match_descriptor>(ctx.u());
    //pmd->signature.set_name(ctx.u().get_ellipsis_qname_identifier());

    auto& args = ctx.expressions();
    semantic::push_value const* pv = get<semantic::push_value>(&args.back());
    BOOST_ASSERT(pv);
    entity_identifier const* peid = get<entity_identifier>(&pv->value);
    BOOST_ASSERT(peid); // must be entity_identifier
    pmd->get_match_result(0).append_result(true, *peid);
    return pmd;
}

std::expected<entity_identifier, error_storage> ellipsis_pattern::const_apply(fn_compiler_context& ctx, qname_identifier fid, functional_match_descriptor& md) const
{
    entity_signature signature = md.build_signature(ctx.u(), fid);
    BOOST_ASSERT(signature.named_fields().empty());
    BOOST_ASSERT(signature.positioned_fields().size() == 1);

    entity const& entres = ctx.u().eregistry().find_or_create(indirect_signatured_entity{ signature }, [&ctx, &signature]() {
        return make_shared<pack_entity>(ctx.u().get_typename_entity_identifier(), std::move(signature));
    });
    ctx.pop_chain();
    return entres.id();
}

}
