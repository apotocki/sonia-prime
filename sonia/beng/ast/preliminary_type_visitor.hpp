//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/variant.hpp"
#include "sonia/optional.hpp"
#include "sonia/utility/scope_exit.hpp"

#include "../ast.hpp"
#include "../semantic.hpp"
#include "expression_visitor.hpp"
#include "compiler_context.hpp"
#include "../entities/type_entity.hpp"
#include "../entities/enum_entity.hpp"

#include "sonia/utility/functional/variant_compare_three_way.hpp"

namespace sonia::lang::beng {

struct preliminary_type_visitor : static_visitor<beng_type>
{
    compiler_context& ctx;
    //beng_generic_type const& tp;
    //std::vector<semantic_expression_type>& result;

    preliminary_type_visitor(compiler_context& c /*, beng_generic_type const& t, std::vector<semantic_expression_type>& r*/)
        : ctx{ c }
        //, tp{ t }
        //, result{ r }
    {}

    inline result_type operator()(beng_bool_t const& v) const { return v; }
    inline result_type operator()(beng_int_t const& v) const { return v; }
    inline result_type operator()(beng_float_t const& v) const { return v; }
    inline result_type operator()(beng_decimal_t const& v) const { return v; }
    inline result_type operator()(beng_string_t const& v) const { return v; }
    inline result_type operator()(beng_preliminary_object_t const& v) const
    {
        auto pe = ctx.resolve_entity(v.name());
        if (dynamic_cast<type_entity const*>(pe.get()) || dynamic_cast<enum_entity const*>(pe.get())) {
            return beng_object_t{ pe.get() };
        }
        if (!pe) {
            throw exception("%1%(%2%,%3%): `%4%`: undeclared identifier"_fmt %
                v.location().resource % v.location().line % v.location().column %
                ctx.u().print(v.name())
            );
        }
        throw exception("%1%(%2%,%3%): `%4%`: identifier is not a type, see declaration at %5%(%6%,%7%)"_fmt %
            v.location().resource % v.location().line % v.location().column %
            ctx.u().print(v.name()) %
            pe->location().resource % pe->location().line % pe->location().column
        );
    }

    inline result_type operator()(beng_preliminary_vector_t const& v) const
    {
        return beng_vector_t{ apply_visitor(*this, v.type) };
    }

    inline result_type operator()(beng_preliminary_array_t const& v) const
    {
        return beng_array_t{ apply_visitor(*this, v.type), v.size };
    }

    inline result_type operator()(beng_preliminary_tuple_t const& v) const
    {
        beng_tuple_t result;
        result.fields.reserve(v.fields.size());
        for (auto t : v.fields) {
            result.fields.emplace_back(apply_visitor(*this, t));
        }
        return result;
    }

    inline result_type operator()(beng_preliminary_fn_t const& v) const
    {
        return beng_fn_t{ apply_visitor(*this, v.arg), apply_visitor(*this, v.result) };
    }

    inline result_type operator()(beng_preliminary_union_t const& v) const
    {
        beng_union_t result;
        size_t reserved_size = v.members.size();
        result.members.reserve(reserved_size);
        for (auto t : v.members) {
            auto rt = apply_visitor(*this, t);
            if (auto * put = get<beng_union_t>(&rt); put) {
                if (!put->members.empty()) {
                    reserved_size += result.members.size() - 1;
                    result.members.reserve(reserved_size);
                    std::move(put->members.begin(), put->members.end(), std::back_inserter(result.members));
                }
            } else {
                result.members.emplace_back(std::move(rt));
            }
        }
        
        std::sort(result.members.begin(), result.members.end(),
            [](beng_type const& l, beng_type const& r) { return variant_compare_three_way{}(l, r) == std::strong_ordering::less; });

        auto eit = std::unique(result.members.begin(), result.members.end());
        result.members.erase(eit, result.members.end());

        return result;
    }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

}