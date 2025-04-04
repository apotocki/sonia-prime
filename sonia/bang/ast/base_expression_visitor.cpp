//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "base_expression_visitor.hpp"

#include <boost/container/flat_set.hpp>

#include "assign_expression_visitor.hpp"

#include "fn_compiler_context.hpp"
#include "ct_expression_visitor.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"

namespace sonia::lang::bang {

base_expression_visitor::base_expression_visitor(fn_compiler_context& c) noexcept
    : ctx{ c }
{
}

base_expression_visitor::base_expression_visitor(fn_compiler_context& c, annotated_entity_identifier er) noexcept
    : ctx{ c }
    , expected_result{ std::move(er) }
{
}

unit& base_expression_visitor::u() const noexcept
{
    return ctx.u();
}

template <typename ExprT>
base_expression_visitor::result_type base_expression_visitor::apply_cast(entity_identifier eid, ExprT const& e) const
{
    entity const& ent = u().eregistry_get(eid);
    return apply_cast(ent, e);
}

template <typename ExprT>
inline base_expression_visitor::result_type base_expression_visitor::apply_cast(entity const& ent, ExprT const& e) const
{
    BOOST_ASSERT(ent.id());
    if (!expected_result || u().get(builtin_eid::any) == expected_result.value) return std::pair{ ent.id(), false };
    if (!is_type_expected) {
        if (ent.id() == expected_result.value) return std::pair{ ent.id(), false };
        return std::unexpected(make_error<cast_error>(get_start_location(e), expected_result.value, ent.id(), e));
        //THROW_NOT_IMPLEMENTED_ERROR("ct_expression_visitor::apply_cast const cast check");
    } else {
        entity_identifier typeeid = ent.get_type();
        if (typeeid == expected_result.value) {
            return std::pair{ ent.id(), false };
        }

        lex::resource_location expr_loc = get_start_location(e);
        pure_call_t cast_call{ expected_result.location };
        cast_call.emplace_back(annotated_entity_identifier{ ent.id(), expr_loc });

        auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, expected_result);
        if (!match) {
            // ignore casting error details
            return std::unexpected(make_error<cast_error>(expr_loc /*expected_result.location*/, expected_result.value, typeeid, e));
            //return std::unexpected(append_cause(
            //    make_error<cast_error>(expr_loc, expected_result.value, typeeid, e),
            //    std::move(match.error())
            //));
        }

        auto res = match->apply(ctx);
        if (!res)
            return std::unexpected(std::move(res.error()));
        return std::pair{ std::move(*res), true };
    }
}

template <typename ExprT>
base_expression_visitor::result_type base_expression_visitor::apply_cast(semantic::managed_expression_list l, ExprT const& e) const
{
    //THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor::apply_cast(semantic::managed_expression_list, ExprT const&)");
    BOOST_ASSERT(ctx.context_type);
    if (!expected_result || ctx.context_type == expected_result.value || expected_result.value == u().get(builtin_eid::any)) {
        return std::pair{ std::move(l), false };
    }

    //if (expected_result.value == u().get(builtin_eid::any)) {
    //    ctx.context_type = expected_result.value;
    //    return std::pair{ std::move(l), false };
    //}

    //GLOBAL_LOG_DEBUG() << ("expected type: %1%, actual type: %2%"_fmt % u().print(expected_result.value) % u().print(typeeid)).str();

    lex::resource_location expr_loc = get_start_location(e);
    pure_call_t cast_call{ expected_result.location };
    //cast_call.emplace_back(annotated_identifier{ u().get(builtin_id::to) }, annotated_entity_identifier{ expected_result.value, expected_result.location });
    cast_call.emplace_back(context_value{ ctx.context_type, expr_loc });

    auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, expected_result);
    if (!match) {
        // ignore casting error details
        return std::unexpected(make_error<cast_error>(expr_loc /*expected_result.location*/, expected_result.value, ctx.context_type, e));
        //return std::unexpected(append_cause(
        //    make_error<cast_error>(expr_loc /*expected_result.location*/, expected_result.value, typeeid, e),
        //    std::move(match.error())
        //));
    }

    auto res = match->apply(ctx);
    if (!res) return std::unexpected(std::move(res.error()));

    return apply_visitor(make_functional_visitor<result_type>([&l](auto& v) -> result_type {
        if constexpr (std::is_same_v<semantic::managed_expression_list, std::decay_t<decltype(v)>>) {
            l.splice_back(v);
            return std::pair{ std::move(l), true };
        } else { // else strange case
            return std::pair{ std::move(v), true };
        }
    }), *res);
}

