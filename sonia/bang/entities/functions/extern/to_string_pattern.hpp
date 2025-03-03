//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functions/external_fn_pattern.hpp"

namespace sonia::lang::bang {

class to_string_pattern : public basic_fn_pattern
{
public:
    using basic_fn_pattern::basic_fn_pattern;

    error_storage apply(fn_compiler_context&, functional_match_descriptor&) const override;
    std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context&, functional_match_descriptor&) const override;
};

}
