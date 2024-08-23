//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "preliminary_type_visitor.hpp"

namespace sonia::lang::bang {

preliminary_type_visitor2::result_type preliminary_type_visitor2::operator()(bang_preliminary_parameter_pack_t const& v) const
{
    entity_identifier element_type = apply_visitor(*this, v.type);
    functional& ellipsis_fnl = ctx.u().fregistry().resolve(ctx.u().get_ellipsis_qname_identifier());

    named_expression_term_list_t ellipsis_args;
    ellipsis_args.emplace_back(entity_expression{ element_type, });
    pure_call_t ellipsis_call{ lex::resource_location{}, std::move(ellipsis_args) };
    auto ptrn = ellipsis_fnl.find(ctx, ellipsis_call);
    if (!ptrn.has_value()) {
        throw exception(ctx.u().print(*ptrn.error()));
    }
    auto r = ptrn.value()->const_apply(ctx);
    if (!r.has_value()) {
        throw exception(ctx.u().print(*r.error()));
    }
    return r.value();
}

preliminary_type_visitor2::result_type preliminary_type_visitor2::operator()(bang_preliminary_object_t const& v) const
{
    qname_identifier qnid = ctx.u().qnregistry().resolve(v.name());
    functional const* fn = ctx.resolve_functional(qnid);
    if (!fn) {
        throw exception(ctx.u().print(*make_error<undeclared_identifier_error>(v.location(), qnid)));
    }
    entity_identifier type_eid = fn->default_entity();
    if (!type_eid) {
        throw exception(ctx.u().print(*make_error<basic_general_error>(v.location(), "is not a type"sv)));
    }
    return type_eid;
}

}
