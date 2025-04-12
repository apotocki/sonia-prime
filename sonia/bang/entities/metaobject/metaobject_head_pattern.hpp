//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "metaobject_argument_pattern.hpp"

namespace sonia::lang::bang {

class metaobject_head_pattern : public metaobject_argument_pattern
{
public:
    metaobject_head_pattern() = default;

    std::expected<syntax_expression_result_t, error_storage> apply(fn_compiler_context&, functional_match_descriptor&) const override;

    std::ostream& print(unit const&, std::ostream& s) const override { return s << "head(metaobject)"sv; }
};

}
