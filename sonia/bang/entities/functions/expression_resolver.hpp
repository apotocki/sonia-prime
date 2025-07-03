//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class expression_resolver : public functional::entity_resolver
{
    lex::resource_location location_;
    syntax_expression_t expression_;

public:
    expression_resolver(lex::resource_location, syntax_expression_t const& expr);

    std::expected<entity_identifier, error_storage> const_resolve(fn_compiler_context&) const override;
    lex::resource_location const& location() const override;
};

}
