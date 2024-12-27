//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "declaration_visitor.hpp"

#include "fn_compiler_context.hpp"
#include "expression_visitor.hpp"
#include "ct_expression_visitor.hpp"

#include "../entities/enum_entity.hpp"
#include "../entities/type_entity.hpp"
#include "../entities/struct/struct_entity.hpp"
#include "../entities/functional_entity.hpp"
#include "../entities/functions/basic_fn_pattern.hpp"

namespace sonia::lang::bang {

inline unit& declaration_visitor::u() const noexcept { return ctx.u(); }

void declaration_visitor::operator()(extern_var const& d) const
{
    auto vartype = apply_visitor(ct_expression_visitor{ ctx }, d.type);
    if (!vartype) {
        throw exception{ u().print(*vartype.error()) };
    }
    //entity_identifier vartype = apply_visitor(preliminary_type_visitor{ ctx }, d.type);
    u().new_variable(qname{d.name.value}, d.name.location, *vartype, variable_entity::kind::EXTERN);
    //THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor extern_var");
}

void declaration_visitor::operator()(struct_decl const& sd) const
{
    // to do: check the allowence of absolute qname
    qname sqn = ctx.ns() / sd.name();
    functional & fnl = u().fregistry().resolve(sqn);
    if (fnl.default_entity()) {
        throw exception{ u().print(identifier_redefinition_error{ annotated_qname_identifier{fnl.id(), sd.location() }, u().eregistry().get(fnl.default_entity()).location() }) };
    }
    entity_signature esig{ fnl.id() };
    auto se = sonia::make_shared<struct_entity>(u().get_typename_entity_identifier(), std::move(esig), sd.fields);
    se->set_location(sd.location());
    u().eregistry().insert(se);
    fnl.set_default_entity(se->id());
}

void declaration_visitor::operator()(expression_statement_t const& ed) const
{
    auto bst = ctx.expressions_branch(); // store branch
    entity_identifier void_eid = u().get_void_entity_identifier();
    ctx.context_type = void_eid;
    if (auto res = apply_visitor(expression_visitor{ ctx }, ed.expression); !res) {
        throw exception(u().print(*res.error()));
    }
    if (ctx.context_type != void_eid) {
        ctx.append_expression(semantic::truncate_values(1, false));
    }
    ctx.collapse_chains(bst);
}

void declaration_visitor::operator()(if_decl_t const& stm) const
{
    ctx.pushed_unnamed_ns();
    expression_visitor vis{ ctx, { u().get_bool_entity_identifier(), get_start_location(stm.condition) } };
    if (auto res = apply_visitor(vis, stm.condition); !res) {
        throw exception(u().print(*res.error()));
    }
    ctx.append_expression(semantic::conditional_t{});
    semantic::conditional_t & cond = get<semantic::conditional_t>(ctx.expressions().back());

    auto bst = ctx.expressions_branch(); // store branch

    if (!stm.true_body.empty()) {
        ctx.push_chain(cond.true_branch);
        for (infunction_statement const& d : stm.true_body) {
            apply_visitor(*this, d);
        }
        ctx.collapse_chains(bst);
    }
    ctx.pop_ns();

    if (!stm.false_body.empty()) {
        ctx.pushed_unnamed_ns();
        ctx.push_chain(cond.false_branch);
        for (infunction_statement const& d : stm.false_body) {
            apply_visitor(*this, d);
        }
        ctx.collapse_chains(bst);
        ctx.pop_ns();
    }
}

void declaration_visitor::operator()(while_decl_t const& wd) const
{
    ctx.pushed_unnamed_ns();
    ctx.append_expression(std::move(semantic::loop_scope_t{}));
    if (wd.continue_expression) {
        ctx.push_chain(get<semantic::loop_scope_t>(ctx.expressions().back()).continue_branch);
        expression_visitor vis{ ctx };
        if (auto res = apply_visitor(vis, *wd.continue_expression); !res) {
            throw exception(u().print(*res.error()));
        }
        if (ctx.context_type != u().get_void_entity_identifier()) {
            ctx.append_expression(semantic::truncate_values(1, false));
            ctx.context_type = u().get_void_entity_identifier();
        }
        ctx.pop_chain();
    }

    ctx.push_chain(get<semantic::loop_scope_t>(ctx.expressions().back()).branch);

    expression_visitor vis{ ctx, { u().get_bool_entity_identifier(), get_start_location(wd.condition) } };
    if (auto res = apply_visitor(vis, wd.condition); !res) {
        throw exception(u().print(*res.error()));
    }

    if (auto const* ppv = get<semantic::push_value>(&ctx.expressions().back())) {
        auto* pb = get<bool>(&ppv->value);
        if (!pb) THROW_INTERNAL_ERROR("a bool value is expected");
        if (!*pb) { // while(false) => skip the loop
            ctx.pop_ns(); // restore ns
            ctx.pop_chain(); // loop_scope_t chain
            ctx.expressions().pop_back(); // semantic::loop_scope_t{}
            return;
        }
        ctx.expressions().pop_back(); // push_value(true)
        for (infunction_statement const& d : wd.body) {
            apply_visitor(*this, d);
        }
        
        ctx.append_expression(semantic::loop_continuer{});
        ctx.pop_ns(); // restore ns
        ctx.pop_chain(); // loop_scope_t chain
        return;
    }
    ctx.append_expression(semantic::conditional_t{});
    semantic::conditional_t& cond = get<semantic::conditional_t>(ctx.expressions().back());

    auto bst = ctx.expressions_branch(); // store branch

    ctx.push_chain(cond.true_branch);
        
    for (infunction_statement const& d : wd.body) {
        apply_visitor(*this, d);
    }
    
    ctx.append_expression(semantic::loop_continuer{});
    cond.true_branch_finished = 1;
    ctx.collapse_chains(bst);

    ctx.push_chain(cond.false_branch);
    ctx.append_expression(semantic::loop_breaker{});
    cond.false_branch_finished = 1;
    ctx.collapse_chains(bst);

    ctx.pop_chain();
    ctx.pop_ns();
}

void declaration_visitor::operator()(continue_statement_t const&) const
{
    // to do: check the existance of enclisong loop
    //THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor continue_statement_t");
    ctx.append_expression(semantic::loop_continuer{});
}

void declaration_visitor::operator()(break_statement_t const&) const
{
    // to do: check the existance of enclisong loop (or switch)
    //THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor break_statement_t");
    ctx.append_expression(semantic::loop_breaker{});
}

void declaration_visitor::append_fnsig(fn_pure_t& fndecl, functional ** ppf) const
{
#if 0
    qname fn_qname = ctx.ns() / fndecl.name();
    if (!fn_qname.has_prefix(ctx.ns())) {
        throw exception("%1%(%2%,%3%): %4% : not a nested scope identifier"_fmt %
            fndecl.location().resource % fndecl.location().line % fndecl.location().column %
            u().print(fndecl.name())
        );
    }
    
    auto fnres = ctx.build_function_descriptor(fndecl);
    if (!fnres.has_value()) {
        throw exception(ctx.u().print(*fnres.error()));
    }
    function_descriptor& fd = fnres.value();

    qname_identifier fn_qnameid = u().qnregistry().resolve(fn_qname);
    functional& f = ctx.u().fregistry().resolve(fn_qnameid);
    if (ppf) *ppf = std::addressof(f);

    f.push(make_shared<fn_pattern>(std::move(fd)));
#endif
    THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor append_fnsig");

#if 0
    
    function_signature sig;
    sig.setup(ctx, fd.parameters);
    sig.normilize(ctx);
    sig.build_mangled_id(ctx.u());
    if (fd.result) {
        preliminary_type_visitor tqvis{ ctx };
        sig.fn_type.result = apply_visitor(tqvis, *fd.result);
    }
    ;
    f.push(std::move(make_shared<fn_signature_pattern>(std::move(sig))));
    
    auto e = ctx.u().eregistry().find(fn_qnameid);
    if (!e) {
        fe = make_shared<functional_entity>(fn_qnameid);
        fe->set_location(fd.location());
        ctx.u().eregistry().insert(fe);
    } else if (fe = dynamic_pointer_cast<functional_entity>(e); !fe) {
        throw exception(ctx.u().print(identifier_redefinition_error{annotated_qname_identifier{fn_qnameid, fd.location()}, e->location()}));
    }

    return fe->put_signature(std::move(sig));
#endif
}

// extern function declaration
void declaration_visitor::operator()(fn_pure_t const& fd) const
{
    THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor fn_pure_t");
#if 0
    shared_ptr<functional_entity> fe;
    function_signature& sig = append_fnsig(fd, fe);
    auto fnm = ctx.u().qnregistry().concat(fe->name(), sig.mangled_id);
    if (!ctx.u().eregistry().find(fnm)) { // external is not registered, will be trying to bind in runtime
        // create the description for late binding
        auto fnent = sonia::make_shared<function_entity>(fnm, function_signature{ sig });
        fnent->set_inline();
        
        // signature
        int64_t sigval = (sig.parameters_count() + 1) * (fnent->is_void() ? -1 : 1);
        fnent->body.emplace_back(semantic::push_value{ mp::integer{ sigval }});
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
#endif
}

#if 0
function_entity & declaration_visitor::append_fnent(fn_pure_t& fnd, function_signature& sig, span<infunction_declaration_t> body) const
{
    THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor append_fnent");
#if 0
    fn_compiler_context fnctx{ ctx, fnd.name() / sig.mangled_id };
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
#endif
}
#endif

void declaration_visitor::operator()(fn_decl_t const& fnd) const
{
    qname fn_qname = ctx.ns() / fnd.name();
    functional& fnl = ctx.u().resolve_functional(fn_qname);

    auto fnptrn = make_shared<generic_fn_pattern>(ctx, fnl, fnd);
    fnl.push(std::move(fnptrn));

#if 0
    //---------------
    fieldset fnfs;
    if (auto err = ctx.build_fieldset(fnd, fnfs); err) {
        throw exception(u().print(*err));
    }

    

    
    auto fptrn = make_shared<basic_fn_pattern>(std::move(fnfs));
    f.push(fptrn);
    function_descriptor& fd = fptrn->
    function_descriptor& fd = fnres.value();
    entity_signature fnsig{ u().get_fn_qname_identifier() };
    
    fn_compiler_context fnctx{ ctx, fnd.name() / u().new_identifier() };

    

    

    shared_ptr<type_entity> fn_type_entity;
    if (fd.result_type()) {
        // create function type
        fnsig.push(u().get_fn_result_identifier(), fd.result_type());
        fn_type_entity = make_shared<type_entity>(u().get_typename_entity_identifier()); // function type is typename
        fn_type_entity->set_signature(std::move(fnsig));
    }


    auto fnent = sonia::make_shared<function_entity>(fnqnid, function_signature{ sig });
    

    declaration_visitor dvis{ fnctx };
    for (infunction_declaration_t& d : fnd.body) {
        apply_visitor(dvis, d);
    }
    // here all captured variables (if exist) are allocated
    fnctx.finish_frame();

    intptr_t paramnum = 0;
    size_t paramcount = params.size() + fnctx.captured_variables.size();
    for (variable_entity* var : params) {
        var->set_index(paramnum - paramcount);
        ++paramnum;
    }
    for (auto [from, tovar] : fnctx.captured_variables) {
        tovar->set_index(paramnum - paramcount);
        ++paramnum;
    }

    THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor fn_decl_t");
    //function_signature& sig = append_fnsig(fnd);
    //function_entity & fnent = append_fnent(fnd, sig, fnd.body);
    //ctx.expressions.emplace_back(semantic::set_variable{ &fnent });
#endif
}

void declaration_visitor::operator()(let_statement const& ld) const
{
    entity_identifier vartype;
    if (ld.type) {
        auto optvartype = apply_visitor(ct_expression_visitor{ ctx }, *ld.type);
        if (!optvartype) {
            throw exception{ u().print(*optvartype.error()) };
        }
        vartype = *optvartype;
    }
    ctx.context_type = u().get_void_entity_identifier();
    if (ld.expression) {
        auto evis = vartype ? expression_visitor{ ctx, { vartype, ld.location() } } : expression_visitor{ ctx };
        if (auto res = apply_visitor(evis, *ld.expression); !res) {
            /*
            BOOST_ASSERT(vartype);
            throw exception(ctx.u().print(basic_general_error{ld.location(),
                ("`%1%` initializing: can not convert to `%2%`\n%3%"_fmt % ctx.u().print(ld.name()) % ctx.u().print(*vartype) %
                    ctx.u().print(*etype.error())).str()
            }));
            */
            throw exception(u().print(*res.error()));
        }
    }
    variable_entity& ve = ctx.new_variable(ld.aname, vartype.self_or(ctx.context_type), variable_entity::kind::LOCAL);
    ve.set_weak(ld.weakness);
    if (!ld.expression) {
        ctx.append_expression(semantic::push_value{ null }); // just declaration, initializtion just declared variable
    } 
    // else do not set variable because we have a result of the expression on stack and consider it as a variable initialization
    // if (ld.expression) { ctx.append_expression(semantic::set_variable{ &ve }); }
}

//void declaration_visitor::operator()(assign_decl_t const& ad) const
//{
//    lvalue_expression_visitor lvis{ ctx };
//    auto e = apply_visitor(lvis, ad.lvalue);
//    if (!e.has_value()) throw exception(ctx.u().print(*e.error()));
//    
//    if (variable_entity const* ve = dynamic_cast<variable_entity const*>(e.value()); ve) {
//        expression_visitor rvis{ ctx, { ve->get_type(), ad.location } };
//        auto opterr = apply_visitor(rvis, ad.rvalue);
//        if (ve->is_weak()) {
//            THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor assign_decl_t");
//            //ctx.append_expression(semantic::invoke_function{ ctx.u().get_builtin_function(unit::builtin_fn::weak_create) });
//        }
//        ctx.append_expression(semantic::set_variable{ ve });
//        if (ve->is_weak()) {
//            ctx.append_expression(semantic::truncate_values(1, false));
//        }
//        return std::move(opterr);
//    } else {
//        const entity* pe = e.value();
//        THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor assign_decl_t");
//    }
//}

void declaration_visitor::operator()(return_decl_t const& rd) const
{
    ctx.context_type = ctx.u().get_void_entity_identifier();
    size_t initial_branch = ctx.expressions_branch();
    auto evis = ctx.result ? expression_visitor{ ctx, { ctx.result, rd.location } } : expression_visitor{ ctx };
    if (auto res = apply_visitor(evis, rd.expression); !res) {
        throw exception(ctx.u().print(*res.error()));
    }
    ctx.collapse_chains(initial_branch);
    if (!ctx.result) {
        ctx.accumulate_result_type(std::move(ctx.context_type));
    }
    ctx.append_expression(semantic::return_statement{});
}

}
