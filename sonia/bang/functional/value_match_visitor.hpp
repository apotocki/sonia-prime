//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class fn_compiler_context;

// match constexpr expressions
struct value_match_visitor : static_visitor<std::expected<entity_identifier, error_storage>>
{
    fn_compiler_context& caller_ctx;
    fn_compiler_context& callee_ctx;
    semantic::expression_list_t& expressions;
    syntax_expression_t const& cexpr;
    functional_binding& binding; // to forward it to signature_matcher_visitor
    bool matching_type;

    value_match_visitor(fn_compiler_context& caller, fn_compiler_context& callee, semantic::expression_list_t&, syntax_expression_t const& ce, functional_binding& b, bool is_type_matching = false) noexcept;

    result_type operator()(annotated_bool const&) const;

    result_type operator()(annotated_qname_identifier const&) const;
    
    result_type operator()(annotated_entity_identifier const&) const;

    result_type operator()(function_call_t const&) const;
    
    result_type operator()(variable_reference const&) const;
    
    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("type_constraint_match_visitor not implemented expression");
    }
};

}
