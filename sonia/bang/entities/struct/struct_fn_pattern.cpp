//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "struct_fn_pattern.hpp"
#include "struct_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/entities/functions/internal_function_entity.hpp"
#include "sonia/bang/errors/circular_dependency_error.hpp"

namespace sonia::lang::bang {

struct_fn_pattern::struct_fn_pattern(variant<field_list_t, statement_span> const& body)
    : body_{ body }
{}

std::expected<syntax_expression_result_t, error_storage> struct_fn_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    md.signature.result.emplace(u.get(builtin_eid::typename_));
    indirect_signatured_entity smpl{ md.signature };

    entity const& struct_end = u.eregistry_find_or_create(smpl, [this, &ctx, &md]() {
        unit& u = ctx.u();
        functional& fnl = u.fregistry_resolve(md.signature.name);
        qname struct_ns = fnl.name() / u.new_identifier();
        //fn_compiler_context struct_ctx{ ctx, struct_ns };

        internal_function_entity fent{ qname{ struct_ns }, entity_signature{ md.signature }, statement_span{} };
        build_scope(u, md, fent);
        BOOST_ASSERT(fent.bindings.empty());
        // u.fregistry().resolve(struct_ns).name() // do we need a functional to store qname?
        auto res = sonia::make_shared<struct_entity>(std::move(struct_ns), std::move(md.signature), body_);
        res->location = location();
        return res;
    });

    return syntax_expression_result_t{
        .expressions = md.merge_void_spans(el),
        .value_or_type = struct_end.id,
        .is_const_result = true
    };
}

}
