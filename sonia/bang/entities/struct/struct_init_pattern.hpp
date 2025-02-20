//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functions/basic_fn_pattern.hpp"

namespace sonia::lang::bang {

class struct_init_pattern : public basic_fn_pattern
{
    parameter_woa_list_t body_parameters_;
    //variant<field_list_t, statement_span> body_;

public:
    struct_init_pattern(functional const&, variant<field_list_t, statement_span> const&);
    error_storage init(fn_compiler_context& ctx, annotated_entity_identifier result); // result is a defined entity
    error_storage init(fn_compiler_context& ctx, annotated_qname fname, parameter_woa_list_t const& fparameters); // result is a functional name with parameters

    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, pure_call_t const&, annotated_entity_identifier const&) const override;

    error_storage apply(fn_compiler_context&, qname_identifier, functional_match_descriptor&) const override;
    std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context&, qname_identifier, functional_match_descriptor&) const override;
};

}
