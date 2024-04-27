//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "expression_visitor.hpp"
#include "expression_implicit_cast_visitor.hpp"
#include "expression_vector_visitor.hpp"
#include "expression_fn_visitor.hpp"
#include "expression_type_visitor.hpp"
#include "lvalue_expression_visitor.hpp"
#include "declaration_visitor.hpp"

#include "sonia/utility/scope_exit.hpp"

#include "fn_compiler_context.hpp"
#include "../entities/enum_entity.hpp"
#include "../entities/functional_entity.hpp"

#include "casts/expression_cast_to_enum_visitor.hpp"
#include "sonia/bang/errors.hpp"

#include <expected>

namespace sonia::lang::bang {

template <typename ExprT>
inline expression_visitor::result_type expression_visitor::apply_cast(bang_type const& t, ExprT const& e) const
{
    if (!expected_result || (t.which() == expected_result->type.which() && expected_result->type == t)) return t;
    return apply_visitor(
          expression_implicit_cast_visitor{ ctx, t, [this, &e] { return error_context{e, expected_result->location}; } }
        , expected_result->type);
}

expression_visitor::result_type expression_visitor::operator()(annotated_bool const& b) const
{
    ctx.append_expression(semantic::push_value{ b.value });
    return apply_cast(bang_bool_t{}, b);
}

expression_visitor::result_type expression_visitor::operator()(annotated_decimal const& d) const
{
    ctx.append_expression(semantic::push_value{ d.value });
    return apply_cast(bang_decimal_t{}, d);
}

expression_visitor::result_type expression_visitor::operator()(annotated_string const& s) const
{
    ctx.append_expression(semantic::push_value{ s.value });
    return apply_cast(bang_string_t{}, s);
}

expression_visitor::result_type expression_visitor::operator()(variable_identifier const& var) const
{
    shared_ptr<entity> e = ctx.resolve_entity(var.name.value);
    if (auto varptr = dynamic_cast<variable_entity*>(e.get()); varptr) {
        variable_entity::kind k = varptr->varkind();
        if (k == variable_entity::kind::EXTERN || k == variable_entity::kind::STATIC) {
            THROW_NOT_IMPLEMENTED_ERROR();
        }
        if (k == variable_entity::kind::SCOPE_LOCAL || k == variable_entity::kind::LOCAL) {
            if (!ctx.u().qnregistry().resolve(varptr->name()).parent().has_prefix(ctx.base_ns())) {
                if (k == variable_entity::kind::SCOPE_LOCAL || var.scope_local) {
                    return std::unexpected(make_error<basic_general_error>(var.name.location, "variable is not defined in the scope"sv, var.name.value));
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
        expression_fn_visitor dvis{ ctx, *fnptr, [this, &var] { return error_context{var, expected_result->location}; } };
        return apply_visitor(dvis, expected_result->type);
    }
    /*
    // to do: look for variable first
    if (variable_entity const* pv = ctx.resolve_variable(aqnm.name); pv) {
        result.emplace_back(semantic::push_variable{ pv });
        return pv->type();
    }
    */
    return std::unexpected(make_error<undeclared_identifier_error>(var.name));
}

expression_visitor::result_type expression_visitor::operator()(negate_expression_t & op) const
{
    expression_visitor rvis{ ctx, nullptr };
    auto rtype = apply_visitor(rvis, op.argument);
    if (!rtype.has_value()) return rtype;
    ctx.append_expression(semantic::invoke_function{ ctx.u().get_builtin_function(unit::builtin_fn::negate) });
    
    // "result of negated expression"
    return apply_cast(bang_bool_t{}, op);
}

expression_visitor::result_type expression_visitor::operator()(binary_expression_t<binary_operator_type::ASSIGN> & op) const
{
    //GLOBAL_LOG_INFO() << "left expression: " << ctx.u().print(op.left);

    //size_t start_result_pos = result.size();
    lvalue_expression_visitor lvis{ ctx };
    auto e = apply_visitor(lvis, op.left);
    if (!e.has_value()) return std::unexpected(e.error());

    // variable_entity
    if (function_entity const* fe = dynamic_cast<function_entity const*>(e.value()); fe) {
        // fe: (object, property value)->
        bang_type const& t = fe->signature().position_parameters().back();
        expression_visitor rvis{ ctx, expected_result_t{ t, op.location }};
        auto rtype = apply_visitor(rvis, op.right);
        if (!rtype.has_value()) return rtype;
        ctx.append_expression(semantic::invoke_function{ fe->name() });
        return std::move(rtype);
    } else if (variable_entity const* ve = dynamic_cast<variable_entity const*>(e.value()); ve) {
        expression_visitor rvis{ ctx, expected_result_t{ ve->type(), op.location }};
        auto rtype = apply_visitor(rvis, op.right);
        if (ve->is_weak()) {
            ctx.append_expression(semantic::invoke_function{ ctx.u().get_builtin_function(unit::builtin_fn::weak_create) });
        }
        ctx.append_expression(semantic::set_variable{ ve });
        if (ve->is_weak()) {
            ctx.append_expression(semantic::truncate_values(1, false));
        }
        return std::move(rtype);
    } else {
        // to do: functional entity assignment
        THROW_NOT_IMPLEMENTED_ERROR();
    }

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

    bang_type const* exprt = expected_result ? &expected_result->type : nullptr;
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

    bang_type const* exprt = expected_result ? &expected_result->type : nullptr;
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

expression_visitor::result_type expression_visitor::operator()(logic_and_expression_t& op) const
{
    auto largvis = expected_result
        ? expression_visitor{ ctx, expected_result_t{ bang_type{bang_any_t{}} || expected_result->type, expected_result->location }}
        : expression_visitor{ ctx, nullptr };

    auto ltype = apply_visitor(largvis, op.left);
    if (!ltype.has_value()) return ltype;

    ctx.append_expression(semantic::conditional_t{});
    semantic::conditional_t & cond = get<semantic::conditional_t>(ctx.expressions().back());
    auto st = ctx.expressions_state(); // pin state

    ctx.push_chain(cond.true_branch);
    ctx.append_expression(semantic::truncate_values(1, false)); // remove result of left expression
    expression_visitor rargvis{ ctx, expected_result };
    auto rtype = apply_visitor(rargvis, op.right);
    if (!rtype.has_value()) return rtype;
    st.restore_and_detach();
    
    bang_bunion_t const* pbur = get<bang_bunion_t>(&rtype.value());
    bang_bunion_t const* pbul = get<bang_bunion_t>(&ltype.value());

    bang_bunion_t res_type{
        /*true_type*/  pbur ? pbur->true_type : rtype.value(),
        /*false_type*/ (pbul ? pbul->false_type : ltype.value()) || (pbur ? pbur->false_type : rtype.value())
    };
    if (res_type.true_type == res_type.false_type) {
        return apply_cast(res_type.true_type, op);
    }
    return apply_cast(res_type, op);
}

expression_visitor::result_type expression_visitor::operator()(logic_or_expression_t& op) const
{
    auto largvis = expected_result
        ? expression_visitor{ ctx, expected_result_t{ bang_type{bang_any_t{}} || expected_result->type, expected_result->location }}
        : expression_visitor{ ctx, nullptr };
    
    auto ltype = apply_visitor(largvis, op.left);
    if (!ltype.has_value()) return ltype;

    ctx.append_expression(semantic::conditional_t{});
    semantic::conditional_t& cond = get<semantic::conditional_t>(ctx.expressions().back());
    auto st = ctx.expressions_state(); // pin state

    ctx.push_chain(cond.false_branch);
    ctx.append_expression(semantic::truncate_values(1, false)); // remove result of left expression
    expression_visitor rargvis{ ctx, expected_result };
    auto rtype = apply_visitor(rargvis, op.right);
    if (!rtype.has_value()) return rtype;
    st.restore_and_detach();

    bang_bunion_t const* pbur = get<bang_bunion_t>(&rtype.value());
    bang_bunion_t const* pbul = get<bang_bunion_t>(&ltype.value());

    bang_bunion_t res_type{
        /*true_type*/  (pbul ? pbul->true_type : ltype.value()) || (pbur ? pbur->true_type : rtype.value()),
        /*false_type*/ pbur ? pbur->false_type : rtype.value()
    };

    if (res_type.true_type == res_type.false_type) {
        return apply_cast(res_type.true_type, op);
    }
    return apply_cast(res_type, op);
}

expression_visitor::result_type expression_visitor::operator()(binary_expression_t<binary_operator_type::CONCAT>& op) const
{
    // find a function
    auto func_ent = dynamic_pointer_cast<functional_entity>(ctx.u().eregistry().find(ctx.u().make_qname_identifier("concat"sv)));
    BOOST_ASSERT(func_ent);

    pure_call_t proc(std::move(op.location), {});
    proc.positioned_args.emplace_back(std::move(op.left), op.start());
    proc.positioned_args.emplace_back(std::move(op.right), op.location); // ~
    auto et = func_ent->find(ctx, proc);
    if (!et.has_value()) return et;
    // to do: type matching. or should the find method take into account result type?
    return et;
}

expression_visitor::result_type expression_visitor::operator()(case_expression const& ce) const
{
    if (!expected_result) {
        return std::unexpected(make_error<basic_general_error>(ce.name.location, "no context type to resolve the case expression"sv, ce.name.value));
    }

    expression_cast_to_enum_visitor vis{ ctx, ce,  [this, &ce] { return error_context{ce, expected_result->location}; } };

    return apply_visitor(vis, expected_result->type);
}

expression_visitor::result_type expression_visitor::operator()(member_expression_t & me) const
{
    auto otype = apply_visitor(expression_visitor{ ctx, nullptr }, me.object);
    if (!otype.has_value()) return std::move(otype);
    if (auto* uotype = otype.value().as<bang_union_t>(); me.is_object_optional && uotype && uotype->has(bang_tuple_t{})) {
        ctx.append_expression(std::move(semantic::not_empty_condition_t{}));
        ctx.push_chain(get<semantic::not_empty_condition_t>(ctx.expressions().back()).branch);
        otype = *uotype - bang_tuple_t{};
    }
    function_entity const* getter;
    if (auto const* po = sonia::get<bang_object_t>(&*otype); po) {
        if (auto const& pte = dynamic_cast<type_entity const*>(po->value); pte) {
             auto rg = pte->find_field_getter(ctx, me.name);
             if (!rg.has_value()) return std::unexpected(std::move(rg.error()));
             getter = rg.value();
        } else { // could be enum
            THROW_NOT_IMPLEMENTED_ERROR();
        }
    } else {
        return std::unexpected(make_error<left_not_an_object_error>(me.name.location, me.name.value, *otype));
    }

    ctx.append_expression(semantic::invoke_function{ getter->name() });
    return apply_cast(getter->result_type(), me);
}

expression_visitor::result_type expression_visitor::operator()(expression_vector_t & vec) const
{
    bang_tuple_t rtype;
    rtype.fields.reserve(vec.elements.size());
    expression_visitor elemvis{ ctx, nullptr };
    for (expression_t& e : vec.elements) {
        auto et = apply_visitor(elemvis, e);
        if (!et.has_value()) return et;
        rtype.fields.emplace_back(*et);
    }
    rtype.unpacked = true;
    return apply_cast(rtype, vec);

    /*
    if (!expected_result) { // is real case?
        rtype.fields.reserve(vec.elements.size());
        expression_visitor elemvis{ ctx, nullptr };
        for (expression_t & e : vec.elements) {
            rtype.fields.emplace_back(*apply_visitor(elemvis, e));
        }
        return std::move(rtype);
    }

    expression_vector_visitor evv{ ctx, vec, *expected_result_loc_ };
    if (auto optrest = apply_visitor(evv, *expected_result); optrest) {
        return *optrest;
    }
    
    rtype.fields.reserve(vec.elements.size());
    expression_visitor elemvis{ ctx, nullptr };
    for (expression_t & e : vec.elements) {
        rtype.fields.emplace_back(*apply_visitor(elemvis, e));
    }
    throw exception("can't convert the %1% to %2%"_fmt % ctx.u().print(bang_type{rtype}) % ctx.u().print(*expected_result));
    */
    /*
    bang_generic_type const* expected_elem_type = nullptr;
    if (expected_result) {
        if (bang_vector_t const* parr = get<bang_vector_t>(expected_result); parr) {
            expected_elem_type = &parr->type;

            expression_visitor elemvis{ ctx, result, expected_elem_type };
            for (expression_t const& e : vec.elements) {
                apply_visitor(elemvis, e);
            }
            result.emplace_back(semantic::push_value{ decimal{ vec.elements.size() } });
            result.emplace_back(semantic::invoke_function{ &ctx.u().arrayify_entity(), (uint32_t)vec.elements.size() + 1 });
            return *expected_result;
        }
        THROW_NOT_IMPLEMENTED_ERROR("expected type: %1%"_fmt % ctx.u().print(*expected_result));
    }

    
    return std::move(rtype);
    */
}

function_entity& expression_visitor::handle_lambda(lambda_t& l) const
{
    function_signature sig;
    sig.setup(ctx, l.parameters);
    sig.normilize(ctx);
    sig.build_mangled_id(ctx.u());
    declaration_visitor dvis{ ctx };
    return dvis.append_fnent(l, sig, l.body);
}

expression_visitor::result_type expression_visitor::operator()(lambda_t & l) const
{
    return handle_lambda(l).type();
}

expression_visitor::result_type expression_visitor::operator()(function_call_t & proc) const
{
    //expression_visitor evis{ ctx, nullptr };
    //semantic_expression_pair epair = apply_visitor(evis, proc.subject);
    
    // check uniqueness
    if (auto it = std::ranges::adjacent_find(proc.named_args, {}, [](auto const& pair) { return std::get<0>(pair).value; }); it != proc.named_args.end()) {
        ++it; // get second
        auto const& aid = std::get<0>(*it);
        return std::unexpected(make_error<basic_general_error>(aid.location, "repeated argument"sv, ctx.u().qnregistry().resolve(aid.value)));
    }

    if (auto *pl = get<lambda_t>(&proc.fn_object); pl) {
        function_entity& fnent = handle_lambda(*pl);
        //ctx.expressions.emplace_back(semantic::set_variable{ &fnent });
        fnent.materialize_call(ctx, proc);
        ctx.append_expression(semantic::truncate_values{ 1, !fnent.is_void() }); // remove fnobject
        return fnent.signature().fn_type.result;
    }

    variable_identifier const* fnvar = get<variable_identifier>(&proc.fn_object);
    if (!fnvar) {
        THROW_NOT_IMPLEMENTED_ERROR("fn object expression is not implemented yet");
    }
    //GLOBAL_LOG_INFO() << ctx.u().print(fnvar->name);
    shared_ptr<entity> e = ctx.resolve_entity(fnvar->name.value);
    if (!e) [[unlikely]] {
        return std::unexpected(make_error<undeclared_identifier_error>(fnvar->name));
    }
    shared_ptr<functional_entity> func_ent = dynamic_pointer_cast<functional_entity>(e);
    if (!func_ent) [[unlikely]] {
        // to do: can be variable
        return std::unexpected(make_error<basic_general_error>(fnvar->name.location, "is not callable"sv, fnvar->name.value));
    }
    
    return func_ent->find(ctx, proc);
    //if (!rtype.has_value()) return rtype;
    //return rtype;
}

expression_visitor::result_type expression_visitor::operator()(chained_expression_t&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

}
