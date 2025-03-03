//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <expected>

#include "sonia/bang/unit.hpp"

#include "base_expression_visitor.hpp"

namespace sonia::lang::bang {

struct ct_expression_visitor 
    : base_expression_visitor
    , static_visitor<std::expected<entity_identifier, error_storage>>
{
    using result_type = std::expected<entity_identifier, error_storage>;

    inline explicit ct_expression_visitor(fn_compiler_context& c) noexcept
        : base_expression_visitor{ c }
    {}

    ct_expression_visitor(fn_compiler_context& c, annotated_entity_identifier&& er, bool is_type_expected_value = true)
        : base_expression_visitor{ c, std::move(er) }
        
    {
        is_type_expected = is_type_expected_value;
    }

    result_type operator()(annotated_bool const&) const;
    result_type operator()(annotated_integer const&) const;
    result_type operator()(annotated_decimal const&) const;
    result_type operator()(annotated_string const&) const;
    result_type operator()(annotated_qname const&) const;
    result_type operator()(annotated_identifier const&) const;
    result_type operator()(annotated_entity_identifier const&) const;
    result_type operator()(function_call_t const&) const;
    result_type operator()(member_expression_t const&) const;

    result_type operator()(variable_identifier const&) const;
    result_type operator()(lambda_t const&) const;

    result_type operator()(opt_named_syntax_expression_list_t const&) const;

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("ct_expression_visitor not implemented expression");
    }

private:
    template <typename ExprT>
    result_type apply_cast(entity_identifier, ExprT const& e) const;

    template <std::derived_from<pure_call_t> CallExpressionT>
    result_type operator()(builtin_qnid, CallExpressionT const&) const;
};

}
