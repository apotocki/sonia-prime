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

#include "sonia/bang/auxiliary.hpp"

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

inline base_expression_visitor::result_type base_expression_visitor::do_result(entity_identifier eid, bool casted) const noexcept
{
    return std::pair{ syntax_expression_result_t{ semantic::managed_expression_list{ u() }, eid }, casted };
}

template <typename ExprT>
inline base_expression_visitor::result_type base_expression_visitor::apply_cast(entity const& ent, ExprT const& e) const
{
    BOOST_ASSERT(ent.id());
    if (!expected_result || u().get(builtin_eid::any) == expected_result.value) 
        return do_result(ent.id(), false);
    if (!is_type_expected) {
        if (ent.id() == expected_result.value)
            return do_result(ent.id(), false);
        return std::unexpected(make_error<cast_error>(get_start_location(e), expected_result.value, ent.id(), e));
        //THROW_NOT_IMPLEMENTED_ERROR("ct_expression_visitor::apply_cast const cast check");
    } else {
        entity_identifier typeeid = ent.get_type();
        if (typeeid == expected_result.value)
            return do_result(ent.id(), false);

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
base_expression_visitor::result_type base_expression_visitor::apply_cast(syntax_expression_result_t er, ExprT const& e) const
{
    //THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor::apply_cast(semantic::managed_expression_list, ExprT const&)");
    if (!get<0>(er)) {
        return apply_cast(get<1>(er), e);
    }
    
    if (!expected_result || get<1>(er) == expected_result.value || expected_result.value == u().get(builtin_eid::any)) {
        return std::pair{ std::move(er), false };
    }

    //if (expected_result.value == u().get(builtin_eid::any)) {
    //    ctx.context_type = expected_result.value;
    //    return std::pair{ std::move(l), false };
    //}

    //GLOBAL_LOG_DEBUG() << ("expected type: %1%, actual type: %2%"_fmt % u().print(expected_result.value) % u().print(typeeid)).str();

    lex::resource_location expr_loc = get_start_location(e);
    pure_call_t cast_call{ expected_result.location };
    cast_call.emplace_back(indirect_value{
        .location = expr_loc,
        .type = get<1>(er),
        .store = indirect_value_store_t{ in_place_type<semantic::indirect_expression_list>, std::move(get<0>(er)) }
    });

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
    get<0>(er).splice_back(get<0>(*res));
    get<1>(er) = get<1>(*res);
    return std::pair{ std::move(er), true };
}

template <typename ExprT>
inline base_expression_visitor::result_type base_expression_visitor::apply_cast(std::expected<syntax_expression_result_t, error_storage> r, ExprT const& e) const
{
    if (!r) return std::unexpected(std::move(r.error()));
    return apply_cast(std::move(*r), e);
    //return apply_visitor(make_functional_visitor<result_type>([this, e](auto& v) -> result_type {
    //    return apply_cast(std::move(v), e);
    //}), *r);
}

base_expression_visitor::result_type base_expression_visitor::operator()(indirect_value const& v) const
{
    if (auto const* pel = dynamic_cast<semantic::indirect_expression_list const*>(v.store.get_pointer()); pel) {
        return apply_cast(syntax_expression_result_t{ std::move(pel->list), v.type }, v);
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
    auto optqnid = ctx.lookup_qname(aqn);
    if (optqnid) {
        entity const& ent = u().make_qname_entity(*optqnid);
        return apply_cast(ent, aqn);
    }
    return std::unexpected(optqnid.error());
}

base_expression_visitor::result_type base_expression_visitor::operator()(bang_vector_t const& bv) const
{
    // to do: take into account the expected type
    // v.type can be a type or a value
    result_type res = apply_visitor(base_expression_visitor{ ctx }, bv.type);
    if (!res) return std::unexpected(res.error());
    syntax_expression_result_t& er = res->first;
    if (!get<0>(er)) { // constexpr
        // type or constexpr value?
        entity const& e = get_entity(u(), get<1>(er));
        if (e.get_type() == u().get(builtin_eid::typename_)) { // type
            return apply_cast(u().make_vector_type_entity(get<1>(er)), bv);
        } else {
            THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor bang_vector_t");
        }
    } else {
        get<1>(er) = u().make_array_type_entity(ctx.context_type, 1).id();
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
    optional<semantic::managed_expression_list> optexprs;
    //small_vector<entity_identifier, 16> ct_element_results;
    //small_vector<semantic::expression_span, 16> rt_element_results;
    boost::container::small_flat_set<entity_identifier, 8> element_types;

    small_vector<entity_identifier, 16> ct_elements;
    mp::integer mask{ 0 };
    size_t not_constant_count{ 0 };

    entity_signature const* expected_result = nullptr;
    fn_compiler_context& ctx;
    base_expression_visitor const& bvis;

    inline explicit array_expression_processor(base_expression_visitor const& bv) : ctx{ bv.ctx }, bvis{ bv } {}
    inline unit& u() const noexcept { return ctx.u(); }

    base_expression_visitor::result_type process(array_expression_t const& ve)
    {
        for (syntax_expression_t const& ee : ve.elements) {
            auto res = apply_visitor(base_expression_visitor{ ctx }, ee);
            if (!res) return std::unexpected(res.error());
            syntax_expression_result_t& er = res->first;
            if (!get<0>(er)) { // constexpr
                ct_elements.push_back(get<1>(er));
                element_types.insert(get_entity(u(), get<1>(er)).get_type());
            } else {
                mask |= (mp::integer{1} << ct_elements.size());
                ct_elements.push_back(get<1>(er));
                ++not_constant_count;
                if (!optexprs) {
                    optexprs.emplace(std::move(get<0>(er)));
                } else {
                    optexprs->splice_back(get<0>(er));
                }
                element_types.insert(get_entity(u(), get<1>(er)).get_type());
            }
        }

        entity_identifier elem_type = element_types.size() == 1 ?
            *element_types.begin() : ctx.u().make_union_type_entity(element_types).id();

        if (!optexprs) {
            if (element_types.size() == 1) { // all elements have the same type => return array
                return bvis.apply_cast(u().make_array_entity(*element_types.begin(), ct_elements), ve);
            }
            // return tuple of constant values
            entity_signature sig{ u().get(builtin_qnid::tuple), u().get(builtin_eid::metaobject) };
            for (auto const& eid : ct_elements) {
                sig.emplace_back(eid, true);
            }
            indirect_signatured_entity smpl{ sig };
            return bvis.apply_cast(u().eregistry_find_or_create(smpl, [&sig]() {
                return make_shared<basic_signatured_entity>(std::move(sig));
            }), ve);
        }
        if (element_types.size() == 1 && ct_elements.size() == not_constant_count) {
            // all elements have the same type and not const => return array
            if (not_constant_count > 1) {
                u().push_back_expression(*optexprs, semantic::push_value{ mp::integer{ not_constant_count } });
                u().push_back_expression(*optexprs, semantic::invoke_function(u().get(builtin_eid::arrayify)));
            }
            entity_identifier rt = u().make_array_type_entity(*element_types.begin(), not_constant_count).id();
            return bvis.apply_cast(syntax_expression_result_t{ std::move(*optexprs), rt }, ve);
        }
        // return fuzzy array
        entity_signature sig{ u().get(builtin_qnid::fuzzy_array), u().get(builtin_eid::typename_) };
        sig.push_back(u().get(builtin_id::mask), field_descriptor{ u().make_integer_entity(mask).id(), true });
        for (auto const& eid : ct_elements) {
            sig.emplace_back(eid, true);
        }
        indirect_signatured_entity smpl{ sig };
        entity_identifier rt = u().eregistry_find_or_create(smpl, [&sig]() {
            return make_shared<basic_signatured_entity>(std::move(sig));
        }).id();
        return bvis.apply_cast(syntax_expression_result_t{ std::move(*optexprs), rt }, ve);
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
            return apply_cast(syntax_expression_result_t{ std::move(exprs), eid_or_var.type }, var);
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

base_expression_visitor::result_type base_expression_visitor::operator()(function_call_t const& proc) const
{
    ct_expression_visitor vis{ ctx, annotated_entity_identifier{ u().get(builtin_eid::qname), proc.location } };
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
    syntax_expression_result_t& er = res->first;
    if (!get<0>(er)) { // const expression
        entity const& ent = get_entity(u(), get<1>(er));
        if (ent.get_type() == u().get(builtin_eid::typename_)) { // this is array type declaration
            auto szres = apply_visitor(ct_expression_visitor{ ctx, annotated_entity_identifier{ u().get(builtin_eid::integer), get_start_location(ie.index) } }, ie.index);
            if (!szres) return std::unexpected(szres.error());
            integer_literal_entity const& index_ent = static_cast<integer_literal_entity const&>(u().eregistry_get(*szres));
            if (index_ent.value() <= 0) {
                return std::unexpected(make_error<basic_general_error>(get_start_location(ie.index), "index must be greater than 0"sv));
            }
            return apply_cast(u().make_array_type_entity(get<1>(er), (size_t)index_ent.value()).id(), ie);
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
    BOOST_ASSERT(op.args.size() == 2);
    assign_expression_visitor lvis{ ctx, op.location, op.args[1].value() };

    auto res = apply_visitor(lvis, op.args[0].value());
    if (!res) return std::unexpected(std::move(res.error()));

    return apply_cast(std::move(res), op);

    //ctx.context_type = ctx.u().get(builtin_eid::void_);
    //return std::pair{ semantic::managed_expression_list{ ctx.u() }, false };
}

}