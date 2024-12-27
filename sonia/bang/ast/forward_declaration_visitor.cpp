//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "forward_declaration_visitor.hpp"

#include "fn_compiler_context.hpp"

#include "../entities/enum_entity.hpp"
#include "../entities/type_entity.hpp"

#include "sonia/bang/errors.hpp"
#include "sonia/bang/parser.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia::lang::bang {

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

void forward_declaration_visitor::operator()(fn_pure_t const&) const
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

void forward_declaration_visitor::operator()(include_decl& d)
{
    fs::path fpath{ u8string_view{reinterpret_cast<char8_t const*>(d.path.value.data()), d.path.value.size() } };

    auto exp_decls = pctx.parse(fpath);
    if (!exp_decls.has_value()) {
        throw exception{ ctx.u().print(basic_general_error{ d.path.location, exp_decls.error() }) };
    }

    SCOPE_EXIT([this] { pctx.pop_resource(); });

    for (auto& d : *exp_decls) {
        apply_visitor(*this, d);
    }
}

void forward_declaration_visitor::operator()(enum_decl & ed)
{
    THROW_NOT_IMPLEMENTED_ERROR("forward_declaration_visitor enum_decl");
#if 0
    if (auto pe = ctx.u().eregistry().find(ed.name()); pe) [[unlikely]] {
        throw exception(ctx.u().print(identifier_redefinition_error{ ed.aname, pe->location() }));
    }
    auto e = make_shared<enum_entity>(ed.name());
    e->set_location(ed.location());
    for (auto const& c : ed.cases) {
        e->cases.emplace_back(c, ctx.u().as_string(c));
    }
    std::ranges::sort(e->cases);
    ctx.u().eregistry().insert(std::move(e));
#endif
}

void forward_declaration_visitor::operator()(type_decl & td)
{
    THROW_NOT_IMPLEMENTED_ERROR("forward_declaration_visitor type_decl");
#if 0
    if (auto pe = ctx.u().eregistry().find(td.name()); pe) [[unlikely]] {
        throw exception(ctx.u().print(identifier_redefinition_error{ td.aname, pe->location() }));
    }
    auto e = make_shared<type_entity>(td.name());
    e->set_location(td.location());
    e->direct_bases = std::move(td.bases);
    e->direct_parameters = std::move(td.parameters);

    types.emplace_back(e.get());
    ctx.u().eregistry().insert(std::move(e));
#endif
}

}
