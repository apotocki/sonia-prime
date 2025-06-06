//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "metaobject_argument_pattern.hpp"

namespace sonia::lang::bang {

class metaobject_empty_pattern : public metaobject_argument_pattern
{
public:
    metaobject_empty_pattern() = default;

#ifndef NDEBUG
    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const override
    {
        return metaobject_argument_pattern::try_match(ctx, call, exp);
    }

#endif
    std::expected<syntax_expression_result_t, error_storage> apply(fn_compiler_context&, semantic::expression_list_t&, functional_match_descriptor&) const override;

    std::ostream& print(unit const&, std::ostream& s) const override { return s << "empty(metaobject)"sv; }
};

}
