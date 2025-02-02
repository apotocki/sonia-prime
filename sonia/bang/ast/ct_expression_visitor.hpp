//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <expected>

#include "sonia/bang/unit.hpp"
#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

struct ct_expression_visitor : static_visitor<std::expected<entity_identifier, error_storage>>
{
    fn_compiler_context& ctx;
    annotated_entity_identifier expected_result;

    explicit ct_expression_visitor(fn_compiler_context& c)
        : ctx{ c }
    {}

    ct_expression_visitor(fn_compiler_context& c, annotated_entity_identifier&& er)
        : ctx{ c }
        , expected_result{ std::move(er) }
    {}

    result_type operator()(annotated_bool const&) const;
    result_type operator()(annotated_integer const&) const;
    result_type operator()(annotated_decimal const&) const;
    result_type operator()(annotated_string const&) const;
    result_type operator()(annotated_qname const&) const;
    result_type operator()(annotated_identifier const&) const;
    result_type operator()(annotated_entity_identifier const&) const;
    result_type operator()(function_call_t const&) const;

    result_type operator()(variable_identifier const vi) const;
    
    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("ct_expression_visitor not implemented expression");
    }

private:
    unit& u() const noexcept;

    template <typename ExprT>
    result_type apply_cast(entity_identifier, ExprT const& e) const;

    template <std::derived_from<pure_call_t> CallExpressionT>
    result_type operator()(builtin_qnid, CallExpressionT const&) const;
};

}
