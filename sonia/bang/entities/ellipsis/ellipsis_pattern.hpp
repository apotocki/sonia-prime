//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class ellipsis_pattern : public functional::pattern
{
public:
    ellipsis_pattern() = default;

    std::expected<int, error_storage> is_matched(fn_compiler_context&, pure_call_t const&, functional::match_descriptor&) const override;
    std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context&, functional::match_descriptor&) const override;
};

}