//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functions/basic_fn_pattern.hpp"

namespace sonia::lang::bang {

class struct_fn_pattern : public basic_fn_pattern
{
    variant<field_list_t, statement_span> body_;

public:
    struct_fn_pattern(functional const&, variant<field_list_t, statement_span> const&);

    std::expected<application_result_t, error_storage> apply(fn_compiler_context&, functional_match_descriptor&) const override;
};

}
