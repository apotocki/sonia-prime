//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "ct_expression_visitor.hpp"

#include "fn_compiler_context.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

namespace sonia::lang::bang {

//template <typename ExprT>
//inline ct_expression_visitor::result_type ct_expression_visitor::apply_cast(entity_identifier eid, ExprT const& e) const
//{
//    /*
//    entity_identifier typeeid = is_type_expected ?
//        expected_result.value :
//        u().eregistry_get(eid).get_type();
//
//    if (typeeid == expected_result.value || ) {
//        return eid;
//    }
//
//    */
//    BOOST_ASSERT(eid);
//    if (!expected_result || u().get(builtin_eid::any) == expected_result.value) return eid;
//    if (!is_type_expected) {
//        if (eid == expected_result.value) return eid;
//        return std::unexpected(make_error<cast_error>(get_start_location(e), expected_result.value, eid, e));
//        //THROW_NOT_IMPLEMENTED_ERROR("ct_expression_visitor::apply_cast const cast check");
//    } else {
//        entity const& ent = u().eregistry_get(eid);
//        entity_identifier typeeid = ent.get_type();
//        if (typeeid == expected_result.value ) {
//            return eid;
//        }
//
//        //GLOBAL_LOG_DEBUG() << ("expected type: %1%, actual type: %2%"_fmt % u().print(expected_result.value) % u().print(typeeid)).str();
//
//        lex::resource_location expr_loc = get_start_location(e);
//        pure_call_t cast_call{ expected_result.location };
//        //cast_call.emplace_back(annotated_identifier{ u().get(builtin_id::to) }, annotated_entity_identifier{ expected_result.value, expected_result.location });
//        cast_call.emplace_back(annotated_entity_identifier{ typeeid, expr_loc });
//
//        auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, expected_result);
//        if (!match) {
//            //return std::move(ptrn.error());
//            return std::unexpected(append_cause(
//                make_error<cast_error>(expr_loc, expected_result.value, typeeid, e),
//                std::move(match.error())
//            ));
//        }
//    
//        return match->const_apply(ctx);
//    }
//}

//ct_expression_visitor::result_type ct_expression_visitor::operator()(context_value const& cv) const
//{
//
//}

#if 0
ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_bool const& ab) const
{
    return handle(base_expression_visitor::operator()(ab));
    //bool_literal_entity bool_ent{ bv.value };
    //entity_identifier entid = u().eregistry_find_or_create(bool_ent, [this, &bool_ent]() {
    //    auto result = make_shared<bool_literal_entity>(std::move(bool_ent));
    //    result->set_type(u().get(builtin_eid::boolean));
    //    return result;
    //}).id();
    //return apply_cast(entid, bv);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_integer const& iv) const
{
    return handle(base_expression_visitor::operator()(iv));
    //integer_literal_entity int_ent{ iv.value };
    //entity_identifier entid = u().eregistry_find_or_create(int_ent, [this, &int_ent]() {
    //    auto result = make_shared<integer_literal_entity>(std::move(int_ent));
    //    result->set_type(u().get(builtin_eid::integer));
    //    return result;
    //}).id();
    //return apply_cast(entid, iv);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_decimal const& ad) const
{
    return handle(base_expression_visitor::operator()(ad));
    //decimal_literal_entity dec_ent{ dv.value };
    //entity_identifier entid = u().eregistry_find_or_create(dec_ent, [this, &dec_ent]() {
    //    auto result = make_shared<decimal_literal_entity>(std::move(dec_ent));
    //    result->set_type(u().get(builtin_eid::decimal));
    //    return result;
    //}).id();
    //return apply_cast(entid, dv);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_string const& as) const
{
    return handle(base_expression_visitor::operator()(as));
    //string_literal_entity str_ent{ sv.value };
    //entity_identifier entid = u().eregistry_find_or_create(str_ent, [this, &str_ent]() {
    //    auto result = make_shared<string_literal_entity>(std::move(str_ent));
    //    result->set_type(u().get(builtin_eid::string));
    //    return result;
    //}).id();
    //return apply_cast(entid, sv);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_identifier const& aiv) const
{
    return handle(base_expression_visitor::operator()(aiv));

    //identifier_entity id_ent{ iv.value };
    //entity_identifier entid = u().eregistry_find_or_create(id_ent, [this, &id_ent]() {
    //    auto result = make_shared<identifier_entity>(std::move(id_ent));
    //    result->set_type(u().get(builtin_eid::identifier));
    //    return result;
    //}).id();

    //return apply_cast(entid, iv);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_entity_identifier const& aei) const
{
    return handle(base_expression_visitor::operator()(aei));
    //return apply_cast(ee.value, ee);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(variable_identifier const& vi) const
{
    return handle(base_expression_visitor::operator()(vi));
    //auto optent = ctx.lookup_entity(vi.name);
    //    
    //return apply_visitor(make_functional_visitor<result_type>([this, &vi](auto & eid_or_var) -> result_type
    //{
    //    if constexpr (std::is_same_v<std::decay_t<decltype(eid_or_var)>, local_variable>) {
    //        return std::unexpected(make_shared<basic_general_error>(vi.name.location, "can't evaluate the variable as a const expression"sv, vi.name.value));
    //    } else {
    //        if (!eid_or_var) return std::unexpected(make_error<undeclared_identifier_error>(vi.name));
    //        return apply_cast(eid_or_var, vi);
    //    }
    //}), optent);
}
ct_expression_visitor::result_type ct_expression_visitor::operator()(lambda_t const& l) const
{
    THROW_NOT_IMPLEMENTED_ERROR("ct_expression_visitor::operator()(lambda_t const&)");
}
ct_expression_visitor::result_type ct_expression_visitor::operator()(bang_vector_t const& v) const
{
    return handle(base_expression_visitor::operator()(v));
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(annotated_qname const& aqn) const
{
    auto optqnid = ctx.lookup_qname(aqn);
    if (optqnid) {
        entity_identifier entid = u().make_qname_entity(*optqnid).id();
        return apply_cast(entid, aqn);
    }
    return std::unexpected(optqnid.error());
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(function_call_t const& proc) const
{
    ct_expression_visitor vis{ ctx, annotated_entity_identifier{ u().get(builtin_eid::qname), proc.location() } };
    auto qn_ent_id = apply_visitor(vis, proc.fn_object);
    if (!qn_ent_id) return std::unexpected(std::move(qn_ent_id.error()));
    qname_identifier_entity qname_ent = static_cast<qname_identifier_entity const&>(u().eregistry_get(*qn_ent_id));
    
    auto match = ctx.find(qname_ent.value(), proc, expected_result);
    if (!match) return std::unexpected(match.error());
    if (auto res = match->const_apply(ctx); !res) return std::unexpected(std::move(res.error()));
    else return apply_cast(*res, proc);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(member_expression_t const& me) const
{
    pure_call_t get_call{ me.start() };
    get_call.emplace_back(annotated_identifier{ u().get(builtin_id::self), get_start_location(me.object) }, me.object);
    get_call.emplace_back(annotated_identifier{ u().get(builtin_id::property), get_start_location(me.property) }, me.property);

    auto match = ctx.find(builtin_qnid::get, get_call, expected_result);

    if (!match) {
        return std::unexpected(append_cause(
            make_error<basic_general_error>(me.start(), "can't resolve"sv, syntax_expression_t{ me }),
            std::move(match.error())
        ));
    }

    if (auto err = match->const_apply(ctx); err) return err;
    return apply_cast(ctx.context_type, me);
}

template <std::derived_from<pure_call_t> CallExpressionT>
ct_expression_visitor::result_type ct_expression_visitor::operator()(builtin_qnid qnid, CallExpressionT const& call) const
{
    auto match = ctx.find(qnid, call, expected_result);
    if (!match) return std::unexpected(match.error());
    if (auto res = match->const_apply(ctx); !res) return std::unexpected(std::move(res.error()));
    else return apply_cast(*res, call);
}

ct_expression_visitor::result_type ct_expression_visitor::operator()(opt_named_syntax_expression_list_t const& nel) const
{
    auto res = base_expression_visitor::operator()(nel);
    if (!res) return std::unexpected(res.error());
    return apply_visitor(make_functional_visitor<result_type>([this](auto & v) -> result_type {
        if constexpr (std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
            return v;
        } else {
            if (ctx.context_type == u().get(builtin_eid::void_)) {
                BOOST_ASSERT(v.empty());
                return ctx.context_type;
            }
            THROW_NOT_IMPLEMENTED_ERROR("ct_expression_visitor::operator()(opt_named_syntax_expression_list_t const&)");
        }
    }), res->first);
}
#endif

ct_expression_visitor::result_type ct_expression_visitor::handle(base_expression_visitor::result_type&& res) const
{
    if (!res) return std::unexpected(std::move(res.error()));
    auto& [el, reid] = res->first;
    if (!el) {
        return reid;
    } else {
        THROW_NOT_IMPLEMENTED_ERROR("ct_expression_visitor::handle");
    }
}

}
