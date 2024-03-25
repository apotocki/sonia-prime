//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "declaration_visitor.hpp"

#include "fn_compiler_context.hpp"
#include "expression_visitor.hpp"
#include "preliminary_type_visitor.hpp"

#include "../entities/enum_entity.hpp"
#include "../entities/type_entity.hpp"
#include "../entities/functional_entity.hpp"

namespace sonia::lang::beng {

void declaration_visitor::operator()(exten_var & td) const
{
    preliminary_type_visitor tqvis{ ctx };
    beng_type vartype = apply_visitor(tqvis, td.type);
    auto ve = sonia::make_shared<variable_entity>(qname{td.name}, std::move(vartype), variable_entity::kind::EXTERN);
    ctx.u().eregistry().insert(ve);
}

void declaration_visitor::operator()(expression_decl & ed) const
{
    expression_visitor evis{ ctx, ctx.expressions, nullptr };
    apply_visitor(evis, ed.expression);
}

function_signature& declaration_visitor::append_fnsig(fn_pure_decl & fd) const
{
    qname fn_qname = ctx.ns() + fd.name();
    if (!fn_qname.has_prefix(ctx.ns())) {
        throw exception("%1%(%2%,%3%): %4% : not a nested scope identifier"_fmt %
            fd.location().resource % fd.location().line % fd.location().column %
            ctx.u().print(fd.name())
        );
    }

    shared_ptr<functional_entity> pe;
    auto e = ctx.u().eregistry().find(fn_qname);
    if (!e) {
        pe = make_shared<functional_entity>(fn_qname);
        ctx.u().eregistry().insert(pe);
    } else if (pe = dynamic_pointer_cast<functional_entity>(e); !pe) {
        ctx.throw_identifier_redefinition(*e, fd.name(), fd.location());
    }
    
    function_signature sig;
    sig.setup(ctx, fd.parameters);
    sig.normilize(ctx);
    sig.build_mangled_id(ctx.u());
    return pe->put_signature(std::move(sig));
}

void declaration_visitor::operator()(fn_pure_decl & fd) const
{
    function_signature& sig = append_fnsig(fd);
    if (fd.result) {
        preliminary_type_visitor tqvis{ ctx };
        sig.fn_type.result = apply_visitor(tqvis, *fd.result);
    } else { // if result type isn't defined => void
        sig.fn_type.result = beng_tuple_t{};
    }
}

void declaration_visitor::operator()(fn_decl_t & fnd) const
{
    function_signature & sig = append_fnsig(fnd);

    fn_compiler_context fnctx{ ctx, fnd.name() + sig.mangled_id };
    if (fnd.result) {
        preliminary_type_visitor tqvis{ ctx };
        fnctx.result = apply_visitor(tqvis, *fnd.result);
    } // if result type isn't defined => void or defined by body expressions
        
    // setup parameters
    boost::container::small_vector<variable_entity*, 16> params;
    size_t paramnum = 0;
    for (auto const& type : sig.position_parameters()) {
        variable_entity& ve = fnctx.new_position_parameter(paramnum, type);
        params.emplace_back(&ve);
        ++paramnum;
    }
    for (auto const&[aname, type] : sig.named_parameters()) {
        variable_entity& ve = fnctx.new_variable(aname.id, type, variable_entity::kind::LOCAL);
        params.emplace_back(&ve);
    }

    declaration_visitor dvis{ fnctx };
    for (infunction_declaration_t & d : fnd.body) {
        apply_visitor(dvis, d);
    }
    // here all captured variables (if exist) are allocated
    fnctx.finish_frame();

    // assign indexes to parameters & captured variables
    paramnum = 0;
    size_t paramcount = params.size() + fnctx.captured_variables.size();
    for (variable_entity * var : params) {
        var->set_index(static_cast<intptr_t>(paramnum) - paramcount);
        ++paramnum;
    }
    for (auto [from, tovar] : fnctx.captured_variables) {
        tovar->set_index(static_cast<intptr_t>(paramnum) - paramcount);
        ++paramnum;
    }
    if (!fnctx.result || *fnctx.result == beng_type{ beng_tuple_t{} }) { // void result
        sig.fn_type.result = beng_tuple_t{};
    }

    auto fnent = sonia::make_shared<function_entity>(qname{fnctx.ns(), true}, sig);
    
    fnent->body = std::move(fnctx.expressions);
    fnent->captured_variables = std::move(fnctx.captured_variables);
    ctx.u().eregistry().insert(fnent);

    fnent->set_index(ctx.allocate_local_variable_index());

    // initialize variable
    auto fn = fnent->name();
    ctx.expressions.emplace_back(semantic::push_value { function_value{ qname{ fnent->name(), true } } });
    ctx.expressions.emplace_back(semantic::set_variable{ fnent.get() });
}

void declaration_visitor::operator()(let_statement_decl & ld) const
{
    expression_visitor evis{ ctx, ctx.expressions, nullptr };
    auto etype = apply_visitor(evis, ld.expression);
    variable_entity& ve = ctx.new_variable(ld.name, *etype, variable_entity::kind::LOCAL);
    ve.set_index(ctx.allocate_local_variable_index());
    ctx.expressions.emplace_back(semantic::set_variable{ &ve });
}

void declaration_visitor::operator()(return_decl & rd) const
{
    expression_visitor evis{ ctx, ctx.expressions, ctx.result ? &*ctx.result : nullptr };
    apply_visitor(evis, rd.expression);
    ctx.expressions.emplace_back(semantic::return_statement{});
}

}