template <typename ExprT>
inline base_expression_visitor::result_type base_expression_visitor::apply_cast(std::expected<functional::pattern::application_result_t, error_storage> r, ExprT const& e) const
{
    if (!r) return std::unexpected(std::move(r.error()));
    return apply_visitor(make_functional_visitor<result_type>([this, e](auto& v) -> result_type {
        return apply_cast(std::move(v), e);
    }), *r);
}

base_expression_visitor::result_type base_expression_visitor::operator()(context_value const& v) const
{
    ctx.context_type = v.type;
    return apply_cast(semantic::managed_expression_list{ u() }, v);
}

base_expression_visitor::result_type base_expression_visitor::operator()(annotated_bool const& bv) const
{
    return apply_cast(u().make_bool_entity(bv.value), bv);
}

base_expression_visitor::result_type base_expression_visitor::operator()(annotated_integer const& iv) const
{
    return apply_cast(u().make_integer_entity(iv.value), iv);
}

base_expression_visitor::result_type base_expression_visitor::operator()(annotated_decimal const& dv) const
{
    return apply_cast(u().make_decimal_entity(dv.value), dv);
}

base_expression_visitor::result_type base_expression_visitor::operator()(annotated_string const& sv) const
{
    return apply_cast(u().make_string_entity(sv.value), sv);
}

base_expression_visitor::result_type base_expression_visitor::operator()(annotated_identifier const& iv) const
{
    return apply_cast(u().make_identifier_entity(iv.value), iv);
}

base_expression_visitor::result_type base_expression_visitor::operator()(annotated_entity_identifier const& e) const
{
    return apply_cast(e.value, e);
}

base_expression_visitor::result_type base_expression_visitor::operator()(annotated_qname const& aqn) const
{
    auto optqnid = ctx.lookup_qname(aqn);
    if (optqnid) {
        entity const& ent = u().make_qname_entity(*optqnid);
        return apply_cast(ent, aqn);
    }
    return std::unexpected(optqnid.error());
}

base_expression_visitor::result_type base_expression_visitor::operator()(bang_vector_t const& v) const
{
    auto res = apply_visitor(ct_expression_visitor{ ctx }, v.type);
    if (!res) return std::unexpected(res.error());
    return apply_cast(u().make_vector_type_entity(*res), v);
}


base_expression_visitor::result_type base_expression_visitor::operator()(array_expression_t const& ve) const
{
    optional<semantic::managed_expression_list> optexprs;
    small_vector<entity_identifier, 16> ct_element_results;
    small_vector<semantic::expression_span, 16> rt_element_results; //to do: gather content types

    for (syntax_expression_t const& ee : ve.elements) {
        result_type res = apply_visitor(base_expression_visitor{ ctx }, ee);
        if (!res) return std::unexpected(res.error());
        apply_visitor(make_functional_visitor<void>([this, &ct_element_results, &rt_element_results, &optexprs](auto & v) {
            if constexpr (std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
                if (rt_element_results.empty()) {
                    ct_element_results.push_back(v);
                } else {
                    u().push_back_expression(*optexprs, semantic::push_value{ v });
                    rt_element_results.emplace_back(&optexprs->back_entry());
                }
            } else {
                semantic::expression_span argspan = v;
                if (!optexprs) {
                    optexprs.emplace(std::move(v));
                    if (!ct_element_results.empty()) {
                        for (entity_identifier eid : ct_element_results) {
                            u().push_back_expression(*optexprs, semantic::push_value{ eid });
                            rt_element_results.emplace_back(&optexprs->back_entry());
                        }
                    }
                }
                rt_element_results.push_back(std::move(argspan));
                optexprs->splice_back(v);
            }
        }), res->first);
    }
    if (rt_element_results.empty()) {
        // gather types
        boost::container::small_flat_set<entity_identifier, 8> types;
        for (entity_identifier eid : ct_element_results) {
            entity_identifier elemtype_eid = u().eregistry_get(eid).get_type();
            types.insert(elemtype_eid);
        }
        if (types.size() > 1) {
            THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor array_expression_t");
        }
        entity const& arr_ent = u().make_array_entity(*types.begin(), ct_element_results);
        return apply_cast(arr_ent, ve);
    }
    THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor array_expression_t");
}

base_expression_visitor::result_type base_expression_visitor::operator()(variable_identifier const& var) const
{
    auto optent = ctx.lookup_entity(var.name);
    return apply_visitor(make_functional_visitor<result_type>([this, &var](auto eid_or_var) -> result_type
    {
        if constexpr (std::is_same_v<std::decay_t<decltype(eid_or_var)>, entity_identifier>) {
            if (eid_or_var) {
                return apply_cast(eid_or_var, var);
            }
            return std::unexpected(make_error<undeclared_identifier_error>(var.name));
        } else { // if constexpr (std::is_same_v<std::decay_t<decltype(eid_or_var)>, local_variable>) {
            semantic::managed_expression_list exprs{ u() };
            ctx.u().push_back_expression(exprs, semantic::push_local_variable{ eid_or_var });
            ctx.context_type = eid_or_var.type;
            return apply_cast(std::move(exprs), var);
        }
    }), optent);
}

base_expression_visitor::result_type base_expression_visitor::operator()(member_expression_t const& me) const
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

    auto res = match->apply(ctx);
    return apply_cast(std::move(res), me);
}

//base_expression_visitor::result_type base_expression_visitor::operator()(context_identifier const& ci) const
//{
//    if (!expected_result) {
//        return std::unexpected(make_error<basic_general_error>(ci.start, "no context type to resolve the context identifier"sv, ci.name.value));
//    }
//    THROW_EOF_ERROR("base_expression_visitor context_identifier");
//}

base_expression_visitor::result_type base_expression_visitor::operator()(opt_named_syntax_expression_list_t const& nel) const
{
    // e.g. case: let val = (a, b, c)
    pure_call_t tuple_call{ nel.location };
    for (opt_named_syntax_expression_t const& ne : nel.elements) {
        if (auto const* pname = ne.name(); pname) {
            tuple_call.emplace_back(*pname, ne.value());
        } else {
            tuple_call.emplace_back(ne.value());
        }
    }
    auto match = ctx.find(builtin_qnid::make_tuple, tuple_call, expected_result);
    if (!match) return std::unexpected(match.error());

    auto res = match->apply(ctx);
    return apply_cast(std::move(res), nel);
}

base_expression_visitor::result_type base_expression_visitor::operator()(unary_expression_t const& be) const
{
    switch (be.op) {
    case unary_operator_type::NEGATE:
        return this->operator()(builtin_qnid::negate, be);
    case unary_operator_type::ELLIPSIS:
        return this->operator()(builtin_qnid::ellipsis, be);
    }
    THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor unary_expression_t");
}

base_expression_visitor::result_type base_expression_visitor::operator()(function_call_t const& proc) const
{
    ct_expression_visitor vis{ ctx, annotated_entity_identifier{ u().get(builtin_eid::qname), proc.location() } };
    auto qn_ent_id = apply_visitor(vis, proc.fn_object);
    if (!qn_ent_id) return std::unexpected(std::move(qn_ent_id.error()));
    qname_identifier_entity qname_ent = static_cast<qname_identifier_entity const&>(u().eregistry_get(*qn_ent_id));

    auto match = ctx.find(qname_ent.value(), proc, expected_result);
    if (!match) return std::unexpected(match.error());
    auto res = match->apply(ctx);
    return apply_cast(std::move(res), proc);
}

base_expression_visitor::result_type base_expression_visitor::operator()(new_expression_t const& ne) const
{
    pure_call_t new_call{ ne.location };
    new_call.emplace_back(annotated_identifier{ u().get(builtin_id::type) }, ne.name);
    for (auto const& arg: ne.arguments) {
        if (annotated_identifier const* optname = arg.name(); optname) {
            new_call.emplace_back(*optname, arg.value());
        } else {
            new_call.emplace_back(arg.value());
        }
    }

    auto match = ctx.find(builtin_qnid::new_, new_call, expected_result);
    if (!match) {
        return std::unexpected(append_cause(
            make_error<basic_general_error>(ne.location, "can't instantiate the object"sv, ne.name),
            std::move(match.error())
        ));
    }

    auto res = match->apply(ctx);
    return apply_cast(std::move(res), ne);
}

base_expression_visitor::result_type base_expression_visitor::operator()(index_expression_t const& ie) const
{
    auto res = apply_visitor(base_expression_visitor{ ctx }, ie.base);
    if (!res) return std::unexpected(res.error());
    if (entity_identifier const* peid = get<entity_identifier>(&res->first); peid) {
        entity const& ent = u().eregistry_get(*peid);
        if (ent.get_type() == u().get(builtin_eid::typename_)) { // this is array type declaration
            auto szres = apply_visitor(ct_expression_visitor{ ctx, annotated_entity_identifier{ u().get(builtin_eid::integer), get_start_location(ie.index) } }, ie.index);
            if (!szres) return std::unexpected(szres.error());
            integer_literal_entity const& index_ent = static_cast<integer_literal_entity const&>(u().eregistry_get(*szres));
            if (index_ent.value() <= 0) {
                return std::unexpected(make_error<basic_general_error>(get_start_location(ie.index), "index must be greater than 0"sv));
            }
            return apply_cast(u().make_array_type_entity(*peid, (size_t)index_ent.value()).id(), ie);
        }
    }

    // else get call
    pure_call_t get_call{ ie.location };
    get_call.emplace_back(annotated_identifier{ u().get(builtin_id::self) }, ie.base);
    get_call.emplace_back(annotated_identifier{ u().get(builtin_id::property) }, ie.index);
    
    auto match = ctx.find(builtin_qnid::new_, get_call, expected_result);
    if (!match) {
        return std::unexpected(std::move(match.error()));
    }

    return apply_cast(match->apply(ctx), ie);
}

base_expression_visitor::result_type base_expression_visitor::operator()(binary_expression_t const& be) const
{
    switch (be.op) {
    case binary_operator_type::EQ:
        return this->operator()(builtin_qnid::eq, be);
    case binary_operator_type::NE:
        return this->operator()(builtin_qnid::ne, be);
    case binary_operator_type::PLUS:
        return this->operator()(builtin_qnid::plus, be);
    case binary_operator_type::MINUS:
        return this->operator()(builtin_qnid::minus, be);
    case binary_operator_type::BIT_OR:
        return this->operator()(builtin_qnid::bit_or, be);
    case binary_operator_type::BIT_AND:
        return this->operator()(builtin_qnid::bit_and, be);
    case binary_operator_type::ASSIGN:
        return do_assign(be);
    }
    
    THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor binary_expression_t");
}

template <std::derived_from<pure_call_t> CallExpressionT>
base_expression_visitor::result_type base_expression_visitor::operator()(builtin_qnid qnid, CallExpressionT const& call) const
{
    auto match = ctx.find(qnid, call, expected_result);
    if (!match) return std::unexpected(std::move(match.error()));
    auto res = match->apply(ctx);
    return apply_cast(std::move(res), call);
}

base_expression_visitor::result_type base_expression_visitor::do_assign(binary_expression_t const& op) const
{
    //THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor binary_operator_type::ASSIGN");
    //GLOBAL_LOG_INFO() << "left expression: " << ctx.u().print(op.left);
    //size_t start_result_pos = result.size();
    assign_expression_visitor lvis{ ctx, op.location(), op[1].value() };

    auto res = apply_visitor(lvis, op[0].value());
    if (!res) return std::unexpected(std::move(res.error()));

    return apply_cast(std::move(res), op);

    //ctx.context_type = ctx.u().get(builtin_eid::void_);
    //return std::pair{ semantic::managed_expression_list{ ctx.u() }, false };
}

}