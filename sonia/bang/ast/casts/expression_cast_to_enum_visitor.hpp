//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <sstream>

//#include "sonia/utility/scope_exit.hpp"

#include "sonia/bang/semantic.hpp"
//#include "expression_visitor.hpp"
//#include "expression_vector_visitor.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"

//#include "../entities/type_entity.hpp"

#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

struct expression_cast_to_enum_visitor : static_visitor<error_storage>
{
    fn_compiler_context& ctx;
    case_expression const& ce;
    context_locator_t cl_;

    expression_cast_to_enum_visitor(fn_compiler_context& c, case_expression const& e, context_locator_t const& cl)
        : ctx{ c }
        , ce { e }
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

    //inline result_type operator()(bang_particular_bool_t const&) const
    //{
    //    return std::unexpected(cast_error{ loc, e, bang_particular_bool_t{}, ce });
    //}

    inline result_type operator()(bang_object_t const& obj) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("expression_cast_to_enum_visitor bang_object_t");
#if 0
        shared_ptr<entity> e = ctx.resolve_entity(obj.name());
        if (!e) [[unlikely]] {
            return make_error<basic_general_error>(ce.name.location, "unresolved context object"sv, obj.name());
        }
        shared_ptr<enum_entity> enum_ent = dynamic_pointer_cast<enum_entity>(e);
        if (!enum_ent) [[unlikely]] {
            return make_error<basic_general_error>(ce.name.location, "is not a enumeration"sv, obj.name());
        }
        auto const* enumcase = enum_ent->find(ce.name.value);
        if (!enumcase) [[unlikely]] {
            return make_error<unknown_case_error>(ce, obj.name());
        }
        ctx.append_expression(semantic::push_value{ enumcase->value });
        ctx.context_type = obj;
        return {};
#endif
    }

    //inline result_type operator()(bang_vector_t const& v) const
    //{
    //    THROW_NOT_IMPLEMENTED_ERROR();
    //    //if (target.type == v.type) return target;
    //    //return {};
    //}

    inline result_type operator()(bang_union_t const& v) const
    {
        alt_error err;
        for (bang_type const& t : v) {
            if (auto opterr = apply_visitor(*this, t); opterr) {
                err.alternatives.emplace_back(std::move(opterr));
            } else {
                return {};
            }
        }
        return make_error<alt_error>(std::move(err));
    }

    template <typename T>
    inline result_type operator()(T const&) const
    {
        // THROW_NOT_IMPLEMENTED_ERROR();
        return make_error<basic_general_error>(cl_(), "is not a enumeration"sv);
    }
};

}
