//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/bang/unit.hpp"
#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

struct expected_result_t
{
    entity_identifier type;
    lex::resource_location const& location;
};

// returns true if implicit cast was applied
struct expression_visitor : static_visitor<std::expected<bool, error_storage>>
{
    fn_compiler_context& ctx;
    optional<expected_result_t> expected_result;

    // to do: set a flag to notice unneeded result
    explicit expression_visitor(fn_compiler_context& c)
        : ctx{ c }
    {}

    expression_visitor(fn_compiler_context& c, expected_result_t && er)
        : ctx{ c }
        , expected_result{ std::move(er) }
    {}

    expression_visitor(fn_compiler_context& c, optional<expected_result_t> opter)
        : ctx{ c }
        , expected_result{ std::move(opter) }
    {}

    expression_visitor(fn_compiler_context& c, nullptr_t)
        : ctx{ c }
    {}

    result_type operator()(context_value const&) const;
    result_type operator()(variable_identifier const&) const;

    result_type operator()(case_expression const&) const;
    result_type operator()(not_empty_expression_t&) const;
    result_type operator()(member_expression_t&) const;
    result_type operator()(property_expression&) const;

    result_type operator()(annotated_bool const&) const;
    result_type operator()(annotated_integer const&) const;
    result_type operator()(annotated_decimal const&) const;
    result_type operator()(annotated_string const&) const;
    
    result_type operator()(expression_vector_t &) const;

    result_type operator()(chained_expression_t&) const;

    result_type operator()(lambda_t&) const;

    result_type operator()(entity_expression const& ee) const;

    result_type operator()(function_call_t const&) const;
    result_type operator()(unary_expression_t const& be) const;
    result_type operator()(binary_expression_t const& be) const;
        

    
    result_type operator()(binary_operator_t<binary_operator_type::LOGIC_AND>, binary_expression_t &) const;
    result_type operator()(binary_operator_t<binary_operator_type::LOGIC_OR>, binary_expression_t &) const;

    template <binary_operator_type BOpV>
    result_type operator()(binary_operator_t<BOpV>, binary_expression_t &) const;
    //result_type operator()(binary_operator_t<binary_operator_type::PLUS>, binary_expression_t&) const;
    //result_type operator()(binary_operator_t<binary_operator_type::CONCAT>, binary_expression_t &) const;

    function_entity& handle_lambda(lambda_t&) const;
    std::expected<function_entity const*, error_storage> handle_property_get(annotated_identifier id) const;


    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("expression_visitor not implemented expression");
    }

private:
    unit& u() const noexcept;

    template <typename ExprT>
    result_type apply_cast(entity_identifier, ExprT const& e) const;

    template <std::derived_from<pure_call_t> CallExpressionT>
    result_type operator()(qname_identifier fnqn, CallExpressionT const& call) const;

    template <std::derived_from<pure_call_t> CallExpressionT>
    result_type operator()(functional const& fnl, CallExpressionT const& call) const;

    result_type do_assign(binary_expression_t const&) const;
};

}
