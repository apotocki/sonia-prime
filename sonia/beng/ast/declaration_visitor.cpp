//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "declaration_visitor.hpp"

#include "compiler_context.hpp"
#include "expression_visitor.hpp"
#include "preliminary_type_visitor.hpp"

#include "../entities/enum_entity.hpp"
#include "../entities/type_entity.hpp"

namespace sonia::lang::beng {

void declaration_visitor::operator()(exten_var const& td) const
{
    preliminary_type_visitor tqvis{ ctx };
    beng_type vartype = apply_visitor(tqvis, td.type);
    auto ve = sonia::make_shared<variable_entity>(qname{td.name}, std::move(vartype));
    ctx.u().eregistry().insert(ve);
}

void declaration_visitor::operator()(expression_decl const& ed) const
{
    expression_visitor evis{ ctx, ctx.expressions, nullptr };
    apply_visitor(evis, ed.expression);
}

void declaration_visitor::operator()(type_decl const& td) const
{
    auto e = dynamic_pointer_cast<type_entity>(ctx.u().eregistry().find(td.name()));
    assert(e);

    e->signatures.emplace_back();
    auto& sig = e->signatures.back();

    preliminary_type_visitor tqvis{ ctx };
    sig.result_type = beng_object_t{ e.get() };

    for (auto const& parampair : td.parameters) {
        beng_type paramtype = apply_visitor(tqvis, parampair.type);
        if (!parampair.name) {
            sig.position_parameters.emplace_back(paramtype);
        } else {
            sig.named_parameters.emplace_back(parampair.name->id, paramtype);
        }
    }

    // to do: merge types for the same named arguments
    for (qname const& base : td.bases) {
        //tqvis
        shared_ptr<entity> e = ctx.resolve_entity(base);
        if (!e) [[unlikely]] {
            throw exception("unresolved base type '%1%' for the type '%2%'"_fmt % ctx.u().print(base) % ctx.u().print(td.name()));
        }
        shared_ptr<functional_entity> type_ent = dynamic_pointer_cast<type_entity>(e);
        if (!type_ent) [[unlikely]] {
            throw exception("the base '%1%' of a type '%2%' is not a type"_fmt % ctx.u().print(base) % ctx.u().print(td.name()));
        }

        BOOST_ASSERT(1 == type_ent->signatures.size());
        auto const& basesig = type_ent->signatures.back();
        sig.position_parameters.insert(sig.position_parameters.end(), basesig.position_parameters.begin(), basesig.position_parameters.end());
        sig.named_parameters.insert(sig.named_parameters.end(), basesig.named_parameters.begin(), basesig.named_parameters.end());
    }
    // stable to preserve duplicated parameters order if exist
    std::stable_sort(sig.named_parameters.begin(), sig.named_parameters.end(), [](auto const& l, auto const& r) {
        return std::get<0>(l).id < std::get<0>(r).id;
    });
    // report duplicates?
    auto it = std::unique(sig.named_parameters.begin(), sig.named_parameters.end(), [](auto const& l, auto const& r) {
        return std::get<0>(l).id == std::get<0>(r).id;
    });
    sig.named_parameters.erase(it, sig.named_parameters.end());
}

void declaration_visitor::operator()(enum_decl const& ed) const
{
    auto pe = dynamic_pointer_cast<enum_entity>(ctx.u().eregistry().find(ed.name()));
    assert(pe);
    for (auto const& c : ed.cases) {
        pe->cases.emplace_back(c, ctx.u().as_u32string(c));
    }
    std::ranges::sort(pe->cases);
}

function_signature& declaration_visitor::append_fnsig(fn_pure_decl const& fd) const
{
    auto e = ctx.u().eregistry().find(fd.name);
    if (!e) {
        e = make_shared<functional_entity>(fd.name);
        ctx.u().eregistry().insert(e);
    }
    shared_ptr<functional_entity> pe = dynamic_pointer_cast<functional_entity>(e);
    if (!pe) {
        throw exception("an entitity with the same name '%1%' is already defined"_fmt % ctx.u().print(fd.name));
    }
    preliminary_type_visitor tqvis{ ctx };

    function_signature sig;
    for (auto const& parampair : fd.parameters) {
        beng_type paramtype = apply_visitor(tqvis, parampair.type);
        if (!parampair.name) {
            sig.position_parameters.emplace_back(paramtype);
        } else {
            sig.named_parameters.emplace_back(parampair.name->id, paramtype);
        }
    }

    // stable to preserve duplicated parameters order if exist
    std::stable_sort(sig.named_parameters.begin(), sig.named_parameters.end(), [](auto const& l, auto const& r) {
        return std::get<0>(l).id < std::get<0>(r).id;
    });
    // report duplicates?
    auto it = std::unique(sig.named_parameters.begin(), sig.named_parameters.end(), [](auto const& l, auto const& r) {
        return std::get<0>(l).id == std::get<0>(r).id;
    });
    sig.named_parameters.erase(it, sig.named_parameters.end());
    return pe->put_signature(std::move(sig));
}

void declaration_visitor::operator()(fn_pure_decl const& fd) const
{
    function_signature& sig = append_fnsig(fd);
    if (fd.result) {
        preliminary_type_visitor tqvis{ ctx };
        sig.result_type = apply_visitor(tqvis, *fd.result);
    } else { // if result type isn't defined => void
        sig.result_type = beng_tuple_t{};
    }
    ctx.u().build_name(fd.name, sig);
}

void declaration_visitor::operator()(fn_decl_t const& fnd) const
{
    function_signature & sig = append_fnsig(fnd);
    ctx.u().build_name(fnd.name, sig);

    compiler_context fnctx{ ctx.u() };
    if (fnd.result) {
        preliminary_type_visitor tqvis{ ctx };
        fnctx.result = apply_visitor(tqvis, *fnd.result);
    } // if result type isn't defined => void or defined by body expressions
        
    // setup parameters
    size_t paramnum = 0;
    size_t paramcount = sig.parameters_count();
    for (auto const& type : sig.position_parameters) {
        using buff_t = boost::container::small_vector<char, 16>;
        buff_t tailored_param_name = {'$'};
        bool reversed;
        mp::to_string_converter(std::span{ &paramnum, 1 }, std::back_inserter(tailored_param_name), reversed);
        if (reversed) std::reverse(tailored_param_name.begin() + 1, tailored_param_name.end());
        identifier argid = ctx.u().slregistry().resolve(string_view{ tailored_param_name.data(), tailored_param_name.size() });
        auto&& [ve, index] = ctx.new_local_variable(argid, type);
        ve.set_index(static_cast<intptr_t>(paramnum) - paramcount);
        ++paramnum;
    }
    for (auto const&[aname, type] : sig.named_parameters) {
        auto&& [ve, index] = ctx.new_local_variable(aname.id, type);
        ve.set_index(static_cast<intptr_t>(paramnum) - paramcount);
        ++paramnum;
    }

    declaration_visitor dvis{ fnctx };
    for (infunction_declaration_t const& d : fnd.body) {
        apply_visitor(dvis, d);
    }
    fnctx.finish_frame();
    auto fnent = make_shared<function_entity>(sig.mangled_name, sig);
    fnent->set_void(fnd.result  == beng_preliminary_type{ beng_preliminary_tuple_t{} });
    fnent->body = std::move(fnctx.expressions);
    ctx.u().eregistry().insert(fnent);
}

void declaration_visitor::operator()(let_statement_decl const& ld) const
{
    expression_visitor evis{ ctx, ctx.expressions, nullptr };
    auto etype = apply_visitor(evis, ld.expression);
    auto && [ve, index] = ctx.new_local_variable(ld.name, *etype);
    ve.set_index(index);
    ctx.expressions.emplace_back(semantic::set_variable{ &ve });
}

void declaration_visitor::operator()(return_decl const& rd) const
{
    expression_visitor evis{ ctx, ctx.expressions, ctx.result ? &*ctx.result : nullptr };
    apply_visitor(evis, rd.expression);
    ctx.expressions.emplace_back(semantic::return_statement{});
}

void declaration_visitor::check_name(qname const& name) const
{
    if (auto pe = ctx.u().eregistry().find(name); pe) [[unlikely]] {
        throw exception("an entitity with the same name '%1%' is already defined"_fmt % ctx.u().print(name));
    }
}

}
