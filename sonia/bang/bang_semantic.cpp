//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "semantic.hpp"

#include <algorithm>

#include "ast/fn_compiler_context.hpp"
#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

void entity::visit(entity_visitor const& v) const
{
    v(*this);
}

entity_identifier entity::get_type() const noexcept
{
    if (auto const* psig = signature(); psig && psig->result) {
        return psig->result->entity_id();
    }
    return {};
}

std::ostream& entity::print_to(std::ostream& os, unit const& u) const
{
    return os; // << "@E"sv << id().value;
}

std::ostream& signatured_entity::print_to(std::ostream& os, unit const& u) const
{
    entity_signature const& sgn = *signature();
    if (sgn.result->entity_id() == u.get(builtin_eid::typename_)) {
        os << "typename "sv;
    }
    return os << u.print(sgn);
    //return entity::print_to(os, u) << "<"sv << u.print(*signature()) << ">"sv;
}

void append_semantic_result(semantic::expression_list_t& el, syntax_expression_result_t& dest, syntax_expression_result_t& src)
{
    if (!src.is_const_result) {
        dest.branches_expressions = el.concat(dest.branches_expressions, src.branches_expressions);
        dest.expressions = el.concat(dest.expressions, src.expressions);
        dest.temporaries.insert(dest.temporaries.end(), src.temporaries.begin(), src.temporaries.end());
    }
}

namespace semantic {

class managed_expression_result
{
public:
    mutable managed_expression_list store_el;
    small_vector<std::tuple<identifier, local_variable, semantic::expression_span>, 2> temporaries;
    semantic::expression_span branches_expressions;
    semantic::expression_span expressions;

    managed_expression_result(unit& u, expression_list_t& el, syntax_expression_result_t&& res) noexcept
        : store_el{ u }
        , branches_expressions{ std::move(res.branches_expressions) }
        , expressions{ std::move(res.expressions) }
    {
        store_el.splice_back(el, res.branches_expressions);

        temporaries.reserve(res.temporaries.size());
        for (auto const& tmp : res.temporaries) {
            temporaries.emplace_back(tmp);
            store_el.splice_back(el, get<2>(tmp));
        }

        store_el.splice_back(el, res.expressions);
    }

    managed_expression_result(managed_expression_result const& rhs)
        : store_el{ *rhs.store_el.manager() }
        , branches_expressions{ store_el.deep_copy(rhs.branches_expressions) }
        , expressions{ store_el.deep_copy(rhs.expressions) }
    {
        temporaries.reserve(rhs.temporaries.size());
        for (auto const& tmp : rhs.temporaries) {
            temporaries.emplace_back(std::tuple{ get<0>(tmp), get<1>(tmp), store_el.deep_copy(get<2>(tmp)) });
        }
    }
};

class indirect_expression_result : public indirect
{
    std::unique_ptr<managed_expression_result> impl;

public:
    SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(indirect_expression_result);

    inline indirect_expression_result(unit& u, expression_list_t& el, syntax_expression_result_t&& res)
        : impl{ std::make_unique<managed_expression_result>(u, el, std::move(res)) }
    {}

    inline indirect_expression_result(indirect_expression_result const& rhs)
        : impl{ std::make_unique<managed_expression_result>(*rhs.impl) }
    {}

    inline indirect_expression_result(indirect_expression_result&&) = default;

    inline indirect_expression_result& operator= (indirect_expression_result const&)
    {
        THROW_INTERNAL_ERROR("indirect_expression_list copy assignment");
    }

    inline indirect_expression_result& operator=(indirect_expression_result&&) = default;

    inline managed_expression_result const& operator*() const noexcept { return *impl; }
};

}

indirect_value make_indirect_value(unit& u, semantic::expression_list_t & el, syntax_expression_result_t && res, lex::resource_location loc)
{
    return indirect_value{
        .location = std::move(loc),
        .type = res.type(),
        .store = indirect_value_store_t{ in_place_type<semantic::indirect_expression_result>, u, el, std::move(res) }
    };
}

syntax_expression_result_t retrieve_indirect_value(unit&u, semantic::expression_list_t& el, indirect_value const& iv)
{
    auto const* pel = dynamic_cast<semantic::indirect_expression_result const*>(iv.store.get_pointer());
    if (!pel) {
        THROW_INTERNAL_ERROR("retrieve_indirect_value : indirect_value unexpected store type");
    }

    syntax_expression_result_t result{
        .value_or_type = iv.type,
        .is_const_result = false
    };
    semantic::managed_expression_list tmp_el{ u };
    semantic::managed_expression_result const& mer = **pel;
    result.branches_expressions = tmp_el.deep_copy(mer.branches_expressions);
    el.splice_back(tmp_el, result.branches_expressions);
    result.temporaries.reserve(mer.temporaries.size());
    for (auto const& tmp : mer.temporaries) {
        result.temporaries.emplace_back(std::tuple{ get<0>(tmp), get<1>(tmp), tmp_el.deep_copy(get<2>(tmp)) });
        el.splice_back(tmp_el, get<2>(result.temporaries.back()));
    }
    result.expressions = tmp_el.deep_copy(mer.expressions);
    el.splice_back(tmp_el, result.expressions);
    BOOST_ASSERT(tmp_el.empty());
    return result;
}

//vector_type_entity::vector_type_entity(unit& u, entity_identifier et) noexcept
//    : element_type{ et }
//    , sig_{ u.get(builtin_qnid::vector) }
//{
//    set_type(u.get(builtin_eid::typename_));
//    sig_.push_back(u.get(builtin_id::element), field_descriptor{ et, true });
//}
//
//bool vector_type_entity::equal(entity const& rhs) const noexcept
//{
//    if (vector_type_entity const* pr = dynamic_cast<vector_type_entity const*>(&rhs); pr) {
//        return pr->element_type == element_type;
//    }
//    return false;
//}
//
//size_t vector_type_entity::hash() const noexcept
//{
//    size_t seed = __COUNTER__;
//    hash_combine(seed, element_type);
//    return seed;
//}

//std::ostream& vector_type_entity::print_to(std::ostream& os, unit const& u) const
//{
//    return u.eregistry_get(get_type()).print_to(os << '[', u) << ']';
//}



#if 0
class type_mangler_visitor : static_visitor<qname_identifier>
{
    unit & u_;

    inline qname_identifier qnid(identifier id) const { return u_.fregistry().resolve(qname{ id }).id(); }
    inline qname_identifier qnid(string_view sv) const { return qnid(u_.slregistry().resolve(sv)); }

public:
    explicit type_mangler_visitor(unit& u) : u_{ u } {}

    //inline result_type operator()(bang_any_t) const { return qnid("any"sv); }
    //inline result_type operator()(bang_bool_t) const { return qnid("bool"sv); }
    //inline result_type operator()(bang_int_t) const { return qnid("int"sv); }
    //inline result_type operator()(bang_float_t) const { return qnid("float"sv); }
    //inline result_type operator()(bang_decimal_t) const { return qnid("decimal"sv); }
    //inline result_type operator()(bang_string_t) const { return qnid("string"sv); }
        
    inline result_type operator()(bang_object_t const& obj) const { 
        THROW_NOT_IMPLEMENTED_ERROR("type_mangler_visitor bang_object_t");
        //return obj.name();
    }

    inline result_type operator()(bang_fn_type_t const& fn) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
        /*
        apply_visitor(*this, fn.arg);
        ss << "->";
        apply_visitor(*this, fn.result);
        */
    }
    inline result_type operator()(bang_vector_t const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
        /*
        ss << '[';
        apply_visitor(*this, v.type);
        ss << ']';
        */
    }
    inline result_type operator()(bang_array_t const& arr) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
        /*
        apply_visitor(*this, arr.type);
        ss << '[' << arr.size << ']';
        */
    }
    inline result_type operator()(bang_tuple_t const& tpl) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
        /*
        ss << '(';
        for (auto const& f : tpl.fields) {
            if (&f != &tpl.fields.front()) ss << ',';
            apply_visitor(*this, f);
        }
        ss << ')';
        */
    }
    inline result_type operator()(bang_bunion_t const& tpl) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
    inline result_type operator()(bang_union_t const& tpl) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
        /*
        for (auto const& f : tpl.members) {
            if (&f != &tpl.members.front()) ss << "||";
            apply_visitor(*this, f);
        }
        */
    }
};
#endif

#if 0
void function_signature::setup(fn_compiler_context& ctx, parameter_woa_list_t & params)
{
    preliminary_type_visitor tqvis{ ctx };
    for (auto & parampair : params) {
        bang_type paramtype = apply_visitor(tqvis, parampair.type.value);
        if (!parampair.name.external_name) {
            position_parameters().emplace_back(paramtype);
        } else {
            //GLOBAL_LOG_INFO() << "APPEND param: " << ctx.u().print(parampair.name->id);
            named_parameters().emplace_back(*parampair.name.external_name, paramtype);
        }
    }
}


void function_signature::normilize(fn_compiler_context& ctx)
{
    std::sort(named_parameters().begin(), named_parameters().end(), [](auto const& l, auto const& r) {
        return std::get<0>(l) < std::get<0>(r);
    });
    //GLOBAL_LOG_INFO() << "---------->";
    //for (auto k = named_parameters.begin(); k != named_parameters.end(); ++k) {
    //    GLOBAL_LOG_INFO() << "LIST param: " << ctx.u().print(std::get<0>(*k).id);
    //}
    //GLOBAL_LOG_INFO() << "<----------";
    for (auto it = named_parameters().begin(), eit = named_parameters().end();;) {
        auto fit = std::adjacent_find(it, eit, [](auto const& l, auto const& r) {
            return std::get<0>(l) == std::get<0>(r);
        });
        if (fit == eit) break;
        auto dupit = fit; ++dupit;
        if (std::get<1>(*fit) != std::get<1>(*dupit)) [[unlikely]] {
            auto const& loc = std::get<0>(*dupit).location;
            auto const& origloc = std::get<0>(*fit).location;
            // "parameter redefinition"sv
            throw exception(ctx.u().print(identifier_redefinition_error{ std::get<0>(*fit), origloc }));
        } // else skip identical
        it = dupit;
    }
    
    // now remove identical parameters
    auto it = std::unique(named_parameters().begin(), named_parameters().end(), [](auto const& l, auto const& r) {
        return std::get<0>(l) == std::get<0>(r);
    });
    named_parameters().erase(it, named_parameters().end());
    
    // recover sort order
    std::sort(named_parameters().begin(), named_parameters().end(), [](auto const& l, auto const& r) {
        return std::get<0>(l) < std::get<0>(r);
    });
}

//void function_signature::build_symbol(unit& u, symbol& s)
//{
//    THROW_NOT_IMPLEMENTED_ERROR("function_signature::build_symbol");
//}

void function_signature::build_mangled_id(unit& u)
{
    type_mangler_visitor vis{ u };

    std::vector<qname_identifier> ps;

    for (bang_type const& postp : position_parameters()) {
        ps.emplace_back(apply_visitor(vis, postp));
    }
    for (auto const& [aname, tp] : named_parameters()) {
        ps.emplace_back(u.qnregistry().resolve(qname{ aname.value }));
        ps.emplace_back(apply_visitor(vis, tp));
    }
    mangled_id = u.piregistry().resolve(ps);
}


bang_type make_union_type(bang_type arg0, bang_type const* parg1)
{
    if (!parg1) return arg0;
    bang_union_t result;
    if (bang_union_t const* pu1 = get<bang_union_t>(parg1); pu1) {
        result = *pu1;
    } else {
        result.append(*parg1);
    }
    result.append(std::move(arg0));
    if (result.size() == 1) {
        return *result.begin();
    } else {
        return result;
    }
}

bang_type operator|| (bang_type const& l, bang_type const& r)
{
    bang_union_t result;
    if (bang_union_t const* pu1 = get<bang_union_t>(&l); pu1) {
        result = *pu1;
    } else if (bang_bunion_t const* pbu1 = get<bang_bunion_t>(&l); pbu1) {
        result.append(pbu1->true_type);
        result.append(pbu1->false_type);
    } else {
        result.append(l);
    }
    if (bang_bunion_t const* pbu2 = get<bang_bunion_t>(&l); pbu2) {
        result.append(pbu2->true_type);
        result.append(pbu2->false_type);
    } else {
        result.append(r);
    }
    if (result.size() == 1) {
        return *result.begin();
    } else {
        return result;
    }
}

struct union_type_remove_visitor : static_visitor<bool>
{
    bang_union_t& res;
    explicit union_type_remove_visitor(bang_union_t& r) : res{ r } {}
    using basic_type = bang_union_t::basic_type;

    //bool operator()(bang_bool_t const&) const { res.basic_members &= ~(uint16_t)basic_type::bool_e; return true; }
    //bool operator()(bang_int_t const&) const { res.basic_members &= ~(uint16_t)basic_type::int_e; return true; }
    //bool operator()(bang_float_t const&) const { res.basic_members &= ~(uint16_t)basic_type::float_e; return true; }
    //bool operator()(bang_decimal_t const&) const { res.basic_members &= ~(uint16_t)basic_type::decimal_e; return true; }
    //bool operator()(bang_string_t const&) const { res.basic_members &= ~(uint16_t)basic_type::string_e; return true; }
    bool operator()(bang_tuple_t const& t) const
    {
        if (!t.empty()) return false;
        res.basic_members &= ~(uint16_t)basic_type::nil_e;
        return true;
    }

    template <typename T>
    bool operator()(T const& t) const { return false; }
};

bang_type operator- (bang_union_t const& l, bang_type const& r)
{
    bang_union_t result = l;
    if (!apply_visitor(union_type_remove_visitor{ result }, r)) {
        auto it = std::lower_bound(result.other_members.begin(), result.other_members.end(), bang_type{ r });
        if (it != result.other_members.end() && *it == r) {
            result.other_members.erase(it);
        }
    }
    if (result.size() == 1) {
        return *result.begin();
    } else {
        return result;
    }
}
#endif
}
