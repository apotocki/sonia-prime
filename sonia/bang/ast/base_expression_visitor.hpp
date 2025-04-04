
//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <expected>

#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"

#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

class base_expression_visitor : public static_visitor<std::expected<std::pair<functional::pattern::application_result_t, bool>, error_storage>>
{
protected:
    fn_compiler_context& ctx;
    annotated_entity_identifier expected_result;
    bool is_type_expected = true; // type or entity

public:
    explicit base_expression_visitor(fn_compiler_context& c) noexcept;
    base_expression_visitor(fn_compiler_context& c, annotated_entity_identifier er) noexcept;

    result_type operator()(context_value const&) const;

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

    result_type operator()(opt_named_syntax_expression_list_t const& nel) const;

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

    template <std::derived_from<pure_call_t> CallExpressionT>
    result_type operator()(builtin_qnid, CallExpressionT const&) const;

    result_type do_assign(binary_expression_t const&) const;

    template <typename ExprT>
    result_type apply_cast(entity_identifier, ExprT const& e) const;

    template <typename ExprT>
    result_type apply_cast(entity const&, ExprT const& e) const;

    template <typename ExprT>
    result_type apply_cast(semantic::managed_expression_list, ExprT const&) const;

    template <typename ExprT>
    result_type apply_cast(std::expected<functional::pattern::application_result_t, error_storage>, ExprT const&) const;
};

}
