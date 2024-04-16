//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

//#include "sonia/utility/scope_exit.hpp"

#include "sonia/bang/semantic.hpp"
//#include "expression_visitor.hpp"
//#include "expression_vector_visitor.hpp"
#include "fn_compiler_context.hpp"

#include "sonia/bang/entities/type_entity.hpp"

#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

struct expression_cast_to_object_visitor : static_visitor<std::expected<bang_type, error_storage>>
{
    fn_compiler_context& ctx;
    bang_object_t const& target;
    context_locator_t cl_;

    expression_cast_to_object_visitor(fn_compiler_context& c, bang_object_t const& t, context_locator_t const& cl)
        : ctx{ c }
        , target{ t }
        , cl_{ cl }
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
    

    inline result_type operator()(bang_object_t const& v) const
    {
        if (bang_type{ v } == expected_tp) return expected_tp;
        if (auto const* pte = dynamic_cast<type_entity const*>(v.value); pte) {
            if (pte->try_cast(ctx, result, expected_tp)) {
                return expected_tp;
            }
        }
        return nullopt;
    }

    inline result_type operator()(bang_vector_t const& v) const
    {
        expression_cast_to_vector_visitor vis{ ctx, v, result };
        return apply_visitor(vis, type2cast);
    }
    */

    inline result_type operator()(bang_object_t const& v) const
    {
        if (v == target) return target;
        if (auto const* pte = dynamic_cast<type_entity const*>(v.value); pte) {
            if (pte->try_cast(ctx, target)) {
                return target;
            }
        }
        return std::unexpected(make_error<cast_error>(cl_(), target, v));
    }

    //inline result_type operator()(bang_bool_t const& b) const
    //{
    //    return std::unexpected(make_error<cast_error>(cl_(), target, b));
    //}

    template <typename T>
    result_type operator()(T const& v) const
    {
        return std::unexpected(make_error<cast_error>(cl_(), target, v));
    }
};

}