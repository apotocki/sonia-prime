//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "expression_visitor.hpp"

#include "assign_expression_visitor.hpp"
#include "declaration_visitor.hpp"

#include "sonia/utility/scope_exit.hpp"

#include "fn_compiler_context.hpp"
#include "../entities/enum_entity.hpp"
#include "../entities/functional_entity.hpp"

#include "ct_expression_visitor.hpp"

#include "sonia/bang/errors.hpp"

#include <expected>

namespace sonia::lang::bang {

inline unit& expression_visitor::u() const noexcept { return ctx.u(); }

template <typename ExprT>
inline expression_visitor::result_type expression_visitor::apply_cast(entity_identifier typeeid, ExprT const& e) const
{
    BOOST_ASSERT(typeeid);
    if (!expected_result) {
        ctx.context_type = typeeid;
        return false;
    }

    if (typeeid == expected_result.value) {
        ctx.context_type = typeeid;
        return false;
    }

    if (expected_result.value == u().get(builtin_eid::any)) {
        ctx.context_type = expected_result.value;
        return false;
    }

    //GLOBAL_LOG_DEBUG() << ("expected type: %1%, actual type: %2%"_fmt % u().print(expected_result.value) % u().print(typeeid)).str();

    lex::resource_location expr_loc = get_start_location(e);
    pure_call_t cast_call{ expected_result.location };
    cast_call.emplace_back(annotated_identifier{ u().get(builtin_id::to) }, annotated_entity_identifier{ expected_result.value, expected_result.location });
    cast_call.emplace_back(context_value{ typeeid, expr_loc });

    auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, expected_result);
    if (!match) {
        // ignore casting error details
        return std::unexpected(make_error<cast_error>(expr_loc /*expected_result.location*/, expected_result.value, typeeid, e));
        //return std::unexpected(append_cause(
        //    make_error<cast_error>(expr_loc /*expected_result.location*/, expected_result.value, typeeid, e),
        //    std::move(match.error())
        //));
    }
    
    if (auto err = match->apply(ctx); err)
        return std::unexpected(std::move(err));

    return true;
}

expression_visitor::result_type expression_visitor::operator()(context_value const& v) const
{
    return apply_cast(v.type, v);
}

expression_visitor::result_type expression_visitor::operator()(annotated_bool const& b) const
{
    ctx.append_expression(semantic::push_value{ b.value });
    return apply_cast(u().get(builtin_eid::boolean), b);
}

expression_visitor::result_type expression_visitor::operator()(annotated_integer const& d) const
{
    // optimisation. We don't need entities on the semantic expressions layer. Just values are enough.
#if 0
    integer_literal_entity val{ d.value };
    val.set_type(u().get_integer_entity_identifier());
    entity const& valent = u().eregistry_find_or_create(val, [&val]() {
        return make_shared<integer_literal_entity>(std::move(val));
    });
    ctx.append_expression(semantic::push_value{ valent.id() });
    return apply_cast(u().get_integer_entity_identifier(), d);
#else
    ctx.append_expression(semantic::push_value{ d.value });
    return apply_cast(u().get(builtin_eid::integer), d);
#endif
}

expression_visitor::result_type expression_visitor::operator()(annotated_decimal const& d) const
{
    ctx.append_expression(semantic::push_value{ d.value });
    return apply_cast(u().get(builtin_eid::decimal), d);
}

expression_visitor::result_type expression_visitor::operator()(annotated_string const& s) const
{
    ctx.append_expression(semantic::push_value{ s.value });
    return apply_cast(u().get(builtin_eid::string), s);
}

expression_visitor::result_type expression_visitor::operator()(annotated_identifier const& i) const
{
    ctx.append_expression(semantic::push_value{ i.value });
    return apply_cast(u().get(builtin_eid::identifier), i);
}

//expression_visitor::result_type expression_visitor::operator()(annotated_qname const& aqn) const
//{
//    GLOBAL_LOG_INFO() << ctx.u().print(aqn.value);
//    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor annotated_qname");
//}

expression_visitor::result_type expression_visitor::operator()(variable_identifier const& var) const
{
 //   THROW_NOT_IMPLEMENTED_ERROR("expression_visitor variable_identifier");
#if 1
    auto opteid = ctx.lookup_entity(var.name);
    if (!opteid) return std::unexpected(std::move(opteid.error()));
    if (auto eid = *opteid; eid) {
        ctx.append_expression(semantic::push_value{ eid });
        entity const& ent = u().eregistry_get(eid);
        return apply_cast(ent.get_type(), var);
    }
#if 0
        shared_ptr<entity> e = ctx.resolve_entity(var.name.value);
        if (auto varptr = dynamic_cast<variable_entity*>(e.get()); varptr) {
            variable_entity::kind k = varptr->varkind();
            if (k == variable_entity::kind::EXTERN) {
                // do nothing
            } else if (k == variable_entity::kind::STATIC) {
                THROW_NOT_IMPLEMENTED_ERROR();
            } else if (k == variable_entity::kind::SCOPE_LOCAL || k == variable_entity::kind::LOCAL) {
                if (!ctx.u().qnregistry().resolve(varptr->name()).parent().has_prefix(ctx.base_ns())) {
                    if (k == variable_entity::kind::SCOPE_LOCAL || var.scope_local) {
                        return make_error<basic_general_error>(var.name.location, "variable is not defined in the scope"sv, var.name.value);
                    }
                    varptr = &ctx.create_captured_variable_chain(*varptr);
                }
            } else {
                THROW_INTERNAL_ERROR("unknown variable kind");
            }

            ctx.append_expression(semantic::push_variable{ varptr });
            if (varptr->is_weak()) {
                ctx.append_expression(semantic::invoke_function{ ctx.u().get_builtin_function(unit::builtin_fn::weak_lock) });
            }
            return apply_cast(varptr->type(), var);
        } else if (auto fnptr = dynamic_pointer_cast<functional_entity>(e); fnptr) {
            if (!expected_result) {
                THROW_NOT_IMPLEMENTED_ERROR("forward functional template");
            }
            expression_fn_visitor dvis{ ctx, *fnptr, [this, &var] { return error_context{var, expected_result.location}; } };
            return apply_visitor(dvis, expected_result.value);
        }
        /*
        // to do: look for variable first
        if (variable_entity const* pv = ctx.resolve_variable(aqnm.name); pv) {
            result.emplace_back(semantic::push_variable{ pv });
            return pv->type();
        }
        */
#endif
    return std::unexpected(make_error<undeclared_identifier_error>(var.name));
#endif
}

//expression_visitor::result_type expression_visitor::operator()(negate_expression_t & op) const
//{
//    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor negate_expression_t");
//#if 0
//    expression_visitor rvis{ ctx, nullptr };
//    if (auto opterr = apply_visitor(rvis, op.argument); opterr) return std::move(opterr);
//    
//    ctx.append_expression(semantic::invoke_function{ ctx.u().get_builtin_function(unit::builtin_fn::negate) });
//    
//    // "result of negated expression"
//    return apply_cast(bang_bool_t{}, op);
//#endif
//}

