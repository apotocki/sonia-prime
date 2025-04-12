//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/generic_pattern_base.hpp"

namespace sonia::lang::bang {

class tuple_head_pattern : public generic_pattern_base<tuple_head_pattern>
{
    friend class generic_pattern_base<tuple_head_pattern>;

public:
    tuple_head_pattern() = default;

    //std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, pure_call_t const&, annotated_entity_identifier const&) const;

    std::expected<syntax_expression_result_t, error_storage> apply(fn_compiler_context&, functional_match_descriptor&) const override;

    //std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context& ctx, functional_match_descriptor& md) const override;

    std::ostream& print(unit const&, std::ostream& s) const override { return s << "head(tuple)"sv; }

protected:
    error_storage accept_argument(std::nullptr_t, functional_match_descriptor_ptr&, arg_context_type&, syntax_expression_result_t&) const;
};

}
