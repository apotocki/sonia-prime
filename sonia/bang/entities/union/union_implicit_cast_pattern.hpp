//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

// const array to vector
// fn implicit_cast(_) -> union(...) => implementation defined
class union_implicit_cast_pattern : public functional::pattern
{
public:
    union_implicit_cast_pattern() = default;

    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, prepared_call const&, expected_result_t const&) const override;
    std::expected<syntax_expression_result_t, error_storage> apply(fn_compiler_context&, semantic::expression_list_t&, functional_match_descriptor&) const override;

    std::ostream& print(unit const&, std::ostream& s) const override { return s << "implicit_cast(_)->union"sv; }
};

}
