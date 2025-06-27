//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

// Handles implicit cast of compile-time literals (string, integer, decimal) to runtime values.
class const_literal_implicit_cast_pattern : public functional::pattern
{
public:
    const_literal_implicit_cast_pattern() = default;

    std::expected<functional_match_descriptor_ptr, error_storage>
    try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const override;

    std::expected<syntax_expression_result_t, error_storage>
    apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const override;

    std::ostream& print(unit const&, std::ostream& s) const override { return s << "implicy_cast(constexpr @literal)->@literal"sv; }
};

} // namespace sonia::lang::bang
