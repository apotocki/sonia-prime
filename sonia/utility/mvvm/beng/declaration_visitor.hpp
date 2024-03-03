//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "beng.hpp"

namespace sonia::lang::beng {

struct declaration_visitor : static_visitor<void>
{
    compiler_context& ctx;

    explicit declaration_visitor(compiler_context& c) : ctx{ c } {}

    void operator()(empty_t const&) const {}

    void operator()(exten_var const& td) const;

    void operator()(type_decl const& td) const;
    
    void operator()(enum_decl const& ed) const;
    
    void operator()(let_statement_decl const& ld) const;
    
    void operator()(expression_decl const& ed) const;

    void check_name(qname const& name) const;
};

}

#include "compiler.hpp"
#include "expression_visitor.hpp"
#include "ast.hpp"
#include "enum_entity.hpp"
#include "type_entity.hpp"

namespace sonia::lang::beng {

void declaration_visitor::operator()(exten_var const& td) const
{
    auto ve = sonia::make_shared<variable_entity>(qname{td.name}, std::move(td.type));
    ctx.u().eregistry().insert(ve);
}

void declaration_visitor::operator()(expression_decl const& ed) const
{
    expression_visitor evis{ ctx, ctx.expressions, nullptr };
    apply_visitor(evis, ed.expression);
}

void declaration_visitor::operator()(type_decl const& td) const
{
    check_name(td.name);
    auto e = make_shared<type_entity>(td.name);
    e->signatures.emplace_back();
    auto& sig = e->signatures.back();
    sig.result_type = beng_object_t{ td.name };

    for (auto const& parampair : td.parameters) {
        if (parampair.name.empty()) {
            sig.position_arguments.emplace_back(parampair.type);
        } else {
            sig.named_arguments.emplace_back(parampair.name, parampair.type);
        }
    }

    // to do: merge types for the same named arguments
    for (qname const& base : td.bases) {
        shared_ptr<entity> e = ctx.resolve_entity(base);
        if (!e) [[unlikely]] {
            throw exception("unresolved base type '%1%' for the type '%2%'"_fmt % ctx.u().print(base) % ctx.u().print(td.name));
        }
        shared_ptr<procedure_entity> type_ent = dynamic_pointer_cast<type_entity>(e);
        if (!type_ent) [[unlikely]] {
            throw exception("the base '%1%' of a type '%2%' is not a type"_fmt % ctx.u().print(base) % ctx.u().print(td.name));
        }

        BOOST_ASSERT(1 == type_ent->signatures.size());
        auto const& basesig = type_ent->signatures.back();
        sig.position_arguments.insert(sig.position_arguments.end(), basesig.position_arguments.begin(), basesig.position_arguments.end());
        sig.named_arguments.insert(sig.named_arguments.end(), basesig.named_arguments.begin(), basesig.named_arguments.end());
    }

    std::stable_sort(sig.named_arguments.begin(), sig.named_arguments.end(), [](auto const& l, auto const& r) {
        return l.first < r.first;
    });
    auto it = std::unique(sig.named_arguments.begin(), sig.named_arguments.end(), [](auto const& l, auto const& r) {
        return l.first == r.first;
    });
    ctx.u().eregistry().insert(std::move(e));
}

void declaration_visitor::operator()(enum_decl const& ed) const
{
    check_name(ed.name);
    auto e = make_shared<enum_entity>(ed.name);
    for (auto const& c : ed.cases) {
        e->cases.emplace_back(c, ctx.u().as_u32string(c));
    }
    std::ranges::sort(e->cases);
    ctx.u().eregistry().insert(std::move(e));
}

void declaration_visitor::operator()(let_statement_decl const& ld) const
{
    expression_visitor evis{ ctx, ctx.expressions, nullptr };
    auto etype = apply_visitor(evis, ld.expression);
    auto idx = ctx.new_variable(ld.name, etype, true);

    ctx.expressions.emplace_back(semantic::push_variable{ &idx });
}

void declaration_visitor::check_name(qname const& name) const
{
    if (auto pe = ctx.u().eregistry().find(name)) [[unlikely]] {
        throw exception("an entitity with the same name '%1%' is already defined"_fmt % ctx.u().print(name));
    }
}

}
