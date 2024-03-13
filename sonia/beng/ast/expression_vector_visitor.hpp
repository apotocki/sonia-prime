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

namespace sonia::lang::beng {

struct expression_vector_visitor : static_visitor<optional<beng_type>>
{
    compiler_context& ctx;
    expression_vector_t const& vec;
    std::vector<semantic_expression_type>& result;

    expression_vector_visitor(compiler_context& c, expression_vector_t const& v, std::vector<semantic_expression_type>& r)
        : ctx{ c }
        , vec { v }
        , result{ r }
    {}

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
        for (beng_type const& ut : v.members) {
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

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

}