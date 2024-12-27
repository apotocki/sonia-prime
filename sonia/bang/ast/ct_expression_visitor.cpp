//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "ct_expression_visitor.hpp"

#include "fn_compiler_context.hpp"

namespace sonia::lang::bang {

inline unit& ct_expression_visitor::u() const noexcept { return ctx.u(); }

template <typename ExprT>
inline ct_expression_visitor::result_type ct_expression_visitor::apply_cast(entity_identifier eid, ExprT const& e) const
{
    BOOST_ASSERT(eid);
    if (!expected_result || u().get_any_entity_identifier() == expected_result.value) return eid;
    entity const& ent = u().eregistry().get(eid);
    entity_identifier typeeid = ent.get_type();
    if (typeeid == expected_result.value ) {
        return eid;
    }

    //GLOBAL_LOG_DEBUG() << ("expected type: %1%, actual type: %2%"_fmt % u().print(expected_result.value) % u().print(typeeid)).str();

    functional const& fn = u().fregistry().resolve(u().get_implicit_cast_qname_identifier());
    lex::resource_location expr_loc = get_start_location(e);
    pure_call_t cast_call{ expected_result.location };
    cast_call.emplace_back(annotated_identifier{ u().get_to_parameter_identifier() }, annotated_entity_identifier{ expected_result.value, expected_result.location });
    cast_call.emplace_back(annotated_entity_identifier{ typeeid, expr_loc });

    auto match = fn.find(ctx, cast_call, expected_result);
    if (!match) {
        //return std::move(ptrn.error());
        return std::unexpected(append_cause(
            make_error<cast_error>(expr_loc, expected_result.value, typeeid, e),
            std::move(match.error())
        ));
    }
    
    return match->const_apply(ctx);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_string const& sv) const
{
    string_literal_entity str_ent{ sv.value };

    entity_identifier entid = ctx.u().eregistry().find_or_create(str_ent, [this, &str_ent]() {
        auto result = make_shared<string_literal_entity>(std::move(str_ent));
        result->set_type(u().get_string_entity_identifier());
        return result;
    }).id();

    return apply_cast(entid, sv);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_identifier const& iv) const
{
    identifier_entity id_ent{ iv.value };
    entity_identifier entid = ctx.u().eregistry().find_or_create(id_ent, [this, &id_ent]() {
        auto result = make_shared<identifier_entity>(std::move(id_ent));
        result->set_type(u().get_identifier_entity_identifier());
        return result;
    }).id();

    return apply_cast(entid, iv);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_entity_identifier const& ee) const
{
    return apply_cast(ee.value, ee);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(variable_identifier const vi) const
{
    if (functional const* fl = ctx.lookup_functional(vi.name.value); fl) {
        if (entity_identifier eid = fl->default_entity(); eid) { // variable or const
            entity const& ent = u().eregistry().get(eid);
            if (variable_entity const* pve = dynamic_cast<variable_entity const*>(&ent); pve) {
                return std::unexpected(make_shared<basic_general_error>(vi.name.location, "can't evaluate the variable as a const expression"sv, vi.name.value));
            }
            return apply_cast(eid, vi);
        }
    }
    return std::unexpected(make_error<undeclared_identifier_error>(vi.name));
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(function_call_t const& proc) const
{
    variable_identifier const* fnvar = get<variable_identifier>(&proc.fn_object);
    if (!fnvar) {
        THROW_NOT_IMPLEMENTED_ERROR("fn object expression is not implemented yet");
    }
    //GLOBAL_LOG_INFO() << "function call: " << u().print(fnvar->name.value);
    functional const* fnl = ctx.lookup_functional(fnvar->name.value);
    if (!fnl) {
        return std::unexpected(make_error<undeclared_identifier_error>(fnvar->name));
    }

    return this->operator()(*fnl, proc);
}

template <std::derived_from<pure_call_t> CallExpressionT>
ct_expression_visitor::result_type ct_expression_visitor::operator()(functional const& fnl, CallExpressionT const& call) const
{
    auto match = fnl.find(ctx, call, expected_result);
    if (!match) return std::unexpected(std::move(match.error()));

    auto res = match->const_apply(ctx);
    if (!res) return std::unexpected(std::move(res.error()));

    return apply_cast(*res, call);
}

}
