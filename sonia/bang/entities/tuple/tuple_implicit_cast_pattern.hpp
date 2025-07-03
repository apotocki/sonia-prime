//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

// Pattern for implicit casting between tuples with matching field names and count.
class tuple_implicit_cast_pattern : public functional::pattern
{
public:
    tuple_implicit_cast_pattern() = default;

    // Attempts to match a call for tuple implicit cast.
    std::expected<functional_match_descriptor_ptr, error_storage>
    try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const override;

    // Applies the cast, producing the result tuple if all fields can be cast.
    std::expected<syntax_expression_result_t, error_storage>
    apply(fn_compiler_context&, semantic::expression_list_t&, functional_match_descriptor&) const override;

    std::ostream& print(unit const&, std::ostream& s) const override { return s << "implicit_cast(@tuple)->@tuple"sv; }
};

} // namespace sonia::lang::bang
