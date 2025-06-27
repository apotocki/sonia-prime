//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "base_expression_visitor.hpp"

#include <boost/container/flat_set.hpp>

#include "assign_expression_visitor.hpp"

#include "fn_compiler_context.hpp"
#include "ct_expression_visitor.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

base_expression_visitor::base_expression_visitor(fn_compiler_context& c, semantic::expression_list_t& el) noexcept
    : ctx{ c }
    , expressions{ el }
{}

base_expression_visitor::base_expression_visitor(fn_compiler_context& c, semantic::expression_list_t& el, expected_result_t er) noexcept
    : ctx{ c }
    , expressions{ el }
    , expected_result{ std::move(er) }
{}

unit& base_expression_visitor::u() const noexcept
{
    return ctx.u();
}

template <typename ExprT>
inline base_expression_visitor::result_type base_expression_visitor::apply_cast(entity_identifier eid, ExprT const& e) const
{
    return apply_cast(get_entity(u(), eid), syntax_expression_result_t{ .value_or_type = eid, .is_const_result = true }, e);
}

base_expression_visitor::result_type base_expression_visitor::apply_cast(entity const& ent, syntax_expression_result_t er, syntax_expression_t const& e) const
{
    BOOST_ASSERT(er.is_const_result);
    bool is_modifier_compatible = expected_result.can_be_constexpr();
    if (is_modifier_compatible && (!expected_result.type || expected_result.type == u().get(builtin_eid::any) || ent.get_type() == expected_result.type)) {
        return std::pair{ std::move(er), false };
    }
    BOOST_ASSERT(ent.id);
    BOOST_ASSERT(expected_result);

    lex::resource_location expr_loc = get_start_location(e);
    pure_call_t cast_call{ expected_result.location };
    cast_call.emplace_back(annotated_entity_identifier{ ent.id, expr_loc });

    auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, expressions, expected_result);
    if (!match) {
        // ignore casting error details
        //return std::unexpected(make_error<cast_error>(expr_loc /*expected_result.location*/, expected_result.type, typeeid, e));
        return std::unexpected(append_cause(
            make_error<cast_error>(expr_loc, expected_result.type, ent.get_type(), e),
            std::move(match.error())
        ));
    }

    auto res = match->apply(ctx);
    if (!res)
        return std::unexpected(std::move(res.error()));
    res->expressions = expressions.concat(er.expressions, res->expressions);
    res->temporaries.insert(res->temporaries.begin(), er.temporaries.begin(), er.temporaries.end());
    return std::pair{ std::move(*res), true };
}

base_expression_visitor::result_type base_expression_visitor::apply_cast(syntax_expression_result_t er, syntax_expression_t const& e) const
{
    if (er.is_const_result) {
        return apply_cast(get_entity(u(), er.value()), std::move(er), e);
    }

    bool is_modifier_compatible = expected_result.is_modifier_compatible(er);
    if (is_modifier_compatible && (!expected_result.type || expected_result.type == u().get(builtin_eid::any) || er.type() == expected_result.type)) {
        return std::pair{ std::move(er), false };
    }
    
    //GLOBAL_LOG_DEBUG() << ("expected type: %1%, actual type: %2%"_fmt % u().print(expected_result.value) % u().print(typeeid)).str();

    lex::resource_location expr_loc = get_start_location(e);
    pure_call_t cast_call{ expected_result.location };
    semantic::managed_expression_list el{ ctx.u() };
    el.deep_copy(er.expressions);
    cast_call.emplace_back(indirect_value{
        .location = expr_loc,
        .type = er.type(),
        .store = indirect_value_store_t{ in_place_type<semantic::indirect_expression_list>, std::move(el) }
    });

    auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, expressions, expected_result);
    if (!match) {
        // ignore casting error details
        return std::unexpected(make_error<cast_error>(expr_loc /*expected_result.location*/, expected_result.type, er.type(), e));
        //return std::unexpected(append_cause(
        //    make_error<cast_error>(expr_loc /*expected_result.location*/, expected_result.value, typeeid, e),
        //    std::move(match.error())
        //));
    }

    auto res = match->apply(ctx);
    if (!res) return std::unexpected(std::move(res.error()));
    return std::pair{ std::move(*res), true };
}

