//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "forward_declaration_visitor.hpp"

#include "compiler_context.hpp"

#include "../entities/enum_entity.hpp"
#include "../entities/type_entity.hpp"

namespace sonia::lang::beng {

void forward_declaration_visitor::operator()(exten_var const&) const
{
    // skip
}

void forward_declaration_visitor::operator()(expression_decl const&) const
{
    // skip
}

void forward_declaration_visitor::operator()(return_decl const&) const
{
    // skip
}

void forward_declaration_visitor::operator()(fn_pure_decl const&) const
{
    // skip
}

void forward_declaration_visitor::operator()(fn_decl_t const& fnd) const
{
    // to do: in function type declarations?
    // skip
}

void forward_declaration_visitor::operator()(let_statement_decl const& ld) const
{
    // skip
}

void forward_declaration_visitor::operator()(enum_decl const& ed) const
{
    if (auto pe = ctx.u().eregistry().find(ed.name()); pe) [[unlikely]] {
        ctx.throw_identifier_redefinition(*pe, ed.name(), ed.location());
    }
    auto e = make_shared<enum_entity>(ed.name());
    e->set_location(ed.location());
    ctx.u().eregistry().insert(std::move(e));
}

void forward_declaration_visitor::operator()(type_decl const& td) const
{
    if (auto pe = ctx.u().eregistry().find(td.name()); pe) [[unlikely]] {
        ctx.throw_identifier_redefinition(*pe, td.name(), td.location());
    }
    auto e = make_shared<type_entity>(td.name());
    e->set_location(td.location());
    ctx.u().eregistry().insert(std::move(e));
}

}
