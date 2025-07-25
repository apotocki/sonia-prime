//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class tuple_get_pattern : public functional::pattern
{
public:
    tuple_get_pattern() = default;

    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, prepared_call const&, expected_result_t const&) const override;
    
    std::expected<syntax_expression_result_t, error_storage> apply(fn_compiler_context&, semantic::expression_list_t&, functional_match_descriptor&) const override;

    std::ostream& print(unit const&, std::ostream& s) const override { return s << "get(self: tuple(...), property: integer|__identifier)~>auto"sv; }

protected:
    class tuple_get_match_descriptor : public functional_match_descriptor
    {
    public:
        inline tuple_get_match_descriptor(prepared_call const& call, entity const& ent, entity_signature const& sig) noexcept
            : functional_match_descriptor{ call }
            , tpl_entity{ ent }
            , arg_sig{ sig }
        {}

        entity const& tpl_entity;
        entity_signature const& arg_sig;
    };
};

}