template <typename ExprT>
inline base_expression_visitor::result_type base_expression_visitor::apply_cast(std::expected<syntax_expression_result_t, error_storage> r, ExprT const& e) const
{
    if (!r) return std::unexpected(std::move(r.error()));
    syntax_expression_t se = e;
    return apply_cast(std::move(*r), se);
    //return apply_visitor(make_functional_visitor<result_type>([this, e](auto& v) -> result_type {
    //    return apply_cast(std::move(v), e);
    //}), *r);
}

base_expression_visitor::result_type base_expression_visitor::operator()(indirect_value const& v) const
{
    if (auto const* pel = dynamic_cast<semantic::indirect_expression_list const*>(v.store.get_pointer()); pel) {
        semantic::managed_expression_list el{ ctx.u() };
        el.deep_copy((semantic::expression_span)pel->list);
        semantic::expression_span elsp = el;
        expressions.splice_back(el);
        return apply_cast(syntax_expression_result_t{ .expressions = std::move(elsp), .value_or_type = v.type, .is_const_result = false }, v);
    }
    THROW_INTERNAL_ERROR("base_expression_visitor::operator()(indirect_value const& v) unexpected indorect store type");
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
    entity const& ent = u().make_qname_entity(aqn.value);
    return apply_cast(ent, aqn);

    //auto optqnid = ctx.lookup_qname(aqn);
    //if (optqnid) {
    //    entity const& ent = u().make_qname_identifier_entity(*optqnid);
    //    return apply_cast(ent, aqn);
    //}
    //return std::unexpected(optqnid.error());
}

base_expression_visitor::result_type base_expression_visitor::operator()(bang_vector_t const& bv) const
{
    // to do: take into account the expected type
    // v.type can be a type or a value
    result_type res = apply_visitor(base_expression_visitor{ ctx, expressions }, bv.type);
    if (!res) return std::unexpected(res.error());
    syntax_expression_result_t& er = res->first;
    if (er.is_const_result) { // constexpr
        // type or constexpr value?
        entity const& e = get_entity(u(), er.value());
        if (e.get_type() == u().get(builtin_eid::typename_)) { // type
            return apply_cast(u().make_vector_type_entity(er.value()), bv);
        } else {
            THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor bang_vector_t");
        }
    } else {
        er.value_or_type = u().make_array_type_entity(er.type(), 1).id;
        return apply_cast(std::move(er), bv);
    }
}

#if 0
struct array_expression_processor : static_visitor<void>
{
    optional<semantic::managed_expression_list> optexprs;
    small_vector<entity_identifier, 16> ct_element_results;
    small_vector<semantic::expression_span, 16> rt_element_results;
    boost::container::small_flat_set<entity_identifier, 8> element_types;
    fn_compiler_context& ctx;
    base_expression_visitor const& bvis;

    inline explicit array_expression_processor(base_expression_visitor const& bv) : ctx{ bv.ctx }, bvis{ bv } {}
    inline unit& u() const noexcept { return ctx.u(); }

    base_expression_visitor::result_type process(array_expression_t const& ve)
    {
        for (syntax_expression_t const& ee : ve.elements) {
            auto res = apply_visitor(base_expression_visitor{ ctx }, ee);
            if (!res) return std::unexpected(res.error());
            apply_visitor(*this, res->first);
        }

        entity_identifier elem_type = element_types.size() == 1 ?
            *element_types.begin() : ctx.u().make_union_type_entity(element_types).id();
        
        if (rt_element_results.empty()) {
            return bvis.apply_cast(u().make_array_entity(elem_type, ct_element_results), ve);
        }
        BOOST_ASSERT(optexprs);
        if (rt_element_results.size() > 1) {
            u().push_back_expression(*optexprs, semantic::push_value{ mp::integer{ rt_element_results.size() } });
            u().push_back_expression(*optexprs, semantic::invoke_function(u().get(builtin_eid::arrayify)));
        }
        ctx.context_type = u().make_array_type_entity(elem_type, rt_element_results.size()).id();
        return bvis.apply_cast(std::move(*optexprs), ve);
    }

