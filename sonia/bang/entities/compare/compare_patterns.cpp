//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "compare_patterns.hpp"

#include "sonia/bang/entities/entity_signature.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"


namespace sonia::lang::bang {

// opeator ...(typename)
    std::expected<int, error_storage> eq_pattern::is_matched(fn_compiler_context& ctx, pure_call_t const& call, functional::match_descriptor&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("eq_pattern::is_matched");

    //if (!call.named_args.empty()) {
    //    return make_error<basic_general_error>(std::get<2>(call.named_args.front()), "unexpected named argument"sv);
    //}
    //if (call.positioned_args.size() != 2) {
    //    return make_error<basic_general_error>(call.location(), "two arguments are expected"sv);
    //}
    //auto const& tpl = call.positioned_args.front();

    //expression_visitor elvis{ ctx };
    //if (auto opterr = apply_visitor(elvis, std::get<0>(tpl)); opterr) return std::move(opterr);
    //entity_identifier l = ctx.context_type;

    //expression_visitor rvis{ ctx };

    //entity_identifier r = ctx.context_type;
    //expression_visitor evis{ ctx, expected_result_t{ ctx.u().get_typename_entity_identifier(), std::get<1>(tpl) } };
    //if (auto opterr = apply_visitor(evis, std::get<0>(tpl)); opterr) return std::move(opterr);

    //return {};

}

std::expected<entity_identifier, error_storage> eq_pattern::const_apply(fn_compiler_context& ctx, functional::match_descriptor&) const
{
    std::span<semantic::expression_t> args = ctx.expressions();
    THROW_NOT_IMPLEMENTED_ERROR("eq_pattern::const_apply");
#if 0
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
#endif
}

}
