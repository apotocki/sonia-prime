//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "semantic.hpp"

#include <algorithm>

#include "ast/fn_compiler_context.hpp"
#include "ast/preliminary_type_visitor.hpp"
#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

std::ostream& entity::print_to(std::ostream& os, unit const& u) const
{
    return os; // << "@E"sv << id().value;
}

std::ostream& signatured_entity::print_to(std::ostream& os, unit const& u) const
{
    return os << u.print(*signature());
    //return entity::print_to(os, u) << "<"sv << u.print(*signature()) << ">"sv;
}

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

    inline result_type operator()(bang_fn_t const& fn) const
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
#endif

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

}
