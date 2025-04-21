//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "expression_resolver.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

namespace sonia::lang::bang {

expression_resolver::expression_resolver(lex::resource_location loc, syntax_expression_t const& expr)
    : location_{ std::move(loc) }
    , expression_{ expr }
{}

std::expected<entity_identifier, error_storage> expression_resolver::const_resolve(fn_compiler_context& ctx) const
{
    ct_expression_visitor evis{ ctx };
    auto res = apply_visitor(evis, expression_);
    if (!res) return std::unexpected(res.error());
    if (res->expressions) THROW_NOT_IMPLEMENTED_ERROR("expression_resolver::const_resolve");
    return res->value;
    //THROW_NOT_IMPLEMENTED_ERROR("expression_resolver::resolve");
}

lex::resource_location const& expression_resolver::location() const
{
    return location_;
}

}
