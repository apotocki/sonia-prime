//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_make_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/ast/expression_visitor.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> tuple_make_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const&) const
{
    unit& u = ctx.u();
    entity_identifier veid = u.get(builtin_eid::void_);
    size_t pos_arg_num = 0;
    auto pmd = make_shared<functional_match_descriptor>();
    for (auto const& arg : call.args) {
        auto res = apply_visitor(base_expression_visitor{ ctx }, arg.value());
        if (!res) return std::unexpected(std::move(res.error()));
        auto& ser = res->first;
        if (ser.is_const_result && ser.value() == veid) {
            if (ser.expressions) pmd->void_spans.emplace_back(ser.expressions);
            continue;
        }

        annotated_identifier const* pargname = arg.name();
        parameter_match_result* pmr = pargname ? &pmd->get_match_result(pargname->value) : &pmd->get_match_result(pos_arg_num++);
        
        pmr->append_result(ser);
        call.splice_back(ser.expressions);
    }
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> tuple_make_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    entity_signature sig = md.build_signature(u, u.get(builtin_qnid::tuple));
    sig.result.emplace(u.get(builtin_eid::typename_));
    indirect_signatured_entity smpl{ sig };

    entity& e = ctx.u().eregistry_find_or_create(smpl, [&sig]() {
        return make_shared<basic_signatured_entity>(std::move(sig));
    });
    BOOST_ASSERT(e.signature() && e.signature()->name == u.get(builtin_qnid::tuple));

    size_t argcount = 0;
    
    // push call expressions in the right order
    semantic::managed_expression_list exprs{ u };

    md.for_each_named_match([&argcount, &exprs, &el](identifier name, parameter_match_result const& mr) {
        for (auto const& ser : mr.results) {
            exprs.splice_back(el, ser.expressions);
            if (!ser.is_const_result) ++argcount;
        }
    });
    md.for_each_positional_match([&argcount, &exprs, &el](parameter_match_result const& mr) {
        for (auto const& ser : mr.results) {
            exprs.splice_back(el, ser.expressions);
            if (!ser.is_const_result) ++argcount;
        }
    });
    
    if (argcount > 1) {
        u.push_back_expression(exprs, semantic::push_value{ mp::integer{ argcount } });
        u.push_back_expression(exprs, semantic::invoke_function(u.get(builtin_eid::arrayify)));
    }

    return syntax_expression_result_t{ std::move(exprs), e.id() };
}

}