    void operator()(entity_identifier const& eid)
    {
        element_types.insert(get_entity(u(), eid).get_type());
        if (rt_element_results.empty()) {
            ct_element_results.push_back(eid);
        } else {
            u().push_back_expression(*optexprs, semantic::push_value{ eid });
            rt_element_results.emplace_back(&optexprs->back_entry());
        }
    }

    void operator()(semantic::managed_expression_list& l)
    {
        semantic::expression_span argspan = l;
        if (!optexprs) {
            optexprs.emplace(u());
            for (entity_identifier eid : ct_element_results) {
                u().push_back_expression(*optexprs, semantic::push_value{ eid });
                rt_element_results.emplace_back(&optexprs->back_entry());
            }
        } else {
            optexprs->splice_back(l);
        }
        rt_element_results.push_back(std::move(argspan));
        element_types.insert(ctx.context_type);
    }
};
#endif

struct array_expression_processor : static_visitor<void>
{
    syntax_expression_result_t aeres;
    //small_vector<entity_identifier, 16> ct_element_results;
    //small_vector<semantic::expression_span, 16> rt_element_results;
    boost::container::small_flat_set<entity_identifier, 8> element_types;

    small_vector<entity_identifier, 16> ct_elements;
    mp::integer mask{ 0 };
    size_t not_constant_count{ 0 };

    entity_signature const* expected_result = nullptr;
    fn_compiler_context& ctx;
    base_expression_visitor const& bvis;

    inline explicit array_expression_processor(base_expression_visitor const& bv) 
        : ctx{ bv.ctx }
        , bvis{ bv }
    {}

    inline unit& u() const noexcept { return ctx.u(); }

    base_expression_visitor::result_type process(array_expression_t const& ve)
    {
        for (syntax_expression_t const& ee : ve.elements) {
            auto res = apply_visitor(base_expression_visitor{ ctx, bvis.expressions }, ee);
            if (!res) return std::unexpected(res.error());
            syntax_expression_result_t& er = res->first;
            if (er.is_const_result) { // constexpr
                ct_elements.push_back(er.value());
                element_types.insert(get_entity(u(), er.value()).get_type());
            } else {
                mask |= (mp::integer{1} << ct_elements.size());
                ct_elements.push_back(er.type());
                ++not_constant_count;
                element_types.insert(get_entity(u(), er.type()).get_type());
            }
            aeres.expressions = bvis.expressions.concat(aeres.expressions, er.expressions);
            aeres.temporaries.insert(aeres.temporaries.end(), er.temporaries.begin(), er.temporaries.end());
        }

        entity_identifier elem_type = element_types.size() == 1 ?
            *element_types.begin() : ctx.u().make_union_type_entity(element_types).id;

        aeres.is_const_result = !mask;

        if (aeres.is_const_result) {
            if (element_types.size() == 1) { // all elements have the same type => return array
                entity const& arr_ent = u().make_array_entity(*element_types.begin(), ct_elements);
                aeres.value_or_type = arr_ent.id;
                return bvis.apply_cast(arr_ent, std::move(aeres), ve);
            }
            // return tuple of constant values
            entity_signature sig{ u().get(builtin_qnid::tuple), u().get(builtin_eid::typename_) };
            for (auto const& eid : ct_elements) {
                sig.emplace_back(eid, true);
            }
            indirect_signatured_entity smpl{ sig };
            entity const& tpl_ent = u().eregistry_find_or_create(smpl, [&sig]() {
                return make_shared<basic_signatured_entity>(std::move(sig));
            });
            aeres.value_or_type = tpl_ent.id;
            return bvis.apply_cast(tpl_ent, std::move(aeres), ve);
        }

        if (element_types.size() == 1 && ct_elements.size() == not_constant_count) {
            // all elements have the same type and not const => return array
            if (not_constant_count > 1) {
                semantic::managed_expression_list tmpel{ ctx.u() };
                tmpel.splice_back(bvis.expressions, aeres.expressions);
                u().push_back_expression(tmpel, semantic::push_value{ mp::integer{ not_constant_count } });
                u().push_back_expression(tmpel, semantic::invoke_function(u().get(builtin_eid::arrayify)));
                aeres.expressions = tmpel;
                bvis.expressions.splice_back(tmpel);
            }
            aeres.value_or_type = u().make_array_type_entity(*element_types.begin(), not_constant_count).id;
            return bvis.apply_cast(std::move(aeres), ve);
        }
        // return fuzzy array
        entity_signature sig{ u().get(builtin_qnid::fuzzy_array), u().get(builtin_eid::typename_) };
        sig.emplace_back(u().get(builtin_id::mask), u().make_integer_entity(mask).id, true);
        for (auto const& eid : ct_elements) {
            sig.emplace_back(eid, true);
        }
        indirect_signatured_entity smpl{ sig };
        aeres.value_or_type = u().eregistry_find_or_create(smpl, [&sig]() {
            return make_shared<basic_signatured_entity>(std::move(sig));
        }).id;
        return bvis.apply_cast(std::move(aeres), ve);
    }

