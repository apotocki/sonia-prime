//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "preliminary_type_visitor.hpp"

namespace sonia::lang::bang {

inline unit& preliminary_type_visitor::u() const noexcept { return ctx.u(); }

preliminary_type_visitor::result_type preliminary_type_visitor::operator()(bang_preliminary_parameter_pack_t const& v) const
{
    entity_identifier element_type = apply_visitor(*this, v.type);
    functional& ellipsis_fnl = u().fregistry().resolve(ctx.u().get_ellipsis_qname_identifier());

    named_expression_list_t ellipsis_args;
    ellipsis_args.emplace_back(annotated_entity_identifier{ element_type, });
    pure_call_t ellipsis_call{ lex::resource_location{}, std::move(ellipsis_args) };
    auto match = ellipsis_fnl.find(ctx, ellipsis_call, {});
    if (!match) {
        throw exception(u().print(*match.error()));
    }
    auto r = match->const_apply(ctx);
    if (!r) {
        throw exception(ctx.u().print(*r.error()));
    }
    return *r;
}


}
