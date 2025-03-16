//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "struct_fn_pattern.hpp"
#include "struct_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/errors/circular_dependency_error.hpp"

namespace sonia::lang::bang {

struct_fn_pattern::struct_fn_pattern(functional const& fnl, variant<field_list_t, statement_span> const& body)
    : basic_fn_pattern{ fnl }
    , body_{ body }
{}

std::expected<functional::pattern::application_result_t, error_storage> struct_fn_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    entity_signature sig = md.build_signature(u, fnl_.id());
    sig.result.emplace(u.get(builtin_eid::typename_));
    indirect_signatured_entity smpl{ sig };

    return u.eregistry_find_or_create(smpl, [this, &ctx, &sig, &md]() {
        unit& u = ctx.u();
        qname struct_ns = fn_qname() / u.new_identifier();
        fn_compiler_context struct_ctx{ ctx, struct_ns };
        functional_binding_set bound_arguments;
        build_scope(struct_ctx, md, bound_arguments);
        BOOST_ASSERT(bound_arguments.empty());
        // u.fregistry().resolve(struct_ns).name() // do we need a functional to store qname?
        auto res = make_shared<struct_entity>(std::move(struct_ns), std::move(sig), body_);
        res->set_location(location());
        return res;
    }).id();
}

}
