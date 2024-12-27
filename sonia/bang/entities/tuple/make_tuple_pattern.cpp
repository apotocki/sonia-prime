//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "make_tuple_pattern.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "tuple_entity.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> make_tuple_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    size_t pos_arg_num = 0;
    auto pmd = make_shared<functional_match_descriptor>(ctx.u());
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

error_storage make_tuple_pattern::apply(fn_compiler_context& ctx, qname_identifier functional_id, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    entity_signature sig = md.build_signature(u, u.get_tuple_qname_identifier());
    indirect_signatured_entity smpl{ sig };

    entity& e = ctx.u().eregistry().find_or_create(smpl, [this, &u, &sig]() {
        return make_shared<tuple_entity>(u.get_typename_entity_identifier(), std::move(sig));
    });
    BOOST_ASSERT(dynamic_cast<tuple_entity*>(&e));

    size_t argcount = 0;
    // push call expressions in the right order
    ctx.append_expression(semantic::expression_list_t{});
    semantic::expression_list_t& args = get<semantic::expression_list_t&>(ctx.expressions().back());

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
        u.push_back_expression(args, semantic::invoke_function(u.get_arrayify_entity_identifier()));
    }
    ctx.context_type = e.id();
    return {};
}

std::expected<entity_identifier, error_storage> make_tuple_pattern::const_apply(fn_compiler_context& ctx, qname_identifier, functional_match_descriptor& md) const
{
    THROW_NOT_IMPLEMENTED_ERROR("make_tuple_pattern::const_apply");
#if 0
    BOOST_ASSERT(md.signature.named_fields().empty());
    BOOST_ASSERT(md.signature.positioned_fields().size() == 1);

    entity const& entres = ctx.u().eregistry().find_or_create(indirect_signatured_entity{ md.signature }, [&ctx, &md]() {
        return make_shared<pack_entity>(ctx.u().get_typename_entity_identifier(), std::move(md.signature));
    });
    ctx.pop_chain();
    return entres.id();
#endif
}

}