expression_visitor::result_type expression_visitor::do_assign(binary_expression_t const& op) const
{
    //THROW_NOT_IMPLEMENTED_ERROR("expression_visitor binary_operator_type::ASSIGN");
    //GLOBAL_LOG_INFO() << "left expression: " << ctx.u().print(op.left);
    //size_t start_result_pos = result.size();
    assign_expression_visitor lvis{ ctx, op.location(), op[1].value() };

    auto e = apply_visitor(lvis, op[0].value());
    if (e) return std::unexpected(std::move(e));

    return false;
#if 0
    if (variable_entity const* ve = dynamic_cast<variable_entity const*>(*e); ve) {
        expression_visitor rvis{ ctx, annotated_entity_identifier{ ve->get_type(), op.location() } };
        auto res = apply_visitor(rvis, op[1].value());
        if (!res) return std::move(res);
        if (ve->is_weak()) {
            THROW_NOT_IMPLEMENTED_ERROR("expression_visitor binary_operator_type::ASSIGN");
            //ctx.append_expression(semantic::invoke_function{ ctx.u().get_builtin_function(unit::builtin_fn::weak_create) });
        }
        ctx.append_expression(semantic::set_variable{ ve });
        if (ve->is_weak()) {
            ctx.append_expression(semantic::truncate_values(1, false));
        }
        return apply_cast(ve->get_type(), op);
    } else {
        THROW_NOT_IMPLEMENTED_ERROR("expression_visitor binary_operator_type::ASSIGN");
#if 0
        if (function_entity const* fe = dynamic_cast<function_entity const*>(e.value()); fe) {
        // fe: (object, property value)->
        bang_type const& t = fe->signature().position_parameters().back();
        expression_visitor rvis{ ctx, expected_result_t{ t, op.location() }};
        auto opterr = apply_visitor(rvis, op.positioned_args[1]);
        ctx.append_expression(semantic::invoke_function{ fe->name() });
        return std::move(opterr);
    } else  else {
        // to do: functional entity assignment
        THROW_NOT_IMPLEMENTED_ERROR();
#endif
    }
#endif

    /*
    if (variable_identifier const* varnm = get<variable_identifier>(&op.left); varnm) {
        auto optvar = ctx.resolve_variable(varnm->name);
        if (!optvar) {
            ctx.throw_undeclared_identifier(varnm->name, varnm->location);
        }
        result.emplace_back(semantic::set_variable{ optvar });
        return std::move(rtype);
    }
    THROW_NOT_IMPLEMENTED_ERROR();
    */
}

expression_visitor::result_type expression_visitor::operator()(annotated_entity_identifier const& ee) const
{
    entity const& ent = u().eregistry_get(ee.value);
    if (expected_result) {
        if (ent.is(ctx, expected_result.value)) {
            ctx.append_expression(semantic::push_value{ ee.value });
            ctx.context_type = expected_result.value;
            return {};
        }
        GLOBAL_LOG_INFO() << "expected: " << u().print(expected_result.value) << ", actual: " << u().print(ent.id());
        THROW_NOT_IMPLEMENTED_ERROR("expression_visitor entity_expression");
    }
    ctx.append_expression(semantic::push_value{ ee.value });
    ctx.context_type = ent.get_type();
    return {};
}

/*
void attach_not_true_node(semantic::logic_tree_node_t & parent, shared_ptr<semantic::logic_tree_node_t> child)
{
    if (parent.true_branch) {
        attach_not_true_node(*parent.true_branch, child);
    }
    if (parent.false_branch) {
        attach_not_true_node(*parent.false_branch, child);
    } else {
        parent.false_branch = child;
    }
}

void attach_not_false_node(semantic::logic_tree_node_t & parent, shared_ptr<semantic::logic_tree_node_t> child)
{
    if (parent.true_branch) {
        attach_not_false_node(*parent.true_branch, child);
    } else {
        parent.true_branch = child;
    }
    if (parent.false_branch) {
        attach_not_false_node(*parent.false_branch, child);
    }
}

bang_type calculate_type(semantic::logic_tree_node_t & node)
{
    bang_type res = node.true_branch ? calculate_type(*node.true_branch) : node.expression_type;
    if (node.false_branch) {
        bang_type fbranchtype = calculate_type(*node.false_branch);
        return make_union_type(std::move(res), &fbranchtype);
    }
    return res;
}

expression_visitor::result_type expression_visitor::operator()(logic_and_expression_t& op) const
{
    auto plcond = make_shared<semantic::logic_tree_node_t>();
    ctx.push_chain(plcond->condition_expression);

    bang_type const* exprt = expected_result ? &expected_result.value : nullptr;
    bang_type ut = make_union_type(bang_any_t{}, exprt);
    expression_visitor largvis{ ctx, expected_result_t{ ut, op.location } };
    auto ltype = apply_visitor(largvis, op.left);
    if (!ltype.has_value()) return ltype;
    plcond->expression_type = std::move(ltype.value());
    ctx.pop_chain();

    plcond->true_branch = make_shared<semantic::logic_tree_node_t>();
    ctx.push_chain(plcond->true_branch->condition_expression);
    expression_visitor rargvis{ ctx, expected_result };
    auto rtype = apply_visitor(rargvis, op.right);
    if (!rtype.has_value()) return rtype;
    plcond->true_branch->expression_type = std::move(rtype.value());
    ctx.pop_chain();

    bang_type restype = bang_any_t{};
    // is prev a logical branch?
    BOOST_ASSERT(!plcond->condition_expression.empty());

    if (auto* cond = get<semantic::logic_tree_node_t>(&plcond->condition_expression.back()); cond && plcond->condition_expression.size() == 1) {
        attach_not_false_node(*cond, std::move(plcond->true_branch));
        restype = calculate_type(*cond);
        ctx.append_expression(std::move(*cond));
    } else {
        restype = make_union_type(plcond->expression_type, &plcond->true_branch->expression_type);
        ctx.append_expression(std::move(*plcond));
    }
    return apply_cast(restype, op);


    // is prev a logical branch?
    //auto *prevcond = get<semantic::logic_tree_node_t>(&ctx.expressions().back());
    //THROW_NOT_IMPLEMENTED_ERROR();

    //cond.true_branch.emplace_back(semantic::truncate_values{ 1, false }); // remove true valued first argument
    //ctx.append_expression(std::move(cond));
    //ctx.push_chain(get<semantic::conditional<semantic::expression_type>>(ctx.expressions().back()).true_branch);
    //
    //ctx.pop_chain();
    //return rtype;
    ////return make_union_type(bang_particular_bool_t{ false }, &*rtype); // false || rtype
}

expression_visitor::result_type expression_visitor::operator()(logic_or_expression_t& op) const
{
    auto plcond = make_shared<semantic::logic_tree_node_t>();
    ctx.push_chain(plcond->condition_expression);

    bang_type const* exprt = expected_result ? &expected_result.value : nullptr;
    bang_type ut = make_union_type(bang_any_t{}, exprt);
    expression_visitor largvis{ ctx, expected_result_t {ut, op.location }};
    auto ltype = apply_visitor(largvis, op.left);
    if (!ltype.has_value()) return ltype;
    plcond->expression_type = std::move(ltype.value());
    ctx.pop_chain();

    plcond->false_branch = make_shared<semantic::logic_tree_node_t>();
    ctx.push_chain(plcond->false_branch->condition_expression);
    expression_visitor rargvis{ ctx, expected_result };
    auto rtype = apply_visitor(rargvis, op.right);
    if (!rtype.has_value()) return rtype;
    plcond->false_branch->expression_type = std::move(rtype.value());
    ctx.pop_chain();

    bang_type restype = bang_any_t{};
    // is prev a logical branch?
    BOOST_ASSERT(!plcond->condition_expression.empty());
    
    if (auto* cond = get<semantic::logic_tree_node_t>(&plcond->condition_expression.back()); cond && plcond->condition_expression.size() == 1) {
        attach_not_true_node(*cond, std::move(plcond->false_branch));
        restype = calculate_type(*cond);
        ctx.append_expression(std::move(*cond));
    } else {
        restype = make_union_type(plcond->expression_type, &plcond->false_branch->expression_type);
        ctx.append_expression(std::move(*plcond));
    }
    return apply_cast(restype, op);
    
    //semantic::conditional<semantic::expression_type> cond { semantic::condition_type::logic };
    //cond.false_branch.emplace_back(semantic::truncate_values{ 1, false }); // remove false valued first argument
    //ctx.append_expression(std::move(cond));
    //ctx.push_chain(get<semantic::conditional<semantic::expression_type>>(ctx.expressions().back()).false_branch);
    //auto rargvis = exprt ? expression_visitor{ ctx, expected_result_t{ *exprt, op.location } } : expression_visitor{ ctx };
    //return apply_visitor(rargvis, op.right);
    ////return make_union_type(std::move(*ltype), &*rtype);  // true || rtype
}
*/

expression_visitor::result_type expression_visitor::operator()(binary_operator_t<binary_operator_type::LOGIC_AND>, binary_expression_t& op) const
{
    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor binary_operator_type::LOGIC_AND");
    
#if 0
    auto largvis = expected_result
        ? expression_visitor{ ctx, expected_result_t{ bang_type{bang_any_t{}} || expected_result.value, expected_result.location }}
        : expression_visitor{ ctx, nullptr };

    if (auto opterr = apply_visitor(largvis, op.left); opterr) return opterr;
    auto ltype = ctx.context_type;
    
    ctx.append_expression(semantic::conditional_t{});
    semantic::conditional_t & cond = get<semantic::conditional_t>(ctx.expressions().back());
    auto st = ctx.expressions_state(); // pin state

    ctx.push_chain(cond.true_branch);
    ctx.append_expression(semantic::truncate_values(1, false)); // remove result of left expression
    expression_visitor rargvis{ ctx, expected_result };
    if (auto opterr = apply_visitor(rargvis, op.right); opterr) return opterr;
    auto rtype = ctx.context_type;
    st.restore_and_detach();
    
    bang_bunion_t const* pbur = rtype.as<bang_bunion_t>();
    bang_bunion_t const* pbul = ltype.as<bang_bunion_t>();

    bang_bunion_t res_type{
        /*true_type*/  pbur ? pbur->true_type : rtype,
        /*false_type*/ (pbul ? pbul->false_type : ltype) || (pbur ? pbur->false_type : rtype)
    };
    if (res_type.true_type == res_type.false_type) {
        return apply_cast(res_type.true_type, op);
    }
    return apply_cast(res_type, op);
#endif
}

expression_visitor::result_type expression_visitor::operator()(binary_operator_t<binary_operator_type::LOGIC_OR>, binary_expression_t & op) const
{
    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor binary_operator_type::LOGIC_OR");

#if 0
    auto largvis = expected_result
        ? expression_visitor{ ctx, expected_result_t{ expected_result.value || bang_any_t{}, expected_result.location }}
        : expression_visitor{ ctx, nullptr };
    
    if (auto opterr = apply_visitor(largvis, op.left); opterr) return opterr;
    auto ltype = ctx.context_type;
    
    ctx.append_expression(semantic::conditional_t{});
    semantic::conditional_t& cond = get<semantic::conditional_t>(ctx.expressions().back());
    auto st = ctx.expressions_state(); // pin state

    ctx.push_chain(cond.false_branch);
    ctx.append_expression(semantic::truncate_values(1, false)); // remove result of left expression
    expression_visitor rargvis{ ctx, expected_result };
    if (auto opterr = apply_visitor(rargvis, op.right); opterr) return opterr;
    auto rtype = ctx.context_type;
    st.restore_and_detach();

    bang_bunion_t const* pbur = rtype.as<bang_bunion_t>();
    bang_bunion_t const* pbul = ltype.as<bang_bunion_t>();

    bang_bunion_t res_type{
        /*true_type*/  (pbul ? pbul->true_type : ltype) || (pbur ? pbur->true_type : rtype),
        /*false_type*/ pbur ? pbur->false_type : rtype
    };

    if (res_type.true_type == res_type.false_type) {
        return apply_cast(res_type.true_type, op);
    }
    return apply_cast(res_type, op);
#endif
}

expression_visitor::result_type expression_visitor::operator()(unary_expression_t const& be) const
{
    switch (be.op) {
    case unary_operator_type::NEGATE:
        return this->operator()(builtin_qnid::negate, be);
    }
    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor unary_expression_t");
}

expression_visitor::result_type expression_visitor::operator()(binary_expression_t const& be) const
{
    switch (be.op) {
    case binary_operator_type::EQ:
        return this->operator()(builtin_qnid::eq, be);
    case binary_operator_type::NE:
        return this->operator()(builtin_qnid::ne, be);
    case binary_operator_type::PLUS:
        return this->operator()(builtin_qnid::plus, be);
    case binary_operator_type::ASSIGN:
        return do_assign(be);
    }
    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor binary_expression_t");
    //return bang_binary_switcher(be, *this);
}


template <binary_operator_type BOpV>
expression_visitor::result_type expression_visitor::operator()(binary_operator_t<BOpV>, binary_expression_t& op) const
{
    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor binary_operator_t<BOpV>");
#if 0
    // find a functional
    auto & func_ent = ctx.u().get_functional_entity(BOpV);
    
    pure_call_t proc(std::move(op.location), {});
    proc.positioned_args.emplace_back(std::move(op.left), op.start());
    proc.positioned_args.emplace_back(std::move(op.right), op.location);
    auto optres = func_ent.find(ctx, proc);
    if (!optres.has_value()) return std::move(optres.error());
    return apply_cast(optres.value()->fn_type.result, op);
#endif
}

template expression_visitor::result_type expression_visitor::operator()(binary_operator_t<binary_operator_type::CONCAT>, binary_expression_t&) const;
template expression_visitor::result_type expression_visitor::operator()(binary_operator_t<binary_operator_type::PLUS>, binary_expression_t&) const;

//expression_visitor::result_type expression_visitor::operator()(binary_operator_t<binary_operator_type::CONCAT>, binary_expression_t& op) const
//{
//    // find a function
//    auto func_ent = dynamic_pointer_cast<functional_entity>(ctx.u().eregistry().find(ctx.u().make_qname_identifier("concat"sv)));
//    BOOST_ASSERT(func_ent);
//
//    pure_call_t proc(std::move(op.location), {});
//    proc.positioned_args.emplace_back(std::move(op.left), op.start());
//    proc.positioned_args.emplace_back(std::move(op.right), op.location); // ~
//    return func_ent->find(ctx, proc);
//    // to do: type matching. or should the find method take into account result type?
//}
//
//expression_visitor::result_type expression_visitor::operator()(binary_operator_t<binary_operator_type::PLUS>, binary_expression_t& op) const
//{
//    auto func_ent = dynamic_pointer_cast<functional_entity>(ctx.u().eregistry().find(ctx.u().make_qname_identifier("'+"sv)));
//    BOOST_ASSERT(func_ent);
//
//    THROW_NOT_IMPLEMENTED_ERROR();
//}

expression_visitor::result_type expression_visitor::operator()(context_identifier const& ci) const
{
    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor context_identifier");

#if 0
    if (!expected_result) {
        return make_error<basic_general_error>(ce.name.location, "no context type to resolve the case expression"sv, ce.name.value);
    }

    return apply_visitor(
        expression_cast_to_enum_visitor{ ctx, ce,  [this, &ce] { return error_context{ce, expected_result.location}; } },
        expected_result.value
    );
#endif
}

std::expected<function_entity const*, error_storage> expression_visitor::handle_property_get(annotated_identifier id) const
{
    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor::handle_property_get");

#if 0
    if (auto const* po = sonia::get<bang_object_t>(&ctx.context_type); po) {
        if (auto const& pte = dynamic_cast<type_entity const*>(po->value); pte) {
            auto rg = pte->find_field_getter(ctx, id);
            if (!rg.has_value()) return std::unexpected(std::move(rg.error()));
            function_entity const* getter = rg.value();
            ctx.append_expression(semantic::invoke_function{ getter->name() });
            return getter;
        } else { // could be enum
            THROW_NOT_IMPLEMENTED_ERROR();
        }
    }
    return std::unexpected(make_error<left_not_an_object_error>(id.location, id.value, ctx.context_type));
#endif
}

expression_visitor::result_type expression_visitor::operator()(not_empty_expression_t& me) const
{
    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor::not_empty_expression_t");

#if 0
    auto subvis = expected_result
        ? expression_visitor{ ctx, expected_result_t{ expected_result.value || bang_tuple_t{}, expected_result.location } }
        : expression_visitor{ ctx, nullptr };

    if (auto opterr = apply_visitor(subvis, me.value); opterr) return opterr;
    if (auto* uotype = ctx.context_type.as<bang_union_t>(); uotype && uotype->has(bang_tuple_t{})) {
        ctx.append_expression(std::move(semantic::not_empty_condition_t{}));
        ctx.push_chain(get<semantic::not_empty_condition_t>(ctx.expressions().back()).branch);
        ctx.context_type = *uotype - bang_tuple_t{};
        return {};
    }
    return make_error<basic_general_error>(get_start_location(me), "the expression type is not optional"sv, me);
#endif
}

expression_visitor::result_type expression_visitor::operator()(member_expression_t const& me) const
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

    if (auto err = match->apply(ctx); err) return std::unexpected(std::move(err));
    return apply_cast(ctx.context_type, me);
    
#if 0
    /*
    if (auto* uotype = ctx.context_type.as<bang_union_t>(); me.is_object_optional && uotype && uotype->has(bang_tuple_t{})) {
        ctx.append_expression(std::move(semantic::not_empty_condition_t{}));
        ctx.push_chain(get<semantic::not_empty_condition_t>(ctx.expressions().back()).branch);
        ctx.context_type = *uotype - bang_tuple_t{};
    }
    */
    auto expgetter = handle_property_get(me.name);
    if (!expgetter.has_value()) return std::move(expgetter.error());

    function_entity const* getter = expgetter.value();
    return apply_cast(getter->result_type(), me);
#endif
}

#if 0
expression_visitor::result_type expression_visitor::operator()(property_expression& pe)  const
{
    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor property_expression");
    //if (ctx.context_type == bang_tuple_t{}) {
    //    return std::unexpected(make_error<left_not_an_object_error>(pe.name.location, pe.name.value, bang_tuple_t{}));
    //}
    auto expgetter = handle_property_get(pe.name);
    if (!expgetter.has_value()) return std::move(expgetter.error());

    function_entity const* getter = expgetter.value();
    return apply_cast(getter->result_type(), pe);
}
#endif

expression_visitor::result_type expression_visitor::operator()(expression_vector_t & vec) const
{
    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor expression_vector_t");
#if 0
    if (expected_result) {
        return apply_visitor(vector_expression_visitor{ ctx, vec, expected_result.location }, expected_result.value);
    }

    // no expected_result case
    bang_tuple_t rtype;
    rtype.fields.reserve(vec.elements.size());
    expression_visitor elemvis{ ctx, nullptr };
    for (expression_t& e : vec.elements) {
        if (auto opterr = apply_visitor(elemvis, e); opterr) return opterr;
        rtype.fields.emplace_back(ctx.context_type);
    }
    rtype.unpacked = true;
    return apply_cast(rtype, vec);
#endif
}

function_entity& expression_visitor::handle_lambda(lambda& l) const
{
    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor handle_lambda");
#if 0
    function_signature sig;
    sig.setup(ctx, l.parameters);
    sig.normilize(ctx);
    sig.build_mangled_id(ctx.u());
    declaration_visitor dvis{ ctx };
    return dvis.append_fnent(l, sig, l.body);
#endif
}

expression_visitor::result_type expression_visitor::operator()(lambda & l) const
{
    THROW_NOT_IMPLEMENTED_ERROR("expression_visitor lambda_t");

#if 0
    function_entity& fe = handle_lambda(l);
    ctx.context_type = fe.type();
    return {};
#endif
}

template <std::derived_from<pure_call_t> CallExpressionT>
expression_visitor::result_type expression_visitor::operator()(builtin_qnid qnid, CallExpressionT const& call) const
{
    auto match = ctx.find(qnid, call, expected_result);
    if (auto err = match->apply(ctx); err) return std::unexpected(std::move(err));
    return apply_cast(ctx.context_type, call);
}

expression_visitor::result_type expression_visitor::operator()(new_expression_t const& ne) const
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

    if (auto err = match->apply(ctx); err)
        return std::unexpected(std::move(err));

    return false;
}

expression_visitor::result_type expression_visitor::operator()(function_call_t const& proc) const
{
    //THROW_NOT_IMPLEMENTED_ERROR("expression_visitor function_call_t");

    //expression_visitor evis{ ctx, nullptr };
    //semantic_expression_pair epair = apply_visitor(evis, proc.subject);
    
    // no nee check uniqueness due to the named ellipsis support
    //if (auto it = std::ranges::adjacent_find(proc.named_args, {}, [](auto const& pair) { return std::get<0>(pair).value; }); it != proc.named_args.end()) {
    //    ++it; // get second
    //    auto const& aid = std::get<0>(*it);
    //    return make_error<basic_general_error>(aid.location, "repeated argument"sv, ctx.u().qnregistry().resolve(aid.value));
    //}

#if 0
    if (auto *pl = get<lambda_t>(&proc.fn_object); pl) {
        THROW_NOT_IMPLEMENTED_ERROR("expression_visitor function_call_t lambda_t");
#if 0
        function_entity& fnent = handle_lambda(*pl);
        //ctx.expressions.emplace_back(semantic::set_variable{ &fnent });
        fnent.materialize_call(ctx, proc);
        ctx.append_expression(semantic::truncate_values{ 1, !fnent.is_void() }); // remove fnobject
        ctx.context_type = fnent.signature().fn_type.result;
        return {};
#endif
    }
#endif

    ct_expression_visitor vis{ ctx, annotated_entity_identifier{ u().get(builtin_eid::qname), proc.location() } };
    auto qn_ent_id = apply_visitor(vis, proc.fn_object);
    if (!qn_ent_id) return std::unexpected(std::move(qn_ent_id.error()));
    qname_identifier_entity qname_ent = static_cast<qname_identifier_entity const&>(u().eregistry_get(*qn_ent_id));
    
    auto match = ctx.find(qname_ent.value(), proc, expected_result);
    if (!match) return std::unexpected(match.error());
    if (auto err = match->apply(ctx); err) return std::unexpected(std::move(err));
    return apply_cast(ctx.context_type, proc);

    //annotated_qname const* fnvar = get<annotated_qname>(&proc.fn_object);
    //if (!fnvar) {
    //    THROW_NOT_IMPLEMENTED_ERROR("fn object expression is not implemented yet");
    //}
    ////GLOBAL_LOG_INFO() << "function call: " << u().print(fnvar->name.value);
    //functional const* fnl = ctx.lookup_functional(fnvar->value);
    //if (!fnl) {
    //    return std::unexpected(make_error<undeclared_identifier_error>(*fnvar));
    //}
    //
    ////shared_ptr<entity> e = ctx.resolve_entity(fnvar->name.value);
    ////if (!e) [[unlikely]] {
    ////    return make_error<undeclared_identifier_error>(fnvar->name);
    ////}
    ////shared_ptr<functional_entity> fnl = dynamic_pointer_cast<functional_entity>(e);
    ////if (!fnl) [[unlikely]] {
    ////    // to do: can be variable
    ////    return make_error<basic_general_error>(fnvar->name.location, "is not callable"sv, fnvar->name.value);
    ////}
    //
    //return this->operator()(*fnl, proc);

    ////THROW_NOT_IMPLEMENTED_ERROR("expression_visitor::operator()(function_call_t");
}

expression_visitor::result_type expression_visitor::operator()(chained_expression_t&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

expression_visitor::result_type expression_visitor::operator()(opt_named_syntax_expression_list_t const& nel) const
{
    if (!expected_result) {
        // e.g. case: let val = (a, b, c)
        pure_call_t tuple_call{ nel.location };
        for (opt_named_syntax_expression_t const& ne : nel) {
            if (auto const* pname = ne.name(); pname) {
                tuple_call.emplace_back(annotated_identifier{pname->value}, ne.value());
            } else {
                tuple_call.emplace_back(ne.value());
            }
        }
        auto match = ctx.find(builtin_qnid::make_tuple, tuple_call, expected_result);
        if (!match) return std::unexpected(match.error());
       
        if (auto err = match->apply(ctx); err)
            return std::unexpected(std::move(err));
        return false; // no implicit cast
    }
        ////// build tuple
        //// build tuple signature
        //entity_signature isg{ u().get_tuple_qname_identifier() };
        //boost::container::small_vector<size_t, 8> codepoints;
        //codepoints.emplace_back(ctx.current_expressions_pointer().second);
        //for (opt_named_syntax_expression_t const& ne : nel) {
        //    if (auto res = apply_visitor(*this, ne.value()); !res) return res;
        //    if (auto const* pname = ne.name(); pname) {
        //        isg.push(pname->value, field_descriptor{ ctx.context_type, false});
        //    } else {
        //        isg.push(field_descriptor{ ctx.context_type, false });
        //    }
        //    codepoints.emplace_back(ctx.current_expressions_pointer().second);
        //}
        //THROW_NOT_IMPLEMENTED_ERROR();

    THROW_NOT_IMPLEMENTED_ERROR();
}

}