    //void operator()(entity_identifier const& eid)
    //{
    //    ct_elements.push_back(eid);
    //    element_types.insert(get_entity(u(), eid).get_type());
    //}

    //void operator()(semantic::managed_expression_list& l)
    //{
    //    mask |= (mp::integer{1} << ct_elements.size());
    //    ct_elements.push_back(ctx.context_type);
    //    ++not_constant_count;
    //    if (!optexprs) {
    //        optexprs.emplace(std::move(l));
    //    } else {
    //        optexprs->splice_back(l);
    //    }
    //    element_types.insert(get_entity(u(), ctx.context_type).get_type());
    //}
};


base_expression_visitor::result_type base_expression_visitor::operator()(array_expression_t const& ve) const
{
    array_expression_processor proc{ *this };
    return proc.process(ve);
}

base_expression_visitor::result_type base_expression_visitor::operator()(variable_reference const& var) const
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
            semantic::expression_span exprs_span;
            u().push_back_expression(expressions, exprs_span, semantic::push_local_variable{ eid_or_var });
            return apply_cast(syntax_expression_result_t{ .expressions = std::move(exprs_span), .value_or_type = eid_or_var.type, .is_const_result = false }, var);
        }
    }), optent);
}

base_expression_visitor::result_type base_expression_visitor::operator()(member_expression_t const& me) const
{
    pure_call_t get_call{ me.start() };
    get_call.emplace_back(annotated_identifier{ u().get(builtin_id::self), get_start_location(me.object) }, me.object);
    get_call.emplace_back(annotated_identifier{ u().get(builtin_id::property), get_start_location(me.property) }, me.property);

    auto match = ctx.find(builtin_qnid::get, get_call, expressions, expected_result);

    if (!match) {
        return std::unexpected(append_cause(
            make_error<basic_general_error>(me.start(), "can't resolve"sv, syntax_expression_t{ me }),
            std::move(match.error())
        ));
    }

    return apply_cast(match->apply(ctx), me);
}

//base_expression_visitor::result_type base_expression_visitor::operator()(context_identifier const& ci) const
//{
//    if (!expected_result) {
//        return std::unexpected(make_error<basic_general_error>(ci.start, "no context type to resolve the context identifier"sv, ci.name.value));
//    }
//    THROW_EOF_ERROR("base_expression_visitor context_identifier");
//}

#if 0
base_expression_visitor::result_type base_expression_visitor::operator()(named_syntax_expression_list_t const& nel) const
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
#endif

base_expression_visitor::result_type base_expression_visitor::operator()(unary_expression_t const& be) const
{
    switch (be.op) {
    case unary_operator_type::NEGATE:
        return this->operator()(builtin_qnid::negate, be);
    case unary_operator_type::DEREF:
        return this->operator()(builtin_qnid::deref, be);
    case unary_operator_type::ELLIPSIS:
        return this->operator()(builtin_qnid::ellipsis, be);
    }
    THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor unary_expression_t");
}

template <typename FnIdT, std::derived_from<pure_call_t> ExprT>
inline base_expression_visitor::result_type base_expression_visitor::operator()(FnIdT && fnid, ExprT const& call) const
{
    auto match = ctx.find(std::forward<FnIdT>(fnid), call, expressions, expected_result);
    if (!match) return std::unexpected(match.error());
    return apply_cast(match->apply(ctx), call);
}

