//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "base_expression_visitor.hpp"

#include "fn_compiler_context.hpp"
#include "ct_expression_visitor.hpp"

namespace sonia::lang::bang {

base_expression_visitor::base_expression_visitor(fn_compiler_context& c) noexcept
    : ctx{ c }
{
}

base_expression_visitor::base_expression_visitor(fn_compiler_context& c, annotated_entity_identifier&& er) noexcept
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
            //return std::move(ptrn.error());
            return std::unexpected(append_cause(
                make_error<cast_error>(expr_loc, expected_result.value, typeeid, e),
                std::move(match.error())
            ));
        }

        auto res = match->generic_apply(ctx);
        if (!res) return std::unexpected(std::move(res.error()));
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

    auto res = match->generic_apply(ctx);
    if (!res) return std::unexpected(std::move(res.error()));

    return apply_visitor(make_functional_visitor<result_type>([&l](auto& v) -> result_type {
        if constexpr (std::is_same_v<semantic::managed_expression_list, std::decay_t<decltype(v)>>) {
            l.splice_back(v);
        } // else strange case
        return std::pair{ std::move(v), true };
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

base_expression_visitor::result_type base_expression_visitor::operator()(annotated_string const& sv) const
{
    string_literal_entity str_ent{ sv.value };
    entity const& ent = u().eregistry_find_or_create(str_ent, [this, &str_ent]() {
        auto result = make_shared<string_literal_entity>(std::move(str_ent));
        result->set_type(u().get(builtin_eid::string));
        return result;
    });
    return apply_cast(ent, sv);
}

base_expression_visitor::result_type base_expression_visitor::operator()(annotated_identifier const& iv) const
{
    identifier_entity id_ent{ iv.value };
    entity const& ent = u().eregistry_find_or_create(id_ent, [this, &id_ent]() {
        auto result = make_shared<identifier_entity>(std::move(id_ent));
        result->set_type(u().get(builtin_eid::identifier));
        return result;
    });

    return apply_cast(ent, iv);
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

    auto res = match->generic_apply(ctx);
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
    for (opt_named_syntax_expression_t const& ne : nel) {
        if (auto const* pname = ne.name(); pname) {
            tuple_call.emplace_back(*pname, ne.value());
        } else {
            tuple_call.emplace_back(ne.value());
        }
    }
    auto match = ctx.find(builtin_qnid::make_tuple, tuple_call, expected_result);
    if (!match) return std::unexpected(match.error());

    auto res = match->generic_apply(ctx);
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
    auto res = match->generic_apply(ctx);
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

    auto res = match->generic_apply(ctx);
    return apply_cast(std::move(res), ne);
}

template <std::derived_from<pure_call_t> CallExpressionT>
base_expression_visitor::result_type base_expression_visitor::operator()(builtin_qnid qnid, CallExpressionT const& call) const
{
    auto match = ctx.find(qnid, call, expected_result);
    if (!match) return std::unexpected(std::move(match.error()));
    auto res = match->generic_apply(ctx);
    return apply_cast(std::move(res), call);
}

}
