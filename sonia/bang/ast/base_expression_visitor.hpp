
//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <expected>

#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"

#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

class base_expression_visitor : public static_visitor<std::expected<std::pair<syntax_expression_result_t, bool>, error_storage>>
{
    friend struct array_expression_processor;

protected:
    fn_compiler_context& ctx;
    annotated_entity_identifier expected_result;
    bool is_type_expected = true; // type or entity

public:
    explicit base_expression_visitor(fn_compiler_context& c) noexcept;
    base_expression_visitor(fn_compiler_context& c, annotated_entity_identifier er) noexcept;

    result_type operator()(indirect_value const&) const;

    //result_type operator()(context_identifier const&) const;
    result_type operator()(annotated_bool const&) const;
    result_type operator()(annotated_integer const&) const;
    result_type operator()(annotated_decimal const&) const;
    result_type operator()(annotated_string const&) const;
    result_type operator()(annotated_identifier const&) const;
    result_type operator()(annotated_entity_identifier const&) const;
    result_type operator()(annotated_qname const&) const;

    result_type operator()(variable_identifier const&) const;

    result_type operator()(array_expression_t const&) const;

    result_type operator()(bang_vector_t const&) const;

    result_type operator()(member_expression_t const&) const;

    result_type operator()(unary_expression_t const& be) const;

    result_type operator()(function_call_t const&) const;

    result_type operator()(new_expression_t const&) const;
    result_type operator()(binary_expression_t const&) const;

    result_type operator()(index_expression_t const&) const;

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("base_expression_visitor not implemented expression");
    }

protected:
    unit& u() const noexcept;

    template <typename FnIdT, std::derived_from<pure_call_t> ExprT>
    result_type operator()(FnIdT &&, ExprT const&) const;

    result_type do_assign(binary_expression_t const&) const;

    result_type apply_cast(entity const&, syntax_expression_t const&) const;
    result_type apply_cast(syntax_expression_result_t, syntax_expression_t const&) const;

    template <typename ExprT>
    result_type apply_cast(entity_identifier eid, ExprT const& e) const;
    
    template <typename ExprT>
    inline result_type apply_cast(entity const& ent, ExprT const& e) const
    {
        return apply_cast(ent, syntax_expression_t{ e });
    }

    template <typename ExprT>
    result_type apply_cast(syntax_expression_result_t er, ExprT const& e) const
    {
        return apply_cast(std::move(er), syntax_expression_t{ e });
    }

    template <typename ExprT>
    inline result_type apply_cast(std::expected<syntax_expression_result_t, error_storage>, ExprT const&) const;

    result_type do_result(entity_identifier, bool casted) const noexcept;
};

}