base_expression_visitor::result_type base_expression_visitor::operator()(function_call_t const& proc) const
{
    base_expression_visitor vis{ ctx, expressions, expected_result_t{ .type = u().get(builtin_eid::qname), .location = proc.location, .modifier = parameter_constraint_modifier_t::const_type } };
    auto fn_ent_id = apply_visitor(vis, proc.fn_object);
    if (!fn_ent_id) return std::unexpected(std::move(fn_ent_id.error()));
    BOOST_ASSERT(!fn_ent_id->first.expressions);
    entity const& ent = get_entity(u(), fn_ent_id->first.value());

    if (qname_identifier_entity const* pqnent = dynamic_cast<qname_identifier_entity const*>(&ent); pqnent) {
        return (*this)(pqnent->value(), proc);
    } else if (qname_entity const* pqnent = dynamic_cast<qname_entity const*>(&ent); pqnent) {
        if (!pqnent->value()) { // make tuple
            return (*this)(u().get(builtin_qnid::make_tuple), proc);
        }
        auto optqnid = ctx.lookup_qname(annotated_qname{ pqnent->value(), get_start_location(proc.fn_object) });
        if (!optqnid) {
            return std::unexpected(optqnid.error());
        }
        return (*this)(*optqnid, proc);
    } else {
        return std::unexpected(make_error<basic_general_error>(get_start_location(proc.fn_object), "functional object is expected"sv, proc.fn_object));
    }
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
    auto match = ctx.find(builtin_qnid::new_, new_call, expressions, expected_result);
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
    auto res = apply_visitor(base_expression_visitor{ ctx, expressions }, ie.base);
    if (!res) return std::unexpected(res.error());
    syntax_expression_result_t& er = res->first;
    if (er.is_const_result) { // const expression
        entity const& ent = get_entity(u(), er.value());
        if (ent.get_type() == u().get(builtin_eid::typename_)) { // this is array type declaration
            auto szres = apply_visitor(ct_expression_visitor{ ctx, expressions, expected_result_t{ u().get(builtin_eid::integer), get_start_location(ie.index) } }, ie.index);
            if (!szres) return std::unexpected(szres.error());
            BOOST_ASSERT(!szres->expressions); // not impelemented const value expressions
            integer_literal_entity const& index_ent = static_cast<integer_literal_entity const&>(get_entity(u(), szres->value));
            if (index_ent.value() <= 0) {
                return std::unexpected(make_error<basic_general_error>(get_start_location(ie.index), "index must be greater than 0"sv));
            }
            entity const& type_ent = u().make_array_type_entity(er.value(), (size_t)index_ent.value());
            er.value_or_type = type_ent.id;
            return apply_cast(type_ent, std::move(er), ie);
        }
    }

    // else get call
    pure_call_t get_call{ ie.location };
    get_call.emplace_back(annotated_identifier{ u().get(builtin_id::self) }, ie.base);
    get_call.emplace_back(annotated_identifier{ u().get(builtin_id::property) }, ie.index);

    auto match = ctx.find(builtin_qnid::get, get_call, expressions, expected_result);
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
    case binary_operator_type::CONCAT:
        return this->operator()(builtin_qnid::string_concat, be);
    case binary_operator_type::LOGIC_AND:
        return this->operator()(builtin_qnid::logic_and, be);
        //return do_logic_and(be);
    case binary_operator_type::LOGIC_OR:
        return do_logic_or(be);
    case binary_operator_type::ASSIGN:
        return do_assign(be);
    default:
        break;
    }
    
    THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor binary_expression_t");
}

