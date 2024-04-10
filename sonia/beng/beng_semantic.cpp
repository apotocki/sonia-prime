//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "semantic.hpp"

#include <algorithm>

#include "ast/fn_compiler_context.hpp"
#include "ast/preliminary_type_visitor.hpp"

namespace sonia::lang::beng {

struct type_mangler_visitor : static_visitor<qname>
{
    unit & u_;
    explicit type_mangler_visitor(unit & u) : u_{ u } {}

    inline result_type operator()(beng_bool_t) const { return qname{ u_.slregistry().resolve("bool"sv) }; }
    inline result_type operator()(beng_particular_bool_t) const { THROW_INTERNAL_ERROR("particular_bool can not be a part of a mangled name"); }
    inline result_type operator()(beng_int_t) const { return qname{ u_.slregistry().resolve("int"sv)}; }
    inline result_type operator()(beng_float_t) const { return qname{ u_.slregistry().resolve("float"sv) }; }
    inline result_type operator()(beng_decimal_t) const { return qname{ u_.slregistry().resolve("decimal"sv) }; }
    inline result_type operator()(beng_string_t) const { return qname{ u_.slregistry().resolve("string"sv) }; }
        
    inline result_type operator()(beng_object_t const& obj) const { return qname{ obj.name(), true }; }

    inline result_type operator()(beng_fn_t const& fn) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
        /*
        apply_visitor(*this, fn.arg);
        ss << "->";
        apply_visitor(*this, fn.result);
        */
    }
    inline result_type operator()(beng_vector_t const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
        /*
        ss << '[';
        apply_visitor(*this, v.type);
        ss << ']';
        */
    }
    inline result_type operator()(beng_array_t const& arr) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
        /*
        apply_visitor(*this, arr.type);
        ss << '[' << arr.size << ']';
        */
    }
    inline result_type operator()(beng_tuple_t const& tpl) const
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
    inline result_type operator()(beng_union_t const& tpl) const
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

void function_signature::setup(fn_compiler_context& ctx, parameter_woa_list_t & params)
{
    preliminary_type_visitor tqvis{ ctx };
    for (auto & parampair : params) {
        beng_type paramtype = apply_visitor(tqvis, parampair.type);
        if (!parampair.name) {
            position_parameters().emplace_back(paramtype);
        } else {
            //GLOBAL_LOG_INFO() << "APPEND param: " << ctx.u().print(parampair.name->id);
            named_parameters().emplace_back(*parampair.name, paramtype);
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
            throw exception(ctx.u().print(identifier_redefinition_error{ loc, origloc, std::get<0>(*fit).value }));
        } // else skip identical
        it = dupit;
    }
    
    // now remove identical oarameters
    auto it = std::unique(named_parameters().begin(), named_parameters().end(), [](auto const& l, auto const& r) {
        return std::get<0>(l) == std::get<0>(r);
    });
    named_parameters().erase(it, named_parameters().end());
    
    // recover sort order
    std::sort(named_parameters().begin(), named_parameters().end(), [](auto const& l, auto const& r) {
        return std::get<0>(l) < std::get<0>(r);
    });
}

void function_signature::build_mangled_id(unit& u)
{
    type_mangler_visitor vis{ u };

    std::vector<qname> ps;

    for (beng_type const& postp : position_parameters()) {
        ps.emplace_back(apply_visitor(vis, postp));
    }
    for (auto const& [aname, tp] : named_parameters()) {
        ps.emplace_back(qname{ aname.value });
        ps.emplace_back(apply_visitor(vis, tp));
    }
    mangled_id = u.piregistry().resolve(ps);
}

beng_type make_union_type(beng_type arg0, beng_type const* parg1)
{
    if (!parg1) return arg0;
    beng_union_t result;
    if (beng_union_t const* pu1 = get<beng_union_t>(parg1); pu1) {
        result = *pu1;
    } else {
        result.append(*parg1);
    }
    if (beng_union_t* pu0 = get<beng_union_t>(&arg0); pu0) {
        for (auto const& m : *pu0) { result.append(m); }
    } else {
        result.append(std::move(arg0));
    }

    if (result.size() == 1) {
        return *result.begin();
    } else {
        return result;
    }
}

}
