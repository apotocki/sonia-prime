//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "metaobject_bit_and_pattern.hpp"

#include <boost/container/flat_map.hpp>
#include "sonia/small_vector.hpp"

#include "sonia/bang/entities/generic_pattern_base.ipp"

namespace sonia::lang::bang {

class metaobject_bit_and_match_descriptor : public functional_match_descriptor
{
public:
    using functional_match_descriptor::functional_match_descriptor;

    small_vector<entity_signature const*, 2> arguments;
};

error_storage metaobject_bit_and_pattern::accept_argument(std::nullptr_t, functional_match_descriptor_ptr& pmd, arg_context_type& argctx, syntax_expression_result_t& er) const
{
    if (argctx.pargname || er.first) return argctx.make_argument_mismatch_error();
    fn_compiler_context& ctx = argctx.ctx;
    prepared_call const& call = argctx.call;
    unit& u = ctx.u();
    entity_identifier argtype;
    entity_identifier eid = er.second;

    entity const& arg_entity = u.eregistry_get(eid);
    if (auto psig = arg_entity.signature(); psig && psig->name == u.get(builtin_qnid::metaobject)) {
        // argument is typename metaobject
        if (!pmd) {
            pmd = make_shared<metaobject_bit_and_match_descriptor>(u);
        }
        metaobject_bit_and_match_descriptor& md = static_cast<metaobject_bit_and_match_descriptor&>(*pmd);
        md.arguments.push_back(psig);
        return {};
    }
    return argctx.make_argument_mismatch_error();
}

std::expected<syntax_expression_result_t, error_storage> metaobject_bit_and_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& tmd = static_cast<metaobject_bit_and_match_descriptor&>(md);

    boost::container::small_flat_multimap<identifier, entity_identifier, 16> named_args;
    small_vector<entity_identifier, 16> pos_args;
    for (entity_signature const* sig : tmd.arguments) {
        for (auto [name, idx] : sig->named_fields_indices()) {
            named_args.emplace(name, sig->get_field(idx)->entity_id());
        }
        for (auto idx : sig->positioned_fields_indices()) {
            pos_args.push_back(sig->get_field(idx)->entity_id());
        }
    }

    entity_signature sig{ u.get(builtin_qnid::metaobject), u.get(builtin_eid::metaobject) };
    for (auto [name, eid] : named_args) {
        sig.push_back(name, field_descriptor{ eid, true });
    }
    for (entity_identifier eid : pos_args) {
        sig.push_back(field_descriptor{ eid, true });
    }

    indirect_signatured_entity smpl{ sig };

    return make_result(u, u.eregistry_find_or_create(smpl, [&u, &sig]() {
        return make_shared<basic_signatured_entity>(std::move(sig));
    }).id());
}

template generic_pattern_base<metaobject_bit_and_pattern>;

}
