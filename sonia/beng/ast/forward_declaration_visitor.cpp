//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "forward_declaration_visitor.hpp"

#include "fn_compiler_context.hpp"

#include "../entities/enum_entity.hpp"
#include "../entities/type_entity.hpp"

#include "sonia/beng/errors.hpp"

namespace sonia::lang::beng {

    /*
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
*/

void forward_declaration_visitor::operator()(enum_decl const& ed) const
{
    if (auto pe = ctx.u().eregistry().find(ed.name()); pe) [[unlikely]] {
        throw exception(ctx.u().print(identifier_redefinition_error{ ed.location(), pe->location(), ed.name() }));
    }
    auto e = make_shared<enum_entity>(qname{ed.name(), true});
    e->set_location(ed.location());
    for (auto const& c : ed.cases) {
        e->cases.emplace_back(c, ctx.u().as_u32string(c));
    }
    std::ranges::sort(e->cases);
    ctx.u().eregistry().insert(std::move(e));
}

void forward_declaration_visitor::operator()(type_decl & td)
{
    if (auto pe = ctx.u().eregistry().find(td.name()); pe) [[unlikely]] {
        throw exception(ctx.u().print(identifier_redefinition_error{ td.location(), pe->location(), td.name() }));
    }
    auto e = make_shared<type_entity>(qname{td.name(), true});
    e->set_location(td.location());
    e->direct_bases = std::move(td.bases);
    e->direct_parameters = std::move(td.parameters);

    types.emplace_back(e.get());
    ctx.u().eregistry().insert(std::move(e));
}

}
