//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "beng.hpp"
#include "unit.hpp"
#include "ast.hpp"

#include "semantic.hpp"

namespace sonia::lang::beng {

struct expression_visitor : static_visitor<beng_generic_type>
{
    compiler_context& ctx;
    beng_generic_type const* expetced_result;
    std::vector<semantic_expression_type>& result;

    explicit expression_visitor(compiler_context& c, std::vector<semantic_expression_type>& r, beng_generic_type const* er)
        : ctx{ c }
        , result { r }
        , expetced_result{ er }
    {}

    result_type operator()(qname const& e) const;

    result_type operator()(case_expression const& ce) const;

    result_type operator()(decimal const& e) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }

    result_type operator()(small_u32string const& e) const;
    
    result_type operator()(procedure_t const& proc) const;

    result_type operator()(binary_expression_t<operator_type::ASSIGN> const& op) const;
};

}

#include "compiler.hpp"
#include "enum_entity.hpp"
#include "procedure_entity.hpp"

namespace sonia::lang::beng {

expression_visitor::result_type expression_visitor::operator()(small_u32string const& e) const
{
    result.emplace_back(semantic::push_value{ e });
    return beng_string_t{};
}

expression_visitor::result_type expression_visitor::operator()(qname const& e) const
{
    // to do: look for variable first
    shared_ptr<entity> expr = ctx.resolve_entity(e);
    THROW_NOT_IMPLEMENTED_ERROR();
}

expression_visitor::result_type expression_visitor::operator()(binary_expression_t<operator_type::ASSIGN> const& op) const
{
    expression_visitor evis{ ctx, result, nullptr };
    auto rtype = apply_visitor(evis, op.right);

    if (qname const* varnm = get<qname>(&op.left); varnm) {
        auto optvar = ctx.resolve_variable(*varnm);
        if (!optvar) {
            throw exception("variable '%1%' is not defined"_fmt % ctx.u().print(*varnm));
        }
        result.emplace_back(semantic::pop_variable{ optvar });
        return std::move(rtype);
    }
    THROW_NOT_IMPLEMENTED_ERROR();
}

expression_visitor::result_type expression_visitor::operator()(case_expression const& ce) const
{
    if (!expetced_result) {
        throw exception("no context type to resolve the case expression %1%"_fmt % ctx.u().print(ce.name));
    }
    beng_object_t const* pobj = get<beng_object_t>(expetced_result);
    if (!pobj) {
        throw exception("the context type isn't an object to resolve the case expression %1%"_fmt % ctx.u().print(ce.name));
    }
    shared_ptr<entity> e = ctx.resolve_entity(pobj->name());
    if (!e) [[unlikely]] {
        throw exception("unresolved context object '%1%'"_fmt % ctx.u().print(pobj->name()));
    }
    shared_ptr<enum_entity> enum_ent = dynamic_pointer_cast<enum_entity>(e);
    if (!enum_ent) [[unlikely]] {
        throw exception("name '%1%' is not a enumerration"_fmt % ctx.u().print(pobj->name()));
    }
    auto const* enumcase = enum_ent->find(ce.name);
    if (!enumcase) [[unlikely]] {
        throw exception("name '%1%' is not a case of the enumerration %2%"_fmt % ctx.u().print(ce.name) % ctx.u().print(pobj->name()));
    }
    result.emplace_back(semantic::push_value{ enumcase->value });
    return *expetced_result;
}

expression_visitor::result_type expression_visitor::operator()(procedure_t const& proc) const
{
    //expression_visitor evis{ ctx, nullptr };
    //semantic_expression_pair epair = apply_visitor(evis, proc.subject);
    
    // check uniqueness
    if (auto it = std::ranges::adjacent_find(proc.named_args, {}, [](auto const& pair) { return std::get<0>(pair).id; }); it != proc.named_args.end()) {
        ++it; // get second
        auto const& aid = std::get<0>(*it);
        throw exception("%1%(%2%,%3%): duplicate argument `%4%`"_fmt % aid.location.resource % aid.location.line % aid.location.column % ctx.u().print(aid.id));
    }

    shared_ptr<entity> e = ctx.resolve_entity(proc.name);
    if (!e) [[unlikely]] {
        throw exception("unresolved procedure name '%1%'"_fmt % ctx.u().print(proc.name));
    }
    shared_ptr<procedure_entity> proc_ent = dynamic_pointer_cast<procedure_entity>(e);
    if (!proc_ent) [[unlikely]] {
        throw exception("name '%1%' is not callable"_fmt % ctx.u().print(proc.name));
    }
    beng_generic_type rtype;
    if (!proc_ent->find(ctx, proc.positioned_args, proc.named_args, result, rtype)) {
        throw exception("can't match a procedure call '%1%'"_fmt % ctx.u().print(proc.name));
    }

    return rtype;
}

}
