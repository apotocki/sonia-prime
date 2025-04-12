//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functions/basic_fn_pattern.hpp"

namespace sonia::lang::bang {

// const array to vector
// fn implicit_cast(integer) -> float16 => implementation defined
// fn implicit_cast(integer) -> float => implementation defined
// fn implicit_cast(integer) -> double => implementation defined
// fn implicit_cast(integer) -> decimal => implementation defined

class numeric_implicit_cast_pattern : public functional::pattern
{
public:
    numeric_implicit_cast_pattern() = default;

    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, prepared_call const&, annotated_entity_identifier const&) const override;
    std::expected<syntax_expression_result_t, error_storage> apply(fn_compiler_context&, functional_match_descriptor&) const override;

    std::ostream& print(unit const&, std::ostream& s) const override { return s << "implicy_cast(const array())"sv; }
};

}
