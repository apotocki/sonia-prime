//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/optional.hpp"
//#include "sonia/utility/scope_exit.hpp"

#include "sonia/beng/semantic.hpp"
//#include "expression_visitor.hpp"
//#include "expression_vector_visitor.hpp"
#include "sonia/beng/ast/fn_compiler_context.hpp"

//#include "../entities/type_entity.hpp"

namespace sonia::lang::beng {

struct expression_cast_to_string_visitor : static_visitor<std::expected<beng_type, std::string>>
{
    fn_compiler_context& ctx;
    lex::resource_location const& loc_;
    context_locator_t cl_;

    expression_cast_to_string_visitor(fn_compiler_context& c, lex::resource_location const& loc, context_locator_t const& cl)
        : ctx{ c }
        , loc_{ loc }
        , cl_{ cl }
    {}

    /*
    result_type operator()(beng_vector_t const& v) const
    {
        size_t stored_pos = result.size();
        EXCEPTIONAL_SCOPE_EXIT([this, stored_pos](){ result.resize(stored_pos); });
        expression_visitor elemvis{ ctx, result, &v.type };
        for (expression_t const& e : vec.elements) {
            if (!apply_visitor(elemvis, e)) {
                result.resize(stored_pos);
                return nullopt;
            }
        }
        result.emplace_back(semantic::push_value{ decimal{ vec.elements.size() } });
        result.emplace_back(semantic::invoke_function{ ctx.u().arrayify_entity_name(), (uint32_t)vec.elements.size() + 1 });
        return v;
    }

    result_type operator()(beng_array_t const& ar) const
    {
        if (ar.size != vec.elements.size() || !this->operator()(beng_vector_t{ ar.type })) return nullopt;
        return ar;
    }

    result_type operator()(beng_union_t const& v) const
    {
        for (beng_generic_type const& ut : v.members) {
            if (auto optrest = apply_visitor(*this, ut); optrest) {
                return *optrest;
            }
        }
        return nullopt;
    }

    inline result_type operator()(beng_bool_t const& v) const { return nullopt; }
    inline result_type operator()(beng_int_t const& v) const { return nullopt; }
    inline result_type operator()(beng_float_t const& v) const { return nullopt; }
    inline result_type operator()(beng_decimal_t const& v) const { return nullopt; }
    inline result_type operator()(beng_string_t const& v) const { return nullopt; }
    inline result_type operator()(beng_object_t const& v) const { return nullopt; }
    

    inline result_type operator()(beng_object_t const& v) const
    {
        if (beng_type{ v } == expected_tp) return expected_tp;
        if (auto const* pte = dynamic_cast<type_entity const*>(v.value); pte) {
            if (pte->try_cast(ctx, result, expected_tp)) {
                return expected_tp;
            }
        }
        return nullopt;
    }

    inline result_type operator()(beng_vector_t const& v) const
    {
        expression_cast_to_vector_visitor vis{ ctx, v, result };
        return apply_visitor(vis, type2cast);
    }
    */

    inline result_type operator()(beng_vector_t const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
        //if (target.type == v.type) return target;
        //return nullopt;
    }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

}
