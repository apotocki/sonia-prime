//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <expected>

#include "sonia/optional.hpp"

#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"
#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

class variable_entity;

struct lvalue_expression_visitor : static_visitor<std::expected<entity const*, error_storage>>
{
    fn_compiler_context& ctx;

    explicit lvalue_expression_visitor(fn_compiler_context& c)
        : ctx{ c }
    {}

    //result_type operator()(annotated_qname const&) const;

    result_type operator()(variable_reference const&) const;

    result_type operator()(context_value const&) const;
    result_type operator()(context_identifier const&) const;
    result_type operator()(not_empty_expression_t const&) const;
    result_type operator()(member_expression_t const &) const;

    result_type operator()(annotated_bool const&) const;
    result_type operator()(annotated_integer const&) const;
    result_type operator()(annotated_decimal const&) const;
    result_type operator()(annotated_string const&) const;
    
    result_type operator()(expression_vector_t const&) const;
    result_type operator()(chained_expression_t&) const;

    result_type operator()(lambda_t const&) const;
    result_type operator()(function_call_t const&) const;
    result_type operator()(annotated_entity_identifier const&) const;

    result_type operator()(unary_expression_t const&) const;
    result_type operator()(binary_expression_t const&) const;

    result_type operator()(binary_operator_t<binary_operator_type::ASSIGN>, binary_expression_t const&) const;
    result_type operator()(binary_operator_t<binary_operator_type::PLUS>, binary_expression_t const&) const;
    result_type operator()(binary_operator_t<binary_operator_type::LOGIC_AND>, binary_expression_t const&) const;
    result_type operator()(binary_operator_t<binary_operator_type::LOGIC_OR>, binary_expression_t const&) const;
    result_type operator()(binary_operator_t<binary_operator_type::CONCAT>, binary_expression_t const&) const;
   
    result_type operator()(binary_operator_t<binary_operator_type::EQ>, binary_expression_t const&) const;
    result_type operator()(binary_operator_t<binary_operator_type::NE>, binary_expression_t const&) const;

    result_type operator()(opt_named_syntax_expression_list_t const&) const;

    std::expected<entity const*, error_storage> handle_property_set(annotated_identifier id) const;
};

}
