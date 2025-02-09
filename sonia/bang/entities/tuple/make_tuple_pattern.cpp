//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "make_tuple_pattern.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> make_tuple_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    size_t pos_arg_num = 0;
    auto pmd = make_shared<functional_match_descriptor>(ctx.u());
    auto estate = ctx.expressions_state();
    ctx.push_chain(pmd->call_expressions);
    for (auto const& arg : call.args()) {
        auto last_expr_it = ctx.expressions().last();
        expression_visitor evis{ ctx };
        auto res = apply_visitor(evis, arg.value());
        if (!res) return std::unexpected(std::move(res.error()));

        annotated_identifier const* pargname = arg.name();
        parameter_match_result* pmr = pargname ? &pmd->get_match_result(pargname->value) : &pmd->get_match_result(pos_arg_num++);
        pmr->append_result(false, ctx.context_type, last_expr_it, ctx.expressions());
    }
    return pmd;
}

error_storage make_tuple_pattern::apply(fn_compiler_context& ctx, qname_identifier functional_id, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    entity_signature sig = md.build_signature(u, u.get(builtin_qnid::tuple));
    indirect_signatured_entity smpl{ sig };

    entity& e = ctx.u().eregistry_find_or_create(smpl, [this, &u, &sig]() {
        return make_shared<basic_signatured_entity>(u.get(builtin_eid::typename_), std::move(sig));
    });
    BOOST_ASSERT(e.signature() && e.signature()->name() == u.get(builtin_qnid::tuple));

    size_t argcount = 0;
    
    // push call expressions in the right order
    semantic::managed_expression_list args{ u };

    md.for_each_named_match([&argcount, &args, &md](identifier name, parameter_match_result const& mr) {
        for (auto rng : mr.expressions) {
            ++rng.second;
            args.splice_back(md.call_expressions, rng.first, rng.second);
        }
        ++argcount;
    });
    md.for_each_positional_match([&argcount, &args, &md](parameter_match_result const& mr) {
        for (auto rng : mr.expressions) {
            ++rng.second;
            args.splice_back(md.call_expressions, rng.first, rng.second);
        }
        ++argcount;
    });
    BOOST_ASSERT(!md.call_expressions); // all arguments were transfered
    if (argcount > 1) {
        u.push_back_expression(args, semantic::push_value{ mp::integer{ argcount } });
        u.push_back_expression(args, semantic::invoke_function(u.get(builtin_eid::arrayify)));
    }
    ctx.append_expression(ctx.store_semantic_expressions(std::move(args)));
    ctx.context_type = e.id();
    return {};
}

std::expected<entity_identifier, error_storage> make_tuple_pattern::const_apply(fn_compiler_context& ctx, qname_identifier, functional_match_descriptor& md) const
{
    THROW_NOT_IMPLEMENTED_ERROR("make_tuple_pattern::const_apply");
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
