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
    if (!expected_result || u().get(builtin_eid::any) == expected_result.value) return eid;
    entity const& ent = u().eregistry_get(eid);
    entity_identifier typeeid = ent.get_type();
    if (typeeid == expected_result.value ) {
        return eid;
    }

    //GLOBAL_LOG_DEBUG() << ("expected type: %1%, actual type: %2%"_fmt % u().print(expected_result.value) % u().print(typeeid)).str();

    lex::resource_location expr_loc = get_start_location(e);
    pure_call_t cast_call{ expected_result.location };
    cast_call.emplace_back(annotated_identifier{ u().get(builtin_id::to) }, annotated_entity_identifier{ expected_result.value, expected_result.location });
    cast_call.emplace_back(annotated_entity_identifier{ typeeid, expr_loc });

    auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, expected_result);
    if (!match) {
        //return std::move(ptrn.error());
        return std::unexpected(append_cause(
            make_error<cast_error>(expr_loc, expected_result.value, typeeid, e),
            std::move(match.error())
        ));
    }
    
    return match->const_apply(ctx);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_bool const& bv) const
{
    bool_literal_entity bool_ent{ bv.value };
    entity_identifier entid = u().eregistry_find_or_create(bool_ent, [this, &bool_ent]() {
        auto result = make_shared<bool_literal_entity>(std::move(bool_ent));
        result->set_type(u().get(builtin_eid::boolean));
        return result;
    }).id();
    return apply_cast(entid, bv);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_integer const& iv) const
{
    integer_literal_entity int_ent{ iv.value };
    entity_identifier entid = u().eregistry_find_or_create(int_ent, [this, &int_ent]() {
        auto result = make_shared<integer_literal_entity>(std::move(int_ent));
        result->set_type(u().get(builtin_eid::integer));
        return result;
    }).id();
    return apply_cast(entid, iv);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_decimal const& dv) const
{
    decimal_literal_entity dec_ent{ dv.value };
    entity_identifier entid = u().eregistry_find_or_create(dec_ent, [this, &dec_ent]() {
        auto result = make_shared<decimal_literal_entity>(std::move(dec_ent));
        result->set_type(u().get(builtin_eid::decimal));
        return result;
    }).id();
    return apply_cast(entid, dv);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_string const& sv) const
{
    string_literal_entity str_ent{ sv.value };
    entity_identifier entid = u().eregistry_find_or_create(str_ent, [this, &str_ent]() {
        auto result = make_shared<string_literal_entity>(std::move(str_ent));
        result->set_type(u().get(builtin_eid::string));
        return result;
    }).id();
    return apply_cast(entid, sv);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_identifier const& iv) const
{
    identifier_entity id_ent{ iv.value };
    entity_identifier entid = u().eregistry_find_or_create(id_ent, [this, &id_ent]() {
        auto result = make_shared<identifier_entity>(std::move(id_ent));
        result->set_type(u().get(builtin_eid::identifier));
        return result;
    }).id();

    return apply_cast(entid, iv);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_entity_identifier const& ee) const
{
    return apply_cast(ee.value, ee);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_qname const& aqn) const
{
    auto optqnid = ctx.lookup_qname(aqn);
    if (optqnid) {
        qname_identifier_entity qnid_ent{ *optqnid };
        entity_identifier entid = u().eregistry_find_or_create(qnid_ent, [this, &qnid_ent]() {
            auto result = make_shared<qname_identifier_entity>(std::move(qnid_ent));
            result->set_type(u().get(builtin_eid::qname));
            return result;
        }).id();
        return apply_cast(entid, aqn);
    }
    return std::unexpected(optqnid.error());
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(variable_identifier const vi) const
{
    if (vi.implicit || (vi.name.value.is_relative() && vi.name.value.size() == 1)) {
        identifier varid = *vi.name.value.begin();
        entity_identifier eid = ctx.get_bound(varid);
        if (eid) return eid;
    }
    auto opteid = ctx.lookup_entity(vi.name);
    if (!opteid) return std::unexpected(std::move(opteid.error()));
    if (auto eid = *opteid; eid) {
        entity const& ent = u().eregistry_get(eid);
        if (variable_entity const* pve = dynamic_cast<variable_entity const*>(&ent); pve) {
            return std::unexpected(make_shared<basic_general_error>(vi.name.location, "can't evaluate the variable as a const expression"sv, vi.name.value));
        }
        return apply_cast(eid, vi);
    }
    return std::unexpected(make_error<undeclared_identifier_error>(vi.name));
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(function_call_t const& proc) const
{
    ct_expression_visitor vis{ ctx, annotated_entity_identifier{ u().get(builtin_eid::qname), proc.location() } };
    auto qn_ent_id = apply_visitor(vis, proc.fn_object);
    if (!qn_ent_id) return std::unexpected(std::move(qn_ent_id.error()));
    qname_identifier_entity qname_ent = static_cast<qname_identifier_entity const&>(u().eregistry_get(*qn_ent_id));
    
    auto match = ctx.find(qname_ent.value(), proc, expected_result);
    if (auto res = match->const_apply(ctx); !res) return std::unexpected(std::move(res.error()));
    else return apply_cast(*res, proc);
}

template <std::derived_from<pure_call_t> CallExpressionT>
ct_expression_visitor::result_type ct_expression_visitor::operator()(builtin_qnid qnid, CallExpressionT const& call) const
{
    auto match = ctx.find(qnid, call, expected_result);
    if (auto res = match->const_apply(ctx); !res) return std::unexpected(std::move(res.error()));
    else return apply_cast(*res, call);
}

}