base_expression_visitor::result_type base_expression_visitor::do_logic_and(binary_expression_t const& be) const
{
#if 0
    prepared_call pcall{ ctx, be, expressions };
    if (auto err = pcall.prepare(); err) return std::unexpected(std::move(err));

    expected_result_t bool_exp{ u().get(builtin_eid::boolean), be.location };
    auto call_session = pcall.new_session(ctx);
    syntax_expression_t const* parg_expr;
    auto firstarg = call_session.use_next_positioned_argument(&parg_expr);
    if (!firstarg) {
        if (!firstarg.error()) {
            return std::unexpected(make_error<basic_general_error>(be.location, "missing first argument for logic AND operation"sv));
        }
        return std::unexpected(firstarg.error());
    }
    fn_compiler_context_scope fn_scope{ ctx };
    lex::resource_location first_expr_loc = get_start_location(*parg_expr);
    pure_call_t bool_cast_call{ first_expr_loc };
    local_variable* first_expr_var = nullptr;
    identifier first_expr_var_name;
    if (firstarg->is_const_result) {
        bool_cast_call.emplace_back(annotated_entity_identifier{ firstarg->value(), first_expr_loc });
    } else {
        first_expr_var_name = u().new_identifier();
        first_expr_var = &fn_scope.new_temporary(first_expr_var_name, firstarg->type());
        bool_cast_call.emplace_back(variable_reference{ annotated_qname{ qname{ first_expr_var_name, false } }, false });
    }
    auto match = ctx.find(builtin_qnid::implicit_cast, bool_cast_call, expressions, expected_result_t{ u().get(builtin_eid::boolean), first_expr_loc });
    if (!match) {
        // ignore casting error details
        return std::unexpected(make_error<cast_error>(first_expr_loc, u().get(builtin_eid::boolean), firstarg->type(), *parg_expr));
    }
    auto firstarg_res = match->apply(ctx);
    if (!firstarg_res) {
        return std::unexpected(std::move(firstarg_res.error()));
    }

    if (firstarg_res->is_const_result) {
        if (firstarg_res->value() == u().get(builtin_eid::false_)) {
            // if the first argument is false, return it without evaluation of the second argument
            return apply_cast(std::move(*firstarg), *parg_expr);
        } else if (firstarg->value() == u().get(builtin_eid::true_)) {
            // if the first argument is true, we need to evaluate and return the second argument
            auto secondarg = call_session.use_next_positioned_argument(&parg_expr);
            if (!secondarg) {
                if (!secondarg.error()) {
                    return std::unexpected(make_error<basic_general_error>(be.location, "missing second argument for logic AND operation"sv));
                }
                return std::unexpected(secondarg.error());
            }
            if (auto argterm = call_session.unused_argument(); argterm) {
                return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
            }
            return apply_cast(std::move(*secondarg), *parg_expr);
        } else {
            // unexpected constant value for logic AND operation
            return std::unexpected(make_error<basic_general_error>(be.location, "internal error: unexpected constant value for logic AND operation"sv, firstarg->value()));
        }
    }
    
    auto secondarg = call_session.use_next_positioned_argument(&parg_expr);
    if (!secondarg) {
        if (!secondarg.error()) {
            return std::unexpected(make_error<basic_general_error>(be.location, "missing second argument for logic AND operation"sv));
        }
        return std::unexpected(secondarg.error());
    }
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    entity_identifier secondarg_type;
    if (secondarg->is_const_result) {
        secondarg_type = get_entity(u(), secondarg->value()).get_type();
    } else {
        secondarg_type = secondarg->type();
    }

    // calculate result type
    entity_identifier result_type;
    bool result_is_union;
    if (firstarg->type() == secondarg_type) {
        result_type = secondarg_type;
        result_is_union = false;
    } else {
        entity_identifier argtypes[2] = { firstarg->type(), secondarg_type };
        result_type = u().make_union_type_entity(argtypes).id;
        result_is_union = true;
    }
    
    BOOST_ASSERT(!firstarg->is_const_result);
    BOOST_ASSERT(first_expr_var);
    syntax_expression_result_t& result = *firstarg_res;
    result.value_or_type = result_type;
    result.temporaries.emplace_back(first_expr_var->varid, first_expr_var->type, firstarg->expressions);
    u().push_back_expression(expressions, result.expressions, semantic::conditional_t{});
    semantic::conditional_t& cond = get<semantic::conditional_t>(result.expressions.back());
    if (result_is_union) {
        pure_call_t union_first_arg_cast_call{ first_expr_loc };
        union_first_arg_cast_call.emplace_back(variable_reference{ annotated_qname{ qname{ first_expr_var_name, false } }, false });
        auto match = ctx.find(builtin_qnid::implicit_cast, union_first_arg_cast_call, expressions, expected_result_t{ result_type, first_expr_loc });
        if (!match) {
            // ignore casting error details
            return std::unexpected(make_error<cast_error>(first_expr_loc, u().get(builtin_eid::boolean), firstarg->type(), *parg_expr));
        }
        auto union_first_arg_cast_res = match->apply(ctx);
        if (!union_first_arg_cast_res) return std::unexpected(std::move(union_first_arg_cast_res.error()));
        cond.false_branch = union_first_arg_cast_res->expressions;
        
    }
    u().push_back_expression(expressions, cond.false_branch, semantic::push_local_variable{ *first_expr_var });

    result.temporaries.push_back(first_expr_var ? first_expr_var->id : first_expr_var_name.id);
    // Create a result structure for the entire logic AND operation
    syntax_expression_result_t result;
    
    
    // Evaluate the left operand
    auto left_res = apply_visitor(base_expression_visitor{ ctx, el }, be.args[0].value());
    if (!left_res) return std::unexpected(left_res.error());

    // cast the left operand's result to boolean
    pure_call_t cast_call{ get_start_location(be.args[0]expected_result.location };
    cast_call.emplace_back(annotated_entity_identifier{ ent.id, expr_loc });
    auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, expressions, expected_result);

    expected_result_t{ u().get(builtin_eid::boolean), be.location }
    // Add the left operand's expressions and temporaries to our result
    result.expressions = expressions.concat(result.expressions, left_res->first.expressions);
    result.temporaries.insert(result.temporaries.end(), left_res->first.temporaries.begin(), left_res->first.temporaries.end());

    // Create a conditional for short-circuit evaluation
    semantic::managed_expression_list cond_exprs{ ctx.u() };
    semantic::conditional_t cond;
    
    // Create the true branch (right operand evaluation)
    semantic::managed_expression_list true_branch{ ctx.u() };
    
    // Evaluate the right operand
    auto right_res = apply_visitor(base_expression_visitor{ ctx, expressions, expected_result_t{ u().get(builtin_eid::boolean), be.location } }, be.args[1].value());
    if (!right_res) return std::unexpected(right_res.error());
    
    // Add the right operand's expressions to the true branch
    true_branch.splice_back(expressions, right_res->first.expressions);
    result.temporaries.insert(result.temporaries.end(), right_res->first.temporaries.begin(), right_res->first.temporaries.end());
    
    // Finalize the conditional
    cond.true_branch = true_branch;
    cond.true_branch_finished = 1;
    cond.false_branch_finished = 1;
    
    // Add the conditional to our expressions
    u().push_back_expression(cond_exprs, std::move(cond));
    semantic::expression_span cond_span = cond_exprs;
    expressions.splice_back(cond_exprs);
    
    // Add the conditional to our result
    result.expressions = expressions.concat(result.expressions, cond_span);
    result.value_or_type = u().get(builtin_eid::boolean);
    result.is_const_result = false;
    
    // Apply cast if needed and return the result
    return apply_cast(std::move(result), be);
#endif
    THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor binary_operator_type::LOGIC_AND");
}

base_expression_visitor::result_type base_expression_visitor::do_logic_or(binary_expression_t const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor binary_operator_type::LOGIC_OR");
}

base_expression_visitor::result_type base_expression_visitor::do_assign(binary_expression_t const& op) const
{
    //THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor binary_operator_type::ASSIGN");
    //GLOBAL_LOG_INFO() << "left expression: " << ctx.u().print(op.left);
    //size_t start_result_pos = result.size();
    BOOST_ASSERT(op.args.size() == 2);
    assign_expression_visitor lvis{ ctx, expressions, op.location, op.args[1].value() };

    auto res = apply_visitor(lvis, op.args[0].value());
    if (!res) return std::unexpected(std::move(res.error()));

    return apply_cast(std::move(res), op);

    //ctx.context_type = ctx.u().get(builtin_eid::void_);
    //return std::pair{ semantic::managed_expression_list{ ctx.u() }, false };
}

}