//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "declaration_visitor.hpp"

#include <boost/container/flat_map.hpp>

#include "fn_compiler_context.hpp"
#include "expression_visitor.hpp"
#include "ct_expression_visitor.hpp"

//#include "sonia/bang/entities/type_entity.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/struct/struct_entity.hpp"
#include "sonia/bang/entities/struct/struct_fn_pattern.hpp"
#include "sonia/bang/entities/struct/struct_init_pattern.hpp"
#include "sonia/bang/entities/enum/enum_entity.hpp"
#include "sonia/bang/entities/functions/internal_function_entity.hpp"

#include "sonia/bang/functional/basic_fn_pattern.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/parser.hpp"

#include "sonia/bang/entities/functions/expression_resolver.hpp"

#include "sonia/bang/errors/identifier_redefinition_error.hpp"

namespace sonia::lang::bang {

inline unit& declaration_visitor::u() const noexcept { return ctx.u(); }

error_storage declaration_visitor::apply(statement_span initial_decls) const
{
    size_t initial_stack_size = decl_stack_.size();
    decl_stack_.emplace_back(initial_decls);
    do {
        if (decl_stack_.back().empty()) {
            decl_stack_.pop_back();
            continue;
        }
        size_t index = decl_stack_.size() - 1;
        if (auto err = apply_visitor(*this, decl_stack_.back().front()); err) return err;
        decl_stack_[index] = decl_stack_[index].subspan(1);
    } while (decl_stack_.size() > initial_stack_size);
    return {};
}

error_storage declaration_visitor::operator()(include_decl const& d) const
{
    fs::path fpath{ u8string_view{reinterpret_cast<char8_t const*>(d.path.value.data()), d.path.value.size() } };

    parser_context pctx{ u() };
    auto exp_decls = pctx.parse(fpath);
    if (!exp_decls.has_value()) {
        return make_error<basic_general_error>(d.path.location, exp_decls.error());
    }
    u().push_ast(fpath, std::move(pctx.statements()));
    decl_stack_.emplace_back(*exp_decls);
    return {};
}

error_storage declaration_visitor::operator()(extern_var const& d) const
{
    semantic::managed_expression_list el{ u() };
    auto vartype = apply_visitor(ct_expression_visitor{ ctx, el }, d.type());
    if (!vartype) {
        return std::move(vartype.error());
    }
    BOOST_ASSERT(!el);

    qname var_qname = ctx.ns() / d.name.value;
    functional& fnl = u().fregistry_resolve(var_qname);
    auto ve = sonia::make_shared<extern_variable_entity>(vartype->value, fnl.id());
    ve->location = d.name.location;
    u().eregistry_insert(ve);
    
    fnl.set_default_entity(annotated_entity_identifier{ ve->id, d.name.location });
    return {};
}

error_storage declaration_visitor::operator()(using_decl const& ud) const
{
    // to do: check the allowence of absolute qname
    qname uqn = ctx.ns() / ud.name();
    functional& fnl = u().fregistry_resolve(uqn);
    if (!ud.parameters) {
        fnl.set_default_entity(sonia::make_shared<expression_resolver>(ud.location(), ud.expression));
    } else {
        auto fnptrn = make_shared<basic_fn_pattern>(fnl);
        error_storage err = fnptrn->init(ctx, *ud.parameters);
        if (!err) {
            fnptrn->result_constraints.emplace(parameter_constraint_set_t{ .type_expression = ud.expression }, parameter_constraint_modifier_t::const_value);
            fnl.push(std::move(fnptrn));
        }
        return err;
    }
    //entity_signature esig{ fnl.id() };
    //auto se = sonia::make_shared<type_entity>(u().get_typename_entity_identifier(), std::move(esig));
    //se->set_location(ud.location());
    //u().eregistry_insert(se);
    //fnl.set_default_entity(se->id());
    return {};
}

/*
struct field_t
{
    annotated_identifier name;
    parameter_constraint_modifier_t modifier;
    syntax_expression_t type;
    optional<syntax_expression_t> value;
};

struct parameter
{
    parameter_name name;
    parameter_constraint_modifier_t modifier;
    parameter_constraint_set<ExprT> constraints;
};

using parameter_name = variant<named_parameter_name, unnamed_parameter_name, varnamed_parameter_name>;
*/
error_storage declaration_visitor::operator()(struct_decl const& sd) const
{
    return apply_visitor(make_functional_visitor<error_storage>([this, &sd](auto const& v) {
        unit& u = ctx.u();
        if constexpr (std::is_same_v<annotated_qname const&, decltype(v)>) {
            annotated_qname const& qn = v;

            functional& fnl = u.fregistry_resolve(ctx.ns() / qn.value);
            auto sent = sonia::make_shared<struct_entity>(u, fnl, sd.body);
            u.eregistry_insert(sent);
            annotated_entity_identifier aeid{ sent->id, qn.location };
            fnl.set_default_entity(aeid);

            functional& init_fnl = u.fregistry_resolve(u.get(builtin_qnid::init));
            auto initptrn = sonia::make_shared<struct_init_pattern>(init_fnl, sd.body);
            if (error_storage err = initptrn->init(ctx, aeid); err) return err;
            init_fnl.push(std::move(initptrn));
        } else { // if constexpr (std::is_same_v<fn_pure_t const&, decltype(v)>) {
            // to do: check the allowence of absolute qname
            fn_pure_t const& fn = v;
            qname fn_qname = ctx.ns() / fn.name();
            functional& fnl = u.fregistry_resolve(fn_qname);
            auto ptrn = sonia::make_shared<struct_fn_pattern>(fnl, sd.body);
            if (error_storage err = ptrn->init(ctx, fn); err) return err;
            fnl.push(std::move(ptrn));

            functional& init_fnl = u.fregistry_resolve(u.get(builtin_qnid::init));
            auto initptrn = sonia::make_shared<struct_init_pattern>(init_fnl, sd.body);
            if (error_storage err = initptrn->init(ctx, annotated_qname{ fn_qname, fn.location() }, fn.parameters); err) return err;
            init_fnl.push(std::move(initptrn));
        }
        return error_storage{};
    }), sd.decl);
}

error_storage declaration_visitor::operator()(enum_decl const& ed) const
{
    unit& u = ctx.u();
    functional& fnl = u.fregistry_resolve(ctx.ns() / ed.name.value);
    auto eent = make_shared<enum_entity>(u, fnl, ed.cases);
    u.eregistry_insert(eent);
    annotated_entity_identifier aeid{ eent->id, ed.name.location };
    fnl.set_default_entity(aeid);
    return {};
}

error_storage declaration_visitor::operator()(expression_statement_t const& ed) const
{
    //auto bst = ctx.expressions_branch(); // store branch
    semantic::managed_expression_list el{ u() };
    auto res = apply_visitor(base_expression_visitor{ ctx, el }, ed.expression);
    if (!res) return std::move(res.error());
    syntax_expression_result_t & er = res->first;
    ctx.push_scope();
    for (auto& [varid, t, sp] : er.temporaries) {
        ctx.expressions().splice_back(el, sp);
        ctx.push_scope_variable(
            annotated_identifier{ u().new_identifier() },
            local_variable{ .type = t, .varid = varid, .is_weak = false },
            fnent);
    }
    ctx.expressions().splice_back(el, er.expressions);
    size_t scope_sz = ctx.current_scope_binding().size();
    ctx.pop_scope();
    if (!er.is_const_result && er.type() != u().get(builtin_eid::void_)) {
        ++scope_sz;
    }
    if (scope_sz) {
        ctx.append_expression(semantic::truncate_values(scope_sz, false));
    }
    // ctx.collapse_chains(bst);
    
    return {};
}

error_storage declaration_visitor::do_rt_if_decl(if_decl const& stm) const
{
    ctx.append_expression(semantic::conditional_t{});
    semantic::conditional_t& cond = get<semantic::conditional_t>(ctx.expressions().back());

    auto bst = ctx.expressions_branch(); // store branch

    if (!stm.true_body.empty()) {
        ctx.push_scope();
        SCOPE_EXIT([this] { ctx.pop_scope(); });
        semantic::managed_expression_list true_branch{ u() };
        ctx.push_chain(true_branch);
        if (auto err = apply(stm.true_body); err) return std::move(err);
        cond.true_branch = ctx.store_semantic_expressions(std::move(true_branch));
        ctx.collapse_chains(bst);
    }

    if (!stm.false_body.empty()) {
        ctx.push_scope();
        SCOPE_EXIT([this] { ctx.pop_scope(); });
        semantic::managed_expression_list false_branch{ u() };
        ctx.push_chain(false_branch);
        if (auto err = apply(stm.false_body); err) return std::move(err);
        cond.false_branch = ctx.store_semantic_expressions(std::move(false_branch));
        ctx.collapse_chains(bst);
    }
    return {};
}

error_storage declaration_visitor::operator()(if_decl const& stm) const
{
    semantic::managed_expression_list el{ u() };
    base_expression_visitor vis{ ctx, el, { u().get(builtin_eid::boolean), get_start_location(stm.condition) } };
    auto res = apply_visitor(vis, stm.condition);
    if (!res) return std::move(res.error());

    syntax_expression_result_t & er = res->first;
    if (er.is_const_result) { // constexpr result
        entity_identifier v = er.value();
        BOOST_ASSERT(v == u().get(builtin_eid::false_) || v == u().get(builtin_eid::true_));
        statement_span body = (v == u().get(builtin_eid::true_) ? stm.true_body : stm.false_body);
        ctx.push_scope();
        SCOPE_EXIT([this] { ctx.pop_scope(); });
        return apply(body);
    } else {
        ctx.expressions().splice_back(el, er.expressions);
        return do_rt_if_decl(stm);
    }
}

error_storage declaration_visitor::operator()(for_decl const& fd) const
{
    THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor for_decl");
}

error_storage declaration_visitor::operator()(while_decl const& wd) const
{
    ctx.push_scope();
    ctx.append_expression(std::move(semantic::loop_scope_t{}));
    if (wd.continue_expression) {
        semantic::loop_scope_t& ls = get<semantic::loop_scope_t>(ctx.expressions().back());
        semantic::managed_expression_list continue_branch{ u() };
        ctx.push_chain(continue_branch);
        
        expression_visitor vis{ ctx };
        if (auto res = apply_visitor(vis, *wd.continue_expression); !res) {
            return std::move(res.error());
        }
        if (ctx.context_type != u().get(builtin_eid::void_)) {
            ctx.append_expression(semantic::truncate_values(1, false));
            ctx.context_type = u().get(builtin_eid::void_);
        }
        ls.continue_branch = ctx.store_semantic_expressions(std::move(continue_branch));
        ctx.pop_chain();
    }

    semantic::loop_scope_t& ls = get<semantic::loop_scope_t>(ctx.expressions().back());
    semantic::managed_expression_list branch{ u() };
    ctx.push_chain(branch);
    //ctx.push_chain(get<semantic::loop_scope_t>(ctx.expressions().back()).branch);

    expression_visitor vis{ ctx, { u().get(builtin_eid::boolean), get_start_location(wd.condition) } };
    if (auto res = apply_visitor(vis, wd.condition); !res) {
        return std::move(res.error());
    }

    if (auto const* ppv = get<semantic::push_value>(&ctx.expressions().back())) {
        auto* pb = get<bool>(&ppv->value);
        if (!pb) THROW_INTERNAL_ERROR("a bool value is expected");
        if (!*pb) { // while(false) => skip the loop
            ctx.pop_scope(); // restore ns
            ctx.pop_chain(); // loop_scope_t chain
            ctx.expressions().pop_back(); // semantic::loop_scope_t{}
            return {};
        }
        ctx.expressions().pop_back(); // push_value(true)
        if (auto err = apply(wd.body); err) {
            return std::move(err);
        }
        //for (auto const& d : wd.body) {
        //    apply_visitor(*this, d);
        //}
        
        ctx.append_expression(semantic::loop_continuer{});
        ls.branch = ctx.store_semantic_expressions(std::move(branch));
        ctx.pop_scope(); // restore ns
        ctx.pop_chain(); // loop_scope_t chain
        return {};
    }
    ctx.append_expression(semantic::conditional_t{});
    semantic::conditional_t& cond = get<semantic::conditional_t>(ctx.expressions().back());

    auto bst = ctx.expressions_branch(); // store branch

    semantic::managed_expression_list true_branch{ u() };
    ctx.push_chain(true_branch);

    if (auto err = apply(wd.body); err) {
        return std::move(err);
    }
    //for (auto const& d : wd.body) {
    //    apply_visitor(*this, d);
    //}
    
    ctx.append_expression(semantic::loop_continuer{});
    cond.true_branch = ctx.store_semantic_expressions(std::move(true_branch));
    cond.true_branch_finished = 1;
    ctx.collapse_chains(bst);

    semantic::managed_expression_list false_branch{ u() };
    ctx.push_chain(false_branch);
    ctx.append_expression(semantic::loop_breaker{});
    cond.false_branch = ctx.store_semantic_expressions(std::move(false_branch));
    cond.false_branch_finished = 1;
    ctx.collapse_chains(bst);

    ls.branch = ctx.store_semantic_expressions(std::move(branch));
    ctx.pop_chain();
    ctx.pop_scope();

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
        ctx.u().eregistry_insert(fe);
    } else if (fe = dynamic_pointer_cast<functional_entity>(e); !fe) {
        throw exception(ctx.u().print(identifier_redefinition_error{annotated_qname_identifier{fn_qnameid, fd.location()}, e->location()}));
    }

