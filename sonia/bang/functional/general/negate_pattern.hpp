//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

// Pattern for negating a boolean value
class negate_pattern : public functional::pattern
{
public:
    negate_pattern() = default;

    // Attempts to match a call with a single boolean argument
    std::expected<functional_match_descriptor_ptr, error_storage>
    try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const override;

    // Applies the negation operation to the boolean argument
    std::expected<syntax_expression_result_t, error_storage>
    apply(fn_compiler_context&, semantic::expression_list_t&, functional_match_descriptor&) const override;

    std::ostream& print(unit const&, std::ostream& s) const override { return s << "negate(boolean)->boolean"sv; }
};

} // namespace sonia::lang::bang