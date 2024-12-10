//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "preliminary_type_visitor.hpp"

namespace sonia::lang::bang {

preliminary_type_visitor::result_type preliminary_type_visitor::operator()(bang_preliminary_parameter_pack_t const& v) const
{
    entity_identifier element_type = apply_visitor(*this, v.type);
    functional& ellipsis_fnl = ctx.u().fregistry().resolve(ctx.u().get_ellipsis_qname_identifier());

    named_expression_list_t ellipsis_args;
    ellipsis_args.emplace_back(annotated_entity_identifier{ element_type, });
    pure_call_t ellipsis_call{ lex::resource_location{}, std::move(ellipsis_args) };
    auto match = ellipsis_fnl.find(ctx, ellipsis_call, {});
    if (!match) {
        throw exception(ctx.u().print(*match.error()));
    }
    auto [ptrn, pmd] = std::move(*match);
    auto r = ptrn->const_apply(ctx, *pmd);
    if (!r) {
        throw exception(ctx.u().print(*r.error()));
    }
    return *r;
}

preliminary_type_visitor::result_type preliminary_type_visitor::operator()(annotated_qname const& v) const
{
    THROW_NOT_IMPLEMENTED_ERROR("preliminary_type_visitor  annotated_qname");
#if 0
    qname_identifier qnid = ctx.u().qnregistry().resolve(v.value);
    functional const* fn = ctx.resolve_functional(qnid);
    if (!fn) {
        throw exception(ctx.u().print(*make_error<undeclared_identifier_error>(v.location, qnid)));
    }
    entity_identifier type_eid = fn->default_entity();
    if (!type_eid) {
        throw exception(ctx.u().print(*make_error<basic_general_error>(v.location, "is not a type"sv)));
    }
    return type_eid;
#endif
}

preliminary_type_visitor::result_type preliminary_type_visitor::operator()(annotated_identifier const& v) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

}
