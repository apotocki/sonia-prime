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

namespace sonia::lang::bang {

struct expression_type_visitor : static_visitor<optional<bang_type>>
{
    fn_compiler_context& ctx;
    bang_type const& expected_tp;
    std::vector<semantic::expression_type>& result;

    expression_type_visitor(fn_compiler_context& c, bang_type const& t, std::vector<semantic::expression_type>& r)
        : ctx{ c }
        , expected_tp{ t }
        , result{ r }
    {}

    /*
    result_type operator()(bang_vector_t const& v) const
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

    result_type operator()(bang_array_t const& ar) const
    {
        if (ar.size != vec.elements.size() || !this->operator()(bang_vector_t{ ar.type })) return nullopt;
        return ar;
    }

    result_type operator()(bang_union_t const& v) const
    {
        for (bang_generic_type const& ut : v.members) {
            if (auto optrest = apply_visitor(*this, ut); optrest) {
                return *optrest;
            }
        }
        return nullopt;
    }

    inline result_type operator()(bang_bool_t const& v) const { return nullopt; }
    inline result_type operator()(bang_int_t const& v) const { return nullopt; }
    inline result_type operator()(bang_float_t const& v) const { return nullopt; }
    inline result_type operator()(bang_decimal_t const& v) const { return nullopt; }
    inline result_type operator()(bang_string_t const& v) const { return nullopt; }
    inline result_type operator()(bang_object_t const& v) const { return nullopt; }
    */

    inline result_type operator()(bang_object_t const& v) const
    {
        if (bang_type{ v } == expected_tp) return expected_tp;
        if (auto const* pte = dynamic_cast<type_entity const*>(v.value); pte) {
            if (pte->try_cast(ctx, expected_tp)) {
                return expected_tp;
            }
        }
        return nullopt;
    }

    inline result_type operator()(bang_vector_t const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
        //expression_vector_visitor vis{ ctx };
    }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

}