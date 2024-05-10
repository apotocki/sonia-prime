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

namespace sonia::lang::bang {

void declaration_visitor::operator()(extern_var & d) const
{
    preliminary_type_visitor tqvis{ ctx };
    bang_type vartype = apply_visitor(tqvis, d.type);
    
    qname var_qname = ctx.ns() + d.name.value;
    qname_identifier var_qnameid = ctx.u().qnregistry().resolve(var_qname);
    auto e = ctx.u().eregistry().find(var_qnameid);
    if (!e) {
        auto ve = sonia::make_shared<variable_entity>(var_qnameid, std::move(vartype), variable_entity::kind::EXTERN);
        ve->set_location(d.name.location);
        ctx.u().eregistry().insert(ve);
    } else {
        throw exception(ctx.u().print(identifier_redefinition_error{ annotated_qname_identifier{var_qnameid, d.name.location}, e->location() }));
    }
}

void declaration_visitor::operator()(expression_decl_t & ed) const
{
    ctx.context_type = bang_tuple_t{};
    if (auto opterr = apply_visitor(expression_visitor{ ctx }, ed.expression); opterr) {
        throw exception(ctx.u().print(*opterr));
    }
    if (ctx.context_type != bang_tuple_t{}) {
        ctx.append_expression(semantic::truncate_values(1, false));
    }
    ctx.collapse_chains();
}

function_signature& declaration_visitor::append_fnsig(fn_pure_decl& fd, shared_ptr<functional_entity> & fe) const
{
    qname fn_qname = ctx.ns() + fd.name();
    if (!fn_qname.has_prefix(ctx.ns())) {
        throw exception("%1%(%2%,%3%): %4% : not a nested scope identifier"_fmt %
            fd.location().resource % fd.location().line % fd.location().column %
            ctx.u().print(fd.name())
        );
    }
    qname_identifier fn_qnameid = ctx.u().qnregistry().resolve(fn_qname);
    auto e = ctx.u().eregistry().find(fn_qnameid);
    if (!e) {
        fe = make_shared<functional_entity>(fn_qnameid);
        fe->set_location(fd.location());
        ctx.u().eregistry().insert(fe);
    } else if (fe = dynamic_pointer_cast<functional_entity>(e); !fe) {
        throw exception(ctx.u().print(identifier_redefinition_error{annotated_qname_identifier{fn_qnameid, fd.location()}, e->location()}));
    }
    
    function_signature sig;
    sig.setup(ctx, fd.parameters);
    sig.normilize(ctx);
    sig.build_mangled_id(ctx.u());
    if (fd.result) {
        preliminary_type_visitor tqvis{ ctx };
        sig.fn_type.result = apply_visitor(tqvis, *fd.result);
    }
    return fe->put_signature(std::move(sig));
}

void declaration_visitor::operator()(fn_pure_decl & fd) const
{
    shared_ptr<functional_entity> fe;
    function_signature& sig = append_fnsig(fd, fe);
    auto fnm = ctx.u().qnregistry().concat(fe->name(), sig.mangled_id);
    if (!ctx.u().eregistry().find(fnm)) { // external is not registered, will be trying to bind in runtime
        // create the description for late binding
        auto fnent = sonia::make_shared<function_entity>(fnm, function_signature{ sig });
        fnent->set_inline();
        
        // signature
        int64_t sigval = (sig.parameters_count() + 1) * (fnent->is_void() ? -1 : 1);
        fnent->body.emplace_back(semantic::push_value{ decimal{ sigval }});
        // name
        small_string fnmangled = ctx.u().as_string(fnm);
        fnent->body.emplace_back(semantic::push_value{ fnmangled });
        // call itself
        fnent->body.emplace_back(ctx.u().get_builtin_function(unit::builtin_fn::extern_function_call));

        ctx.u().eregistry().insert(fnent);

        // for not inline calls
        fnent->set_index(ctx.allocate_local_variable_index());
        ctx.append_expression(semantic::push_value{ function_value{ fnm } });
        ctx.append_expression(semantic::set_variable{ fnent.get() });
    }
    /*
    function_signature& sig = append_fnsig(fd);
    if (fd.result) {
        preliminary_type_visitor tqvis{ ctx };
        sig.fn_type.result = apply_visitor(tqvis, *fd.result);
    } else { // if result type isn't defined => void
        sig.fn_type.result = bang_tuple_t{};
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

    qname_identifier fnqnid = ctx.u().qnregistry().resolve(fnctx.ns());
    auto fnent = sonia::make_shared<function_entity>(fnqnid, function_signature{sig});
    
    fnent->body = std::move(fnctx.expressions());
    fnent->captured_variables = std::move(fnctx.captured_variables);
    ctx.u().eregistry().insert(fnent);

    fnent->set_index(ctx.allocate_local_variable_index());

    // initialize variable
    ctx.append_expression(semantic::push_value { function_value{ fnent->name() } });
    ctx.append_expression(semantic::set_variable{ fnent.get() });
    return *fnent;
}

void declaration_visitor::operator()(fn_decl_t& fnd) const
{
    shared_ptr<functional_entity> fe;
    function_signature& sig = append_fnsig(fnd, fe);
    function_entity & fnent = append_fnent(fnd, sig, fnd.body);
    //ctx.expressions.emplace_back(semantic::set_variable{ &fnent });
}

void declaration_visitor::operator()(let_statement_decl_t & ld) const
{
    optional<bang_type> vartype;
    if (ld.type) {
        preliminary_type_visitor tvis{ ctx };
        vartype = apply_visitor(tvis, *ld.type);
    }
    ctx.context_type = bang_tuple_t{};
    if (ld.expression) {
        auto evis = vartype ? expression_visitor{ ctx, expected_result_t{ *vartype, ld.location() } } : expression_visitor{ ctx };
        if (auto opterr = apply_visitor(evis, *ld.expression); opterr) {
            /*
            BOOST_ASSERT(vartype);
            throw exception(ctx.u().print(basic_general_error{ld.location(),
                ("`%1%` initializing: can not convert to `%2%`\n%3%"_fmt % ctx.u().print(ld.name()) % ctx.u().print(*vartype) %
                    ctx.u().print(*etype.error())).str()
            }));
            */
            throw exception(ctx.u().print(*opterr));
        }
    }
    variable_entity& ve = ctx.new_variable(ld.name(), vartype.value_or(ctx.context_type), variable_entity::kind::LOCAL);
    ve.set_index(ctx.allocate_local_variable_index());
    ve.set_weak(ld.weakness);
    if (ld.expression) {
        ctx.append_expression(semantic::set_variable{ &ve });
    }
}

void declaration_visitor::operator()(return_decl_t & rd) const
{
    ctx.context_type = bang_tuple_t{};
    size_t initial_branch = ctx.expressions_branch();
    auto evis = ctx.result ? expression_visitor{ ctx, expected_result_t{ *ctx.result, rd.location } } : expression_visitor{ ctx };
    if (auto opterr = apply_visitor(evis, rd.expression); opterr) {
        throw exception(ctx.u().print(*opterr));
    }
    ctx.collapse_chains(initial_branch);
    if (!ctx.result) {
        ctx.accumulate_result_type(std::move(ctx.context_type));
    }
    ctx.append_expression(semantic::return_statement{});
}

}
