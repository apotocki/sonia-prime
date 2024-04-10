//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"
#include "sonia/optional.hpp"
#include "sonia/utility/scope_exit.hpp"

#include "../semantic.hpp"
#include "expression_visitor.hpp"
#include "fn_compiler_context.hpp"
#include "../entities/type_entity.hpp"
#include "../entities/enum_entity.hpp"

#include "sonia/utility/functional/variant_compare_three_way.hpp"
#include "sonia/beng/errors.hpp"

namespace sonia::lang::beng {

struct preliminary_type_visitor : static_visitor<beng_type>
{
    fn_compiler_context& ctx;
    //beng_generic_type const& tp;
    //std::vector<semantic_expression_type>& result;

    preliminary_type_visitor(fn_compiler_context& c /*, beng_generic_type const& t, std::vector<semantic_expression_type>& r*/)
        : ctx{ c }
        //, tp{ t }
        //, result{ r }
    {}

    inline result_type operator()(beng_bool_t const& v) const { return v; }
    inline result_type operator()(beng_int_t const& v) const { return v; }
    inline result_type operator()(beng_float_t const& v) const { return v; }
    inline result_type operator()(beng_decimal_t const& v) const { return v; }
    inline result_type operator()(beng_string_t const& v) const { return v; }
    inline result_type operator()(beng_preliminary_object_t & v) const
    {
        auto pe = ctx.resolve_entity(v.name());
        if (dynamic_cast<type_entity const*>(pe.get()) || dynamic_cast<enum_entity const*>(pe.get())) {
            return beng_object_t{ pe.get() };
        }
        if (!pe) {
            throw exception(ctx.u().print(undeclared_identifier_error(v.location(), v.name())));
        }
        throw exception(ctx.u().print(basic_general_error{v.location(),
            ("identifier is not a type, see declaration at %1%"_fmt % ctx.u().print(pe->location())).str(),
            v.name()
        }));
    }

    inline result_type operator()(beng_preliminary_vector_t & v) const
    {
        return beng_vector_t{ apply_visitor(*this, v.type) };
    }

    inline result_type operator()(beng_preliminary_array_t & v) const
    {
        return beng_array_t{ apply_visitor(*this, v.type), v.size };
    }

    inline beng_tuple_t operator()(beng_preliminary_tuple_t & v) const
    {
        beng_tuple_t result;

        // preseve the order of positioned fields
        auto toint = [](parameter<beng_preliminary_type> const& param) -> int { return param.name ? int(param.name->value.value) : -1; };
        std::ranges::stable_sort(v.fields, {}, toint);
        auto eit = v.fields.end();
        auto it = std::lower_bound(v.fields.begin(), eit, 0, [&toint](auto const& l, auto const& r) { return toint(l) < r; });
        if (it != v.fields.end()) {
            auto fit = std::adjacent_find(it, eit, [](auto const& l, auto const& r) { return l.name == r.name; });
            if (fit != eit) {
                auto dupit = fit; ++dupit;
                auto const& loc = dupit->name->location;
                auto const& origloc = fit->name->location;
                throw exception(ctx.u().print(basic_general_error{ loc,
                    ("parameter redefinition, see declaration at %1%"_fmt % ctx.u().print(origloc)).str(),
                    fit->name->value,
                }));
            }
        }
        size_t positioned_count = size_t(it - v.fields.begin());
        size_t named_count = v.fields.size() - positioned_count;

        result.fields.reserve(positioned_count);
        result.named_fields.reserve(named_count);
        
        for (auto t : v.fields) {
            auto tp = apply_visitor(*this, t.type);
            if (t.name) {
                result.named_fields.emplace_back(*t.name, std::move(tp));
            } else {
                result.fields.emplace_back(std::move(tp));
            }
        }
        // named fields sorted by their ids
        return result;
    }

    inline result_type operator()(beng_preliminary_fn_t & v) const
    {
        return beng_fn_t{ this->operator()(v.arg), apply_visitor(*this, v.result) };
    }

    inline beng_union_t operator()(beng_preliminary_union_t & v) const
    {
        beng_union_t result;
        size_t reserved_size = v.members.size();
        result.other_members.reserve(reserved_size);
        for (auto t : v.members) {
            auto rt = apply_visitor(*this, t);
            if (auto * put = get<beng_union_t>(&rt); put) {
                for (auto const& m : *put) {
                    result.append(m);
                }
            } else {
                result.append(std::move(rt));
            }
        }
        return result;
    }

    /*
    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
    */
};

}