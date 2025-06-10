//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class tuple_project_get_pattern : public functional::pattern
{
public:
    tuple_project_get_pattern() = default;

    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, prepared_call const&, expected_result_t const&) const override;
    
    std::expected<syntax_expression_result_t, error_storage> apply(fn_compiler_context&, semantic::expression_list_t&, functional_match_descriptor&) const override;

    std::ostream& print(unit const&, std::ostream& s) const override { return s << "get(self: @tuple_project, property: integer)"sv; }

protected:
    class tuple_project_get_match_descriptor : public functional_match_descriptor
    {
    public:
        inline tuple_project_get_match_descriptor(
            identifier name,
            entity const& orig_tpl, entity_signature const& orig_sig,
            lex::resource_location loc) noexcept
            : functional_match_descriptor{ std::move(loc) }
            , project_name{ name }
            , orig_tuple_entity{ orig_tpl }
            , orig_tuple_sig{ orig_sig }
        {}

        identifier project_name;
        entity const& orig_tuple_entity;
        entity_signature const& orig_tuple_sig;
    };
};

}