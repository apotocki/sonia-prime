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

void declaration_visitor::operator()(extern_var & td) const
{
    preliminary_type_visitor tqvis{ ctx };
    beng_type vartype = apply_visitor(tqvis, td.type);
    auto ve = sonia::make_shared<variable_entity>(qname{td.name.value}, std::move(vartype), variable_entity::kind::EXTERN);
    ctx.u().eregistry().insert(ve);
}

void declaration_visitor::operator()(expression_decl_t & ed) const
{
    expression_visitor evis{ ctx };
    auto r = apply_visitor(evis, ed.expression);
    if (!r.has_value()) {
        throw exception(ctx.u().print(*r.error()));
    }
    if (r.value() != beng_tuple_t{}) {
        ctx.append_expression(semantic::truncate_values(1, false));
    }
    ctx.collapse_chains();
}

function_signature& declaration_visitor::append_fnsig(fn_pure_decl& fd) const
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
        throw exception(ctx.u().print(identifier_redefinition_error{fd.location(), e->location(), fd.name()}));
    }
    
    function_signature sig;
    sig.setup(ctx, fd.parameters);
    sig.normilize(ctx);
    sig.build_mangled_id(ctx.u());
    if (fd.result) {
        preliminary_type_visitor tqvis{ ctx };
        sig.fn_type.result = apply_visitor(tqvis, *fd.result);
    }
    return pe->put_signature(std::move(sig));
}

void declaration_visitor::operator()(fn_pure_decl & fd) const
{
    append_fnsig(fd);
    /*
    function_signature& sig = append_fnsig(fd);
    if (fd.result) {
        preliminary_type_visitor tqvis{ ctx };
        sig.fn_type.result = apply_visitor(tqvis, *fd.result);
    } else { // if result type isn't defined => void
        sig.fn_type.result = beng_tuple_t{};
    }
    */
}

function_entity & declaration_visitor::append_fnent(fn_pure_decl& fnd, function_signature& sig, span<infunction_declaration_t> body) const
{
    fn_compiler_context fnctx{ ctx, fnd.name() + sig.mangled_id };
    if (fnd.result) {
        fnctx.result = sig.fn_type.result;
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
        variable_entity& ve = fnctx.new_variable(aname.value, type, variable_entity::kind::LOCAL);
        params.emplace_back(&ve);
    }

    declaration_visitor dvis{ fnctx };
    for (infunction_declaration_t & d : body) {
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
    sig.fn_type.result = fnctx.compute_result_type();

    auto fnent = sonia::make_shared<function_entity>(qname{fnctx.ns(), true}, function_signature{sig});
    
    fnent->body = std::move(fnctx.expressions());
    fnent->captured_variables = std::move(fnctx.captured_variables);
    ctx.u().eregistry().insert(fnent);

    fnent->set_index(ctx.allocate_local_variable_index());

    // initialize variable
    ctx.append_expression(semantic::push_value { function_value{ qname{ fnent->name(), true } } });
    ctx.append_expression(semantic::set_variable{ fnent.get() });
    return *fnent;
}

void declaration_visitor::operator()(fn_decl_t& fnd) const
{
    function_signature& sig = append_fnsig(fnd);
    function_entity & fnent = append_fnent(fnd, sig, fnd.body);
    //ctx.expressions.emplace_back(semantic::set_variable{ &fnent });
}

void declaration_visitor::operator()(let_statement_decl_t & ld) const
{
    optional<beng_type> vartype;
    if (ld.type) {
        preliminary_type_visitor tvis{ ctx };
        vartype = apply_visitor(tvis, *ld.type);
    }
    if (ld.expression) {
        auto evis = vartype ? expression_visitor{ ctx, expected_result_t{ *vartype, ld.location() } } : expression_visitor{ ctx };
        auto etype = apply_visitor(evis, *ld.expression);
        if (!etype.has_value()) {
            throw exception(ctx.u().print(*etype.error()));
            /*
            BOOST_ASSERT(vartype);
            throw exception(ctx.u().print(basic_general_error{ld.location(),
                ("`%1%` initializing: can not convert to `%2%`\n%3%"_fmt % ctx.u().print(ld.name()) % ctx.u().print(*vartype) %
                    ctx.u().print(*etype.error())).str()
            }));
            */
        }
        if (!vartype) {
            vartype = etype.value();
        }
    }
    variable_entity& ve = ctx.new_variable(ld.name(), *vartype, variable_entity::kind::LOCAL);
    ve.set_index(ctx.allocate_local_variable_index());
    ve.set_weak(ld.weakness);
    if (ld.expression) {
        ctx.append_expression(semantic::set_variable{ &ve });
    }
}

void declaration_visitor::operator()(return_decl_t & rd) const
{
    auto evis = ctx.result ? expression_visitor{ ctx, expected_result_t{ *ctx.result, rd.location } } : expression_visitor{ ctx };
    auto optetype = apply_visitor(evis, rd.expression);
    if (!optetype.has_value()) { throw exception(ctx.u().print(*optetype.error())); }
    if (!ctx.result) {
        ctx.accumulate_result_type(std::move(*optetype));
    }
    ctx.append_expression(semantic::return_statement{});
}

}
