//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "declaration_visitor.hpp"

#include "sonia/small_vector.hpp"

#include "fn_compiler_context.hpp"
#include "expression_visitor.hpp"
#include "ct_expression_visitor.hpp"

#include "../entities/enum_entity.hpp"
#include "../entities/type_entity.hpp"
#include "../entities/struct/struct_entity.hpp"
#include "../entities/functional_entity.hpp"
#include "../entities/functions/basic_fn_pattern.hpp"

#include "sonia/bang/parser.hpp"

#include "sonia/bang/entities/functions/expression_resolver.hpp"
#include "sonia/bang/entities/struct/struct_fn_pattern.hpp"

#include "sonia/bang/errors/identifier_redefinition_error.hpp"

namespace sonia::lang::bang {

inline unit& declaration_visitor::u() const noexcept { return ctx.u(); }

error_storage declaration_visitor::apply(span<const statement> initial_decls) const
{
    decl_stack_.clear();
    decl_stack_.emplace_back(initial_decls);
    for (; !decl_stack_.empty(); ) {
        if (decl_stack_.back().empty()) {
            decl_stack_.pop_back();
            continue;
        }
        size_t index = decl_stack_.size() - 1;
        if (auto err = apply_visitor(*this, decl_stack_.back().front()); err) return err;
        decl_stack_[index] = decl_stack_[index].subspan(1);
    }
    return {};
}

error_storage declaration_visitor::operator()(include_decl const& d) const
{
    fs::path fpath{ u8string_view{reinterpret_cast<char8_t const*>(d.path.value.data()), d.path.value.size() } };

    parser_context pctx{ u() };
    auto exp_decls = pctx.parse(fpath);
    if (!exp_decls.has_value()) {
        throw exception{ ctx.u().print(basic_general_error{ d.path.location, exp_decls.error() }) };
    }
    decl_stack_.emplace_back(*exp_decls);
    return {};
}

error_storage declaration_visitor::operator()(extern_var const& d) const
{
    auto vartype = apply_visitor(ct_expression_visitor{ ctx }, d.type);
    if (!vartype) {
        return std::move(vartype.error());
    }
    //entity_identifier vartype = apply_visitor(preliminary_type_visitor{ ctx }, d.type);
    //u().new_variable(qname{d.name.value}, d.name.location, *vartype, variable_entity::kind::EXTERN);
    ctx.new_variable(d.name, *vartype, variable_entity::kind::EXTERN);
    //THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor extern_var");
    return {};
}

error_storage declaration_visitor::operator()(using_decl const& ud) const
{
    // to do: check the allowence of absolute qname
    qname uqn = ctx.ns() / ud.name();
    functional& fnl = u().fregistry().resolve(uqn);
    if (!ud.parameters) {
        return fnl.set_default_entity(make_shared<expression_resolver>(ud.location(), ud.expression));
    } else {
        THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor using_decl");
    }
    //entity_signature esig{ fnl.id() };
    //auto se = sonia::make_shared<type_entity>(u().get_typename_entity_identifier(), std::move(esig));
    //se->set_location(ud.location());
    //u().eregistry_insert(se);
    //fnl.set_default_entity(se->id());
    return {};
}

error_storage declaration_visitor::operator()(struct_decl const& sd) const
{
    struct struct_decl_visitor : static_visitor<error_storage>
    {
        fn_compiler_context& ctx;
        field_list_t const& fields;

        struct_decl_visitor(fn_compiler_context& c, field_list_t const& fs) noexcept : ctx{ c }, fields{ fs } {}

        error_storage operator()(annotated_qname const& qn) const
        {
            unit& u = ctx.u();
            functional& fnl = u.fregistry().resolve(ctx.ns() / qn.value);
            auto sent = make_shared<struct_entity>(u.get(builtin_eid::typename_), entity_signature{ fnl.id() }, fields);
            u.eregistry_insert(sent);
            return fnl.set_default_entity(annotated_entity_identifier{ sent->id(), qn.location });
        }

        error_storage operator()(fn_pure const& fn) const
        {
            // to do: check the allowence of absolute qname
            qname fn_qname = ctx.ns() / fn.name();
            functional& fnl = ctx.u().fregistry().resolve(fn_qname);
            fnl.push(make_shared<struct_fn_pattern>(ctx, fnl, fn, fields));
            return {};
        };
    };

    return apply_visitor(struct_decl_visitor{ ctx, sd.fields }, sd.decl);
    
    //if (auto const* pfn = sd.as_fn(); pfn) {
    //    functional& fnl = u().fregistry().resolve(ctx.ns() / pfn->name());
    //    fnl.push(make_shared<struct_fn_pattern>(ctx, fnl, *pfn, sd.fields));
    //    return {};
    //} else {
    //    annotated_qname const* aqn = sd.as_name();
    //    BOOST_ASSERT(aqn);
    //    functional& fnl = u().fregistry().resolve(ctx.ns() / aqn->value);

    //    auto sent = make_shared<struct_entity>(ctx.u().get(builtin_eid::typename_), entity_signature{ fnl.id() });
    //    u().eregistry_insert(sent);
    //    return fnl.set_default_entity(annotated_entity_identifier{ sent->id(), aqn->location });
    //}
}

error_storage declaration_visitor::operator()(expression_statement_t const& ed) const
{
    auto bst = ctx.expressions_branch(); // store branch
    entity_identifier void_eid = u().get(builtin_eid::void_);
    ctx.context_type = void_eid;
    if (auto res = apply_visitor(expression_visitor{ ctx }, ed.expression); !res) {
        return std::move(res.error());
    }
    if (ctx.context_type != void_eid) {
        ctx.append_expression(semantic::truncate_values(1, false));
    }
    ctx.collapse_chains(bst);
    return {};
}

error_storage declaration_visitor::operator()(if_decl_t const& stm) const
{
    ctx.pushed_unnamed_ns();
    expression_visitor vis{ ctx, { u().get(builtin_eid::boolean), get_start_location(stm.condition) } };
    if (auto res = apply_visitor(vis, stm.condition); !res) {
        return std::move(res.error());
    }
    ctx.append_expression(semantic::conditional_t{});
    semantic::conditional_t & cond = get<semantic::conditional_t>(ctx.expressions().back());

    auto bst = ctx.expressions_branch(); // store branch

    if (!stm.true_body.empty()) {
        ctx.push_chain(cond.true_branch);
        for (auto const& d : stm.true_body) {
            apply_visitor(*this, d);
        }
        ctx.collapse_chains(bst);
    }
    ctx.pop_ns();

    if (!stm.false_body.empty()) {
        ctx.pushed_unnamed_ns();
        ctx.push_chain(cond.false_branch);
        for (auto const& d : stm.false_body) {
            apply_visitor(*this, d);
        }
        ctx.collapse_chains(bst);
        ctx.pop_ns();
    }
    return {};
}

error_storage declaration_visitor::operator()(while_decl_t const& wd) const
{
    ctx.pushed_unnamed_ns();
    ctx.append_expression(std::move(semantic::loop_scope_t{}));
    if (wd.continue_expression) {
        ctx.push_chain(get<semantic::loop_scope_t>(ctx.expressions().back()).continue_branch);
        expression_visitor vis{ ctx };
        if (auto res = apply_visitor(vis, *wd.continue_expression); !res) {
            return std::move(res.error());
        }
        if (ctx.context_type != u().get(builtin_eid::void_)) {
            ctx.append_expression(semantic::truncate_values(1, false));
            ctx.context_type = u().get(builtin_eid::void_);
        }
        ctx.pop_chain();
    }

    ctx.push_chain(get<semantic::loop_scope_t>(ctx.expressions().back()).branch);

    expression_visitor vis{ ctx, { u().get(builtin_eid::boolean), get_start_location(wd.condition) } };
    if (auto res = apply_visitor(vis, wd.condition); !res) {
        return std::move(res.error());
    }

    if (auto const* ppv = get<semantic::push_value>(&ctx.expressions().back())) {
        auto* pb = get<bool>(&ppv->value);
        if (!pb) THROW_INTERNAL_ERROR("a bool value is expected");
        if (!*pb) { // while(false) => skip the loop
            ctx.pop_ns(); // restore ns
            ctx.pop_chain(); // loop_scope_t chain
            ctx.expressions().pop_back(); // semantic::loop_scope_t{}
            return {};
        }
        ctx.expressions().pop_back(); // push_value(true)
        for (auto const& d : wd.body) {
            apply_visitor(*this, d);
        }
        
        ctx.append_expression(semantic::loop_continuer{});
        ctx.pop_ns(); // restore ns
        ctx.pop_chain(); // loop_scope_t chain
        return {};
    }
    ctx.append_expression(semantic::conditional_t{});
    semantic::conditional_t& cond = get<semantic::conditional_t>(ctx.expressions().back());

    auto bst = ctx.expressions_branch(); // store branch

    ctx.push_chain(cond.true_branch);
        
    for (auto const& d : wd.body) {
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

    return {};
}

error_storage declaration_visitor::operator()(continue_statement_t const&) const
{
    // to do: check the existance of enclisong loop
    //THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor continue_statement_t");
    ctx.append_expression(semantic::loop_continuer{});
    return {};
}

error_storage declaration_visitor::operator()(break_statement_t const&) const
{
    // to do: check the existance of enclisong loop (or switch)
    //THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor break_statement_t");
    ctx.append_expression(semantic::loop_breaker{});
    return {};
}

void declaration_visitor::append_fnsig(fn_pure& fndecl, functional ** ppf) const
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
        ctx.u().eregistry_insert(fe);
    } else if (fe = dynamic_pointer_cast<functional_entity>(e); !fe) {
        throw exception(ctx.u().print(identifier_redefinition_error{annotated_qname_identifier{fn_qnameid, fd.location()}, e->location()}));
    }

    return fe->put_signature(std::move(sig));
#endif
}

// extern function declaration
error_storage declaration_visitor::operator()(fn_pure const& fd) const
{
    THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor fn_pure");
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

        ctx.u().eregistry_insert(fnent);

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
function_entity & declaration_visitor::append_fnent(fn_pure& fnd, function_signature& sig, span<infunction_declaration_t> body) const
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
    ctx.u().eregistry_insert(fnent);

    fnent->set_index(ctx.allocate_local_variable_index());

    // initialize variable
    ctx.append_expression(semantic::push_value { function_value{ fnent->name() } });
    ctx.append_expression(semantic::set_variable{ fnent.get() });
    return *fnent;
#endif
}
#endif

error_storage declaration_visitor::operator()(fn_decl_t const& fnd) const
{
    qname fn_qname = ctx.ns() / fnd.name();
    functional& fnl = ctx.u().resolve_functional(fn_qname);

    auto fnptrn = make_shared<generic_fn_pattern>(ctx, fnl, fnd);
    fnl.push(std::move(fnptrn));
    return {};
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

error_storage declaration_visitor::operator()(let_statement const& ld) const
{
    entity_identifier vartype;
    if (ld.type) {
        auto optvartype = apply_visitor(ct_expression_visitor{ ctx }, *ld.type);
        if (!optvartype) {
            return std::move(optvartype.error());
        }
        vartype = *optvartype;
    }
    ctx.context_type = u().get(builtin_eid::void_);
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
            return std::move(res.error());
        }
    }
    variable_entity& ve = ctx.new_variable(ld.aname, vartype.self_or(ctx.context_type), variable_entity::kind::LOCAL);
    ve.set_weak(ld.weakness);
    if (!ld.expression) {
        ctx.append_expression(semantic::push_value{ null }); // just declaration, initializtion just declared variable
    } 
    // else do not set variable because we have a result of the expression on stack and consider it as a variable initialization
    // if (ld.expression) { ctx.append_expression(semantic::set_variable{ &ve }); }
    return {};
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

error_storage declaration_visitor::operator()(return_decl_t const& rd) const
{
    ctx.context_type = ctx.u().get(builtin_eid::void_);
    size_t initial_branch = ctx.expressions_branch();
    auto evis = ctx.result ? expression_visitor{ ctx, { ctx.result, rd.location } } : expression_visitor{ ctx };
    if (auto res = apply_visitor(evis, rd.expression); !res) {
        return std::move(res.error());
    }
    ctx.collapse_chains(initial_branch);
    if (!ctx.result) {
        ctx.accumulate_result_type(std::move(ctx.context_type));
    }
    ctx.append_expression(semantic::return_statement{});
    return {};
}

}
