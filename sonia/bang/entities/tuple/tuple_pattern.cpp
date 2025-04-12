//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_pattern.hpp"

#include "sonia/small_vector.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

namespace sonia::lang::bang {

class tuple_match_descriptor : public functional_match_descriptor
{
public:
    using functional_match_descriptor::functional_match_descriptor;
    
    small_vector<std::pair<annotated_identifier, entity_identifier>, 8> fields;
};

// tuple($args ... : _ ...)
std::expected<functional_match_descriptor_ptr, error_storage> tuple_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const&) const
{
    unit& u = ctx.u();
    entity_identifier veid = u.get(builtin_eid::void_);
    auto pmd = make_shared<tuple_match_descriptor>(u);

    for (auto const& arg : call.args) { // { argname, expr }
        annotated_identifier const* pargname = arg.name();
        auto res = apply_visitor(ct_expression_visitor{ ctx }, arg.value());
        if (!res) {
            return std::unexpected(append_cause(
                make_error<basic_general_error>(pargname ? pargname->location : get_start_location(arg.value()), "argument error"sv, arg.value()),
                std::move(res.error())
            ));
        }
        
        if (*res == veid) {
            continue; // ignore argument
        }
        pmd->fields.emplace_back(pargname ? *pargname : annotated_identifier{}, *res);
    }

    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> tuple_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    entity_identifier typename_eid = u.get(builtin_eid::typename_);
    auto& tmd = static_cast<tuple_match_descriptor&>(md);
    
    entity_signature result_signature{ u.get(builtin_qnid::tuple), typename_eid };
    
    for (auto const& [name, eid] : tmd.fields) {
        entity const& ent = u.eregistry_get(eid);
        if (name) {
            result_signature.push_back(name.value, field_descriptor{ eid, ent.get_type() != typename_eid });
        } else {
            result_signature.push_back(field_descriptor{ eid, ent.get_type() != typename_eid });
        }
    }
    indirect_signatured_entity smpl{ result_signature };
    entity const& entres = ctx.u().eregistry_find_or_create(smpl, [&result_signature]() {
        return make_shared<basic_signatured_entity>(std::move(result_signature));
    });
    
    return make_result(u, entres.id());
}

}
