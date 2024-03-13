//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "../unit.hpp"

#include "sonia/beng/ast.hpp"
#include "sonia/beng/semantic.hpp"
#include "sonia/optional.hpp"

namespace sonia::lang::beng {

struct expression_visitor : static_visitor<optional<beng_type>>
{
    compiler_context& ctx;
    beng_type const* expected_result;
    std::vector<semantic_expression_type>& result;

    explicit expression_visitor(compiler_context& c, std::vector<semantic_expression_type>& r, beng_type const* er)
        : ctx{ c }
        , result { r }
        , expected_result{ er }
    {}

    result_type operator()(annotated_qname const&) const;

    result_type operator()(case_expression const&) const;

    result_type operator()(decimal const&) const;
    
    result_type operator()(small_u32string const&) const;
    
    result_type operator()(expression_vector_t const&) const;

    result_type operator()(function_call_t const&) const;

    result_type operator()(binary_expression_t<operator_type::ASSIGN> const&) const;
};

}
