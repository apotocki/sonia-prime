//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "metaobject_tail_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"

namespace sonia::lang::bang {

std::expected<functional::pattern::application_result_t, error_storage> metaobject_tail_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();

    entity_signature const& objsignature = argument_signature(ctx, md);
    
    if (objsignature.fields().empty()) {
        return std::unexpected(make_error<basic_general_error>(md.location, "metaobject is empty"sv));
    }

    entity_signature tail_signature{ objsignature.name };
    tail_signature.result = objsignature.result;
    auto nids = objsignature.named_fields_indices();
    auto pids = objsignature.positioned_fields_indices();
    if (!pids.front()) pids = pids.subspan(1);
    for (uint32_t i = 1; i < objsignature.fields().size(); ++i) {
        bool is_positional = !pids.empty() && pids.front() == i;
        if (is_positional) {
            pids = pids.subspan(1);
            tail_signature.push_back(objsignature.fields()[i]);
        } else {
            auto nit = std::ranges::find(nids, i, &std::pair<identifier, uint32_t>::second);
            BOOST_ASSERT(nit != nids.end());
            tail_signature.push_back(nit->first, objsignature.fields()[i]);
        }
    }

    indirect_signatured_entity smpl{ tail_signature };

    return u.eregistry_find_or_create(smpl, [&u, &tail_signature]() {
        return make_shared<basic_signatured_entity>(std::move(tail_signature));
    }).id();
}

}
