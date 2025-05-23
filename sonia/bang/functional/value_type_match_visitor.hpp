//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class fn_compiler_context;

// if result entity_identifier is empty -> ignore  (void argument case)
struct value_type_match_visitor : static_visitor<std::expected<syntax_expression_result_t, error_storage>>
{
    fn_compiler_context& caller_ctx;
    fn_compiler_context& callee_ctx;
    semantic::expression_list_t& expressions;
    syntax_expression_t const& expr;
    functional_binding& binding; // to forward it to signature_matcher_visitor
    mutable int weight = 0;

    value_type_match_visitor(fn_compiler_context& caller, fn_compiler_context& callee, semantic::expression_list_t&, syntax_expression_t const&, functional_binding&) noexcept;
        
    result_type operator()(annotated_qname_identifier const& aqi) const;
    
    result_type operator()(variable_reference const& var) const;

    result_type operator()(function_call_t const& fc) const;

    result_type operator()(placeholder const&) const;

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("value_type_match_visitor not implemented expression");
    }

private:
    result_type match_type(entity_identifier const&, lex::resource_location = {}) const;

};

}
