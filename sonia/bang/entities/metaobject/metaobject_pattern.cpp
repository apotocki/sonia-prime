//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "metaobject_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> metaobject_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    size_t pos_arg_num = 0;
    auto pmd = make_shared<functional_match_descriptor>(ctx.u());
    for (auto const& arg : call.args()) {
        ct_expression_visitor evis{ ctx };
        auto res = apply_visitor(evis, arg.value());
        if (!res) return std::unexpected(std::move(res.error()));

        annotated_identifier const* pargname = arg.name();
        parameter_match_result* pmr = pargname ? &pmd->get_match_result(pargname->value) : &pmd->get_match_result(pos_arg_num++);
        pmr->append_result(*res);
    }
    return pmd;
}

std::expected<functional::pattern::application_result_t, error_storage> metaobject_pattern::generic_apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    return metaobject_pattern::const_apply(ctx, md);
}

std::expected<entity_identifier, error_storage> metaobject_pattern::const_apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    
    entity_signature sig = md.build_signature(u, u.get(builtin_qnid::metaobject));
    sig.result = field_descriptor{ u.get(builtin_eid::metaobject) };
    indirect_signatured_entity smpl{ sig };

    return u.eregistry_find_or_create(smpl, [&u, &sig]() {
        return make_shared<basic_signatured_entity>(std::move(sig));
    }).id();
}

}
