//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <expected>
#include "../unit.hpp"

#include "sonia/beng/semantic.hpp"
#include "sonia/optional.hpp"
#include "sonia/beng/errors.hpp"

namespace sonia::lang::beng {

struct expected_result_t
{
    beng_type const& type;
    lex::resource_location const& location;
};

struct expression_visitor : static_visitor<std::expected<beng_type, error_storage>>
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

    expression_visitor(fn_compiler_context& c, nullptr_t)
        : ctx{ c }
    {}

    result_type operator()(variable_identifier const&) const;

    result_type operator()(case_expression const&) const;
    result_type operator()(member_expression_t&) const;

    result_type operator()(annotated_bool const&) const;
    result_type operator()(annotated_decimal const&) const;
    result_type operator()(annotated_u32string const&) const;
    
    result_type operator()(expression_vector_t &) const;

    result_type operator()(chained_expression_t&) const;

    result_type operator()(lambda_t&) const;

    result_type operator()(function_call_t&) const;

    result_type operator()(negate_expression_t&) const;
    result_type operator()(binary_expression_t<binary_operator_type::ASSIGN> &) const;
    result_type operator()(binary_expression_t<binary_operator_type::LOGIC_AND> &) const;
    result_type operator()(binary_expression_t<binary_operator_type::LOGIC_OR> &) const;
    result_type operator()(binary_expression_t<binary_operator_type::CONCAT>&) const;

    function_entity& handle_lambda(lambda_t&) const;

private:
    template <typename ExprT>
    result_type apply_cast(beng_type const& t, ExprT const& e) const;
};

}