    return fe->put_signature(std::move(sig));
#endif
}

// extern function declaration
error_storage declaration_visitor::operator()(fn_pure_t const& fd) const
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

    auto fnptrn = make_shared<generic_fn_pattern>(fnl);
    error_storage err = fnptrn->init(ctx, fnd);
    if (!err) fnl.push(std::move(fnptrn));
    return err;
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
    semantic::managed_expression_list el{ u() };

    entity_identifier vartype;
    if (ld.type) {
        auto optvartype = apply_visitor(ct_expression_visitor{ ctx, el }, *ld.type);
        if (!optvartype) {
            return std::move(optvartype.error());
        }
        BOOST_ASSERT(!optvartype->expressions);
        vartype = optvartype->value;
    }

    small_vector<std::pair<identifier, syntax_expression_result_t>, 8> results;

    prepared_call pcall{ ctx, el, ld.location() };
    for (auto const& e : ld.expressions) {
        pcall.args.emplace_back(e);
    }
    if (auto err = pcall.prepare(); err) return std::move(err);

    for (auto const& e : pcall.args) {
        auto [pname, expr] = *e;
        auto res = apply_visitor(base_expression_visitor{ ctx, el, { vartype, ld.location() } }, expr);
        if (!res) { return std::move(res.error()); }
        syntax_expression_result& ser = res->first;
        identifier name = pname ? pname->value : identifier{};
        results.emplace_back(name, std::move(ser));
    }

    //ctx.push_scope();
    //for (auto &[name, ser] : results) {
    //    for (auto& [varid, t, sp] : ser.temporaries) {
    //        ctx.expressions().splice_back(el, sp);
    //        ctx.push_scope_variable(
    //            annotated_identifier{ u().new_identifier() },
    //            local_variable{ .type = t, .varid = varid, .is_weak = false },
    //            fnent);
    //    }
    //}

    auto push_temporaries = [&el, this](auto& temporaries) {
        for (auto& [varid, t, sp] : temporaries) {
            ctx.expressions().splice_back(el, sp);
            ctx.push_scope_variable(
                annotated_identifier{ u().new_identifier() },
                local_variable{ .type = t, .varid = varid, .is_weak = false },
                fnent);
        }
    };

    if (results.size() == 1 && !results.front().first) {
        syntax_expression_result_t& er = results.front().second;
        ctx.push_scope();
        push_temporaries(er.temporaries);
        ctx.expressions().splice_back(el, er.expressions);
        auto scope_sz = ctx.current_scope_binding().size();
        ctx.pop_scope();

        if (er.is_const_result) {
            ctx.new_constant(ld.aname, er.value());
        } else {
            ctx.push_scope_variable(
                ld.aname,
                local_variable{ .type = er.type(), .varid = u().new_variable_identifier(), .is_weak = ld.weakness },
                fnent);
        }
        if (scope_sz) {
            u().push_back_expression(ctx.expressions(), semantic::truncate_values(scope_sz, !er.is_const_result));
        }
        return {};
    }

    //small_vector<std::tuple<variable_identifier, entity_identifier, semantic::expression_span>, 2> temporaries;
    // we will return empty_entity with the tuple type
    // now build the type
    boost::container::small_flat_multimap<identifier, entity_identifier, 8> field_map;
    entity_signature sig{ u().get(builtin_qnid::tuple), u().get(builtin_eid::typename_) };
    for (auto & [id, er] : results) {
        ctx.push_scope();
        push_temporaries(er.temporaries);
        ctx.expressions().splice_back(el, er.expressions);
        auto scope_sz = ctx.current_scope_binding().size();
        ctx.pop_scope();
        entity_identifier field_eid;
        if (er.is_const_result) {
            field_eid = er.value();
        } else {
            identifier unnamedid = u().new_identifier();
            ctx.push_scope_variable(
                annotated_identifier{ unnamedid },
                local_variable{ .type = er.type(), .varid = u().new_variable_identifier(), .is_weak = ld.weakness },
                fnent);
            field_eid = u().make_qname_entity(qname{ unnamedid, false }).id;
        }
        if (scope_sz) {
            u().push_back_expression(ctx.expressions(), semantic::truncate_values(scope_sz, !er.is_const_result));
        }
            
        if (id) {
            field_map.emplace(id, field_eid);
        } else {
            sig.push_back(field_descriptor{ field_eid, true });
        }
    }
    // for each key in small_flat_multimap
    for (auto it = field_map.begin(); it != field_map.end();) {
        identifier name = it->first;
        auto range = field_map.equal_range(it->first);
        if (std::next(range.first) == range.second) {
            sig.emplace_back(name, it->second, true);
            ++it;
        } else {
            BOOST_ASSERT(it == range.first);
            entity_signature innersig{ u().get(builtin_qnid::tuple), u().get(builtin_eid::typename_) };
            for (; it != range.second; ++it) {
                innersig.emplace_back(it->second, true);
            }
            indirect_signatured_entity innersmpl{ innersig };
            entity const& const_inner_type = u().eregistry_find_or_create(innersmpl, [&innersig]() {
                return make_shared<basic_signatured_entity>(std::move(innersig));
            });
            sig.emplace_back(name, u().make_empty_entity(const_inner_type), true);
        }
    }
    entity const& tuple_type = u().make_basic_signatured_entity(std::move(sig));
    ctx.new_constant(ld.aname, u().make_empty_entity(tuple_type).id);
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
