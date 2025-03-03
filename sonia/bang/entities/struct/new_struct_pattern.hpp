//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

//#include "sonia/shared_ptr.hpp"
#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class new_struct_pattern : public functional::pattern
{
public:
    new_struct_pattern() = default;
    std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context&, functional_match_descriptor&) const override;
    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, pure_call_t const&, annotated_entity_identifier const&) const override;
    error_storage apply(fn_compiler_context&, functional_match_descriptor&) const override;

    std::ostream& print(unit const&, std::ostream& s) const override;
};

}
