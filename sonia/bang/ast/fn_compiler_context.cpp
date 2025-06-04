//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "fn_compiler_context.hpp"

#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/range/adaptor/reversed.hpp>

#include "sonia/bang/entities/functional.hpp"
#include "sonia/bang/entities/functions/internal_function_entity.hpp"

#include "sonia/bang/errors/identifier_redefinition_error.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

#if 0
struct parameter_pack_element_type_visitor : static_visitor<std::expected<pattern_expression_t, error_storage>>
{
    fn_compiler_context& ctx;
    syntax_expression_t const& element_type_expression_;

    explicit parameter_pack_element_type_visitor(fn_compiler_context& c, syntax_expression_t const& et)
        : ctx{ c }
        , element_type_expression_{ et }
    {}

    inline unit& u() const noexcept { return ctx.u(); }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("parameter_pack_element_type_visitor not implemented expression");
    }

    result_type operator()(annotated_qname_identifier const& aqi)
    {
        functional const& fnl = ctx.u().fregistry().resolve(aqi.value);
        entity_identifier element_type = fnl.default_entity();
        if (!element_type) {
            return std::unexpected(make_error<basic_general_error>(
                aqi.location, //get_start_location(element_type_expression_),
                "identifier is not a type"sv));
        }
        return this->operator()(element_type);
    }

    result_type operator()(entity_identifier eid)
    {
        functional& ellipsis_fnl = u().fregistry().resolve(u().get_ellipsis_qname_identifier());
        named_expression_list_t ellipsis_args;
        ellipsis_args.emplace_back(annotated_entity_identifier{ eid });
        pure_call_t ellipsis_call{ lex::resource_location{}, std::move(ellipsis_args) };
        functional::match_descriptor md;
        auto ptrn = ellipsis_fnl.find(ctx, ellipsis_call, {}, md);
        if (!ptrn.has_value()) {
            return std::unexpected(std::move(ptrn.error()));
        }
        auto r = ptrn.value()->const_apply(ctx, md);
        if (!r.has_value()) {
            return std::unexpected(std::move(r.error()));
        }
        return r.value();
    }
};

struct parameter_visitor : static_visitor<std::expected<pattern_expression_t, error_storage>>
{
    fn_compiler_context& ctx;
    function_descriptor& fd_;
    explicit parameter_visitor(fn_compiler_context& c, function_descriptor& fd)
        : ctx{ c }, fd_{ fd }
    {}

    inline unit& u() const noexcept { return ctx.u(); }

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("parameter_visitor not implemented expression");
    }

    result_type operator()(annotated_entity_identifier const& ee)
    {
        return ee.value;
    }

    result_type operator()(variable_reference const& var)
    {
        if (var.implicit || (var.name.value.is_relative() && var.name.value.size() == 1)) {
            // check for function parameter
            identifier varid = *var.name.value.begin();
            if (auto it = fd_.varparams().find(varid); it != fd_.varparams().end()) {
                return pattern_variable{ varid };
            }
        }

        functional const* f = ctx.lookup_functional(var.name.value);
        if (f) return annotated_qname_identifier{ f->id(), var.name.location };
        
        if (var.name.value.is_absolute() || !var.implicit || var.name.value.size() != 1) {
            return std::unexpected(make_error<undeclared_identifier_error>(var.name));
        }
        identifier varid = *var.name.value.begin();
        //GLOBAL_LOG_INFO() << u().print(varid);
        fd_.variables().insert(varid);
        return pattern_variable{ varid };
    }

    result_type operator()(bang_parameter_pack_t const& ppack)
    {
        result_type elem = apply_visitor(*this, ppack.type);
        if (!elem.has_value()) return std::unexpected(std::move(elem.error()));

        parameter_pack_element_type_visitor vis{ ctx, ppack.type };
        return apply_visitor(vis, elem.value());

        //functional const** pptypevar = get<functional const*>(&elem.value());
        //if (pptypevar) {
        //    entity_identifier element_type = (*pptypevar)->default_entity();
        //    if (!element_type) {
        //        return std::unexpected(make_error<basic_general_error>(get_start_location(ppack.type),
        //            ("identifier is not a type, see declaration at %1%"_fmt % u().print((*pptypevar)->location())).str()));
        //    }
        //    functional& ellipsis_fnl = u().fregistry().resolve(u().get_ellipsis_qname_identifier());
        //    named_expression_term_list_t ellipsis_args;
        //    ellipsis_args.emplace_back(entity_expression{ element_type, });
        //    pure_call_t ellipsis_call{ lex::resource_location{}, std::move(ellipsis_args) };
        //    auto ptrn = ellipsis_fnl.find(ctx, ellipsis_call);
        //    if (!ptrn.has_value()) {
        //        return std::unexpected(std::move(ptrn.error()));
        //    }
        //    auto r = ptrn.value()->const_apply(ctx);
        //    if (!r.has_value()) {
        //        return std::unexpected(std::move(r.error()));
        //    }
        //    return r.value();
        //}
    }
};
#endif

//std::expected<int, error_storage> fn_compiler_context::build_fieldset(fn_pure const& pure_decl, patern_fieldset_t& fs)
//{
////    THROW_NOT_IMPLEMENTED_ERROR("function_descriptor::build_fieldset");
//    
//    auto& params = pure_decl.parameters;
//    std::vector<function_descriptor::named_field> nfields;
//    std::vector<function_descriptor::positioned_field> pfields;
//    boost::container::small_vector<pattern_expression_t, 1> temp_constraint_patterns;
//
//    boost::container::small_vector<annotated_identifier, 16> aux;
//    aux.reserve(params.size());
//
//    parameter_visitor pvis{ *this };
//    for (auto& param : params) {
//        temp_constraint_patterns.clear();
//        parameter_constraint_list_t const& constraints = param.constraints;
//        for (syntax_expression_t const& expr : param.constraints) {
//            auto res = apply_visitor(pvis, expr);
//            if (!res.has_value()) return std::unexpected(std::move(res.error()));
//            temp_constraint_patterns.emplace_back(res.value());
//        }
//        //bool constraint_is_typename = constraint_entity.is(*this, unit_.get_typename_entity_identifier());
//        //param_constraint_type ct = constraint_is_typename ?
//        //    (parampair.type.is_const ? param_constraint_type::const_constraint : param_constraint_type::type_constaint)
//        //    : param_constraint_type::value_constaint;
//        param_constraint_type ct = param_constraint_type::value_type_constaint; // to do: resolve constraint type
//
//        if (param.name.external_name) {
//            aux.push_back(*param.name.external_name);
//            nfields.emplace_back(
//                *param.name.external_name,
//                param.name.internal_name,
//                std::move(temp_constraint_patterns),
//                ct);
//        } else {
//            pfields.emplace_back(
//                param.name.internal_name,
//                std::move(temp_constraint_patterns),
//                ct);
//        }
//    }
//
//    // check for name duplicates
//    std::stable_sort(aux.begin(), aux.end(), [](auto const& l, auto const& r) {
//        return l.value < r.value;
//        //return l.ename && (!r.ename || l.ename->value < r.ename->value);wa
//    });
//    auto it = std::unique(aux.begin(), aux.end(), [](auto const& l, auto const& r) {
//        return l.value == r.value;
//    });
//    if (it != aux.end()) {
//        return std::unexpected(make_error<basic_general_error>(it->location, "duplicate parameter name"sv));
//    }
//
//    std::sort(nfields.begin(), nfields.end(), [](auto const& l, auto const& r) {
//        return l.ename.value < r.ename.value;
//        //return l.ename && (!r.ename || l.ename->value < r.ename->value);wa
//    });
//
//    fs.set_nfields(std::move(nfields));
//    fs.set_pfields(std::move(pfields));
//
//    return 0;
//}

fn_compiler_context::fn_compiler_context(unit& u, qname ns)
    : unit_{ u }
    , parent_{ nullptr }
    , ns_{ std::move(ns) }
    , expression_store_{ u }
{
    init();
}

fn_compiler_context::fn_compiler_context(fn_compiler_context& parent, qname_view nested)
    : unit_{ parent.unit_ }
    , ns_{ parent.ns() / nested }
    , parent_{ nested.has_prefix(parent.ns()) ? &parent : nullptr }
    , expression_store_{ parent.unit_ }
{
    init();
}

void fn_compiler_context::init()
{
    assert(ns_.is_absolute());
    base_ns_size_ = ns_.parts().size();
    expr_stack_.emplace_back();
    scoped_locals_.emplace_back(); // initial scope
}

fn_compiler_context::~fn_compiler_context()
{
    // move expressions to remove
    //expression_store_.splice_back(root_expressions_);
}

optional<variant<entity_identifier, local_variable const&>> fn_compiler_context::get_bound(identifier name) const noexcept
{
    // then look for bound entities
    for (functional_binding const* binding : boost::adaptors::reverse(bindings_)) {
        if (auto optval = binding->lookup(name); optval) {
            if (entity_identifier const* eid = get<entity_identifier>(optval); eid) return *eid;
            if (local_variable const* lv = get<local_variable>(optval); lv) return *lv;
            // temporary entity is bound
            // it's not propper value to return, so just skip it
            // ?? or return nullopt ??
        }
    }
    for (functional_binding const& binding : boost::adaptors::reverse(scoped_locals_)) {
        if (auto optval = binding.lookup(name); optval) {
            if (entity_identifier const* eid = get<entity_identifier>(optval); eid) return *eid;
            if (local_variable const* lv = get<local_variable>(optval); lv) return *lv;
        }
    }

    return nullopt;
}

compiler_task_tracer::task_guard fn_compiler_context::try_lock_task(compiler_task_id const& tid)
{
    return unit_.task_tracer().try_lock_task(tid, worker_id_);
}

void fn_compiler_context::push_scope()
{
    ns_ = ns_ / unit_.new_identifier();
    scope_offset_ += scoped_locals_.back().size();
    scoped_locals_.emplace_back();
}

void fn_compiler_context::pop_scope()
{
    assert(ns_.parts().size() > base_ns_size_);
    ns_.truncate(ns_.parts().size() - 1);
    scoped_locals_.pop_back(); // to do: call destructor for local variables
    if (!scoped_locals_.empty()) {
        scope_offset_ -= scoped_locals_.back().size();
    }
}

void fn_compiler_context::push_scope_variable(annotated_identifier name, local_variable&& lv, internal_function_entity& fnent)
{
    int64_t index = scope_offset_ + scoped_locals_.back().size();
    fnent.push_variable(lv.varid, index);
    scoped_locals_.back().emplace_back(std::move(name), std::move(lv));
}

void fn_compiler_context::push_chain()
{
    expr_stack_.emplace_back();
}

void fn_compiler_context::pop_chain()
{
    semantic::expression_span exprs = stored_expressions();
    expr_stack_.pop_back();
    append_stored_expressions(std::move(exprs));
}

#if 0
error_storage fn_compiler_context::build_function_descriptor(fn_pure const& pure_decl, function_descriptor& fd)
{
    THROW_NOT_IMPLEMENTED_ERROR("fn_compiler_context::build_function_descriptor");
#if 0
    qname fn_qname = ns() / pure_decl.name();

    //if (!fn_qname.has_prefix(ns())) {
    //    return std::unexpected(make_error<basic_general_error>(pure_decl.location(), "not a nested scope identifier"sv, (qname)pure_decl.name()));
    //}

    functional& fnl = u().fregistry().resolve(fn_qname);
    qname_identifier fn_qname_id_ = fnl.id();
    fd.set_id(fn_qname_id_);
    
    auto& params = pure_decl.parameters;
    std::vector<function_descriptor::named_field> nfields;
    std::vector<function_descriptor::positioned_field> pfields;

    // get function parameter ids

    auto put_var = [&fd](annotated_identifier const& aid) -> error_storage {
        if (auto pair = fd.varparams().insert(aid.value); !pair.second) {
            return make_error<basic_general_error>(aid.location, "duplicate parameter name"sv, aid.value);
        }
        return {};
    };

    boost::container::small_flat_map<identifier, lex::resource_location, 16> aux;
    auto check_name = [&aux](annotated_identifier const& aid) -> error_storage {
        if (auto pair = aux.insert(std::pair{ aid.value, aid.location }); !pair.second) {
            return make_error<general_with_see_location_error>(aid.location, "duplicate parameter name"sv, pair.first->second, aid.value);
        }
        return {};
    };

    for (auto& param : params) {
        auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param.name);
        
        if (param.modifier == parameter_constraint_modifier_t::typename_constraint || param.modifier == parameter_constraint_modifier_t::value_constraint) {
            if (internal_name) {
                if (auto err = put_var(*internal_name); err) { return err; }
            } else if (external_name) {
                if (auto err = put_var(*external_name); err) { return err; }
            }
        }
        if (external_name) {
            if (auto err = check_name(*external_name); err) { return err; }
        }
        if (internal_name) {
            if (auto err = check_name(*internal_name); err) { return err; }
        }
    }

    fn_compiler_context fnctx{ u(), fn_qname };
    parameter_visitor pvis{ fnctx, fd };

    for (auto& param : params) {
        auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param.name);

        function_descriptor::generic_field fld;
        parameter_constraint_set_t const& constraints = param.constraints;
        if (constraints.type_expression) {
            auto res = apply_visitor(pvis, *constraints.type_expression);
            if (!res.has_value()) return std::move(res.error());
            fld.constraint = res.value();
        }
        for (syntax_expression_t const& ce : constraints.concepts) {
            auto res = apply_visitor(pvis, ce);
            if (!res.has_value()) return std::move(res.error());
            fld.concepts.emplace_back(res.value());
        }
        fld.bindings.reserve(constraints.bindings.size());
        std::ranges::copy(constraints.bindings, std::back_inserter(fld.bindings));
        fld.constraint_type = param.modifier;
        if (internal_name) fld.iname = *internal_name;
        //bool constraint_is_typename = constraint_entity.is(*this, unit_.get_typename_entity_identifier());
        //param_constraint_type ct = constraint_is_typename ?
        //    (parampair.type.is_const ? param_constraint_type::const_constraint : param_constraint_type::type_constaint)
        //    : param_constraint_type::value_constaint;

        if (external_name) {
            nfields.emplace_back(*external_name, std::move(fld));
        } else {
            pfields.emplace_back(std::move(fld));
        }
    }

    std::sort(nfields.begin(), nfields.end(), [](auto const& l, auto const& r) {
        return l.ename.value < r.ename.value;
        //return l.ename && (!r.ename || l.ename->value < r.ename->value);wa
    });

    fd.set_nfields(std::move(nfields));
    fd.set_pfields(std::move(pfields));


    if (pure_decl.result) {
        auto rtres = apply_visitor(pvis, *pure_decl.result);
        if (!rtres.has_value()) return std::move(rtres.error());
        fd.set_result_type(rtres.value());
    }
#endif
    return {};

    //boost::container::small_vector<annotated_identifier, 16> aux;
    //aux.reserve(params.size());

    //for (auto& parampair : params) {
    //    entity_identifier constraint_eid = apply_visitor(tqvis, parampair.type.value);
    //    entity const& constraint_entity = unit_.eregistry_get(constraint_eid);
    //    bool constraint_is_typename = constraint_entity.is(*this, unit_.get_typename_entity_identifier());
    //    param_constraint_type ct = constraint_is_typename ? 
    //          (parampair.type.is_const ? param_constraint_type::const_constraint : param_constraint_type::type_constaint)
    //        : param_constraint_type::value_constaint;

    //    if (parampair.name.internal_name) aux.push_back(*parampair.name.internal_name);

    //    if (parampair.name.external_name) {
    //        aux.push_back(*parampair.name.external_name);
    //        nfields.emplace_back(
    //            *parampair.name.external_name,
    //            parampair.name.internal_name,
    //            constraint_eid,
    //            ct);
    //    } else {
    //        pfields.emplace_back(
    //            parampair.name.internal_name,
    //            constraint_eid,
    //            ct);
    //    }
    //}

    //// check for name duplicates
    //std::stable_sort(aux.begin(), aux.end(), [](auto const& l, auto const& r) {
    //    return l.value < r.value;
    //    //return l.ename && (!r.ename || l.ename->value < r.ename->value);wa
    //});
    //auto it = std::unique(aux.begin(), aux.end(), [](auto const& l, auto const& r) {
    //    return l.value == r.value;
    //});
    //if (it != aux.end()) {
    //    return std::unexpected(make_error<basic_general_error>(it->location, "duplicate parameter name"sv));
    //}

    //std::sort(nfields.begin(), nfields.end(), [](auto const& l, auto const& r) {
    //    return l.ename.value < r.ename.value;
    //    //return l.ename && (!r.ename || l.ename->value < r.ename->value);wa
    //});

    //res.set_nfields(std::move(nfields));
    //res.set_pfields(std::move(pfields));
    //return res;
    ////THROW_NOT_IMPLEMENTED_ERROR("function_descriptor::function_descriptor");
}
#endif

functional const* fn_compiler_context::lookup_functional(qname_view name) const
{
    if (name.is_absolute()) {
        return unit_.fregistry_find(name);
    }
    qname checkns = ns_;
    size_t sz = checkns.parts().size();
    for (;;) {
        checkns.append(name);
        functional* f = unit_.fregistry_find(checkns);
        if (f || !sz) return f;
        --sz;
        checkns.truncate(sz);
    }
}

std::expected<qname_identifier, error_storage> fn_compiler_context::lookup_qname(annotated_qname const& name) const
{
    functional const* pfn = lookup_functional(name.value);
    if (pfn) return pfn->id();
    return std::unexpected(make_error<undeclared_identifier_error>(name));
}

variant<entity_identifier, local_variable const&> fn_compiler_context::lookup_entity(annotated_qname const& name)
{
    using result_t = variant<entity_identifier, local_variable>;
    if (name.value.is_relative() && name.value.size() == 1) {
        identifier varid = *name.value.begin();
        auto optbv = get_bound(varid);
        if (optbv) return std::move(*optbv);
    }

    functional const* pfn = lookup_functional(name.value);
    if (pfn) return pfn->default_entity(*this);
    return entity_identifier{}; // undeclared
}

std::expected<functional::match, error_storage> fn_compiler_context::find(builtin_qnid qnid, pure_call_t const& call, semantic::expression_list_t& el, expected_result_t const& expected_result)
{
    functional const& fn = u().fget(qnid);
    return fn.find(*this, call, el, expected_result);
}

std::expected<functional::match, error_storage> fn_compiler_context::find(qname_identifier qnid, pure_call_t const& call, semantic::expression_list_t& el, expected_result_t const& expected_result)
{
    functional const& fn = u().fregistry_resolve(qnid);
    return fn.find(*this, call, el, expected_result);
}

/*
local_variable & fn_compiler_context::new_variable(annotated_identifier name, entity_identifier t)
{
    lex::resource_location const* ploc;
    functional_binding::value_type const* pv = scoped_locals_.back().lookup(name.value, &ploc);
    if (pv) {
        throw identifier_redefinition_error(name, *ploc);
    }
    return get<local_variable>(scoped_locals_.back().emplace_back(name,
        local_variable{ .type = std::move(t), .varid = unit_.new_variable_identifier(), .is_weak = false }));
}
*/

void fn_compiler_context::new_constant(annotated_identifier name, entity_identifier eid)
{
    lex::resource_location const* ploc;
    functional_binding::value_type const* pv = scoped_locals_.back().lookup(name.value, &ploc);
    if (pv) {
        throw identifier_redefinition_error(name, *ploc);
    }
    scoped_locals_.back().emplace_back(name, eid);
}

#if 0
variable_entity& fn_compiler_context::create_captured_variable_chain(variable_entity& v)
{
    if (!parent_) {
        THROW_INTERNAL_ERROR("can't find fn context for variable: '%1%', current context ns: '%2%'"_fmt %
            u().print(v.name) % u().print(ns())
        );
    }
    qname_view name_qv = u().fregistry().resolve(v.name).name();
    qname_view vardefscope = name_qv.parent();
    if (vardefscope.has_prefix(parent_->base_ns())) {
        return new_captured_variable(name_qv.back(), v.get_type(), v);
    } else {
        variable_entity& parentvar = parent_->create_captured_variable_chain(v);
        return new_captured_variable(name_qv.back(), v.get_type(), parentvar);
    }
}
#endif

std::pair<entity_identifier, bool> fn_compiler_context::finish_frame()
{
    if (result_value_or_type) {
        // result type is already set
        return { result_value_or_type, is_const_result };
    }

    if (return_statements_.empty()) {
        // no return statements, so we can use void as result type
        return { unit_.get(builtin_eid::void_), true };
    }

    boost::container::small_flat_set<entity_identifier, 4> const_values;
    boost::container::small_flat_set<entity_identifier, 4> types;
    for (semantic::return_statement * rts : return_statements_) {
        if (rts->is_const_result) {
            const_values.insert(rts->value_or_type);
            if (!types.empty() || const_values.size() > 1) {
                types.insert(get_entity(u(), rts->value_or_type).get_type());
            }
        } else {
            types.insert(rts->value_or_type);
        }
    }
    if (types.empty() && const_values.size() == 1) {
        return { *const_values.begin(), true };
    }
    THROW_NOT_IMPLEMENTED_ERROR("fn_compiler_context: finish_frame with multiple return values not implemented yet");
    //if (accum_result) {
    //    result = accum_result;
    //} else { // no explicit return
    //    result = u().get(builtin_eid::void_);
    //    u().push_back_expression(expression_store_, expressions(), semantic::return_statement{});
    //}
}

//void fn_compiler_context::accumulate_result_type(entity_identifier t)
//{
//    if (!accum_result) {
//        accum_result = std::move(t);
//    } else {
//        THROW_NOT_IMPLEMENTED_ERROR("compiler context: accumulate_result_type");
//#if 0
//        accum_result = make_union_type(*accum_result, &t);
//#endif
//    }
//}

#if 0
entity_identifier fn_compiler_context::compute_result_type()
{
    if (result) { return result; }
    else if (!accum_result) {
        accum_result = unit_.get(builtin_eid::void_);
    }
    return accum_result;
}

fn_compiler_context::expressions_state_type::expressions_state_type(fn_compiler_context& ctx) noexcept
    : pctx_{ &ctx }
    , cursize_{ ctx.expressions().size() }
    , stack_size_{ ctx.expr_stack_.size() }
    , cur_type_{ ctx.context_type }
{}

fn_compiler_context::expressions_state_type::expressions_state_type(expressions_state_type&& rhs) noexcept
    : pctx_{ rhs.pctx_ }
    , cursize_{ rhs.cursize_ }
    , stack_size_{ rhs.stack_size_ }
    , cur_type_{ rhs.cur_type_ }
{
    rhs.pctx_ = nullptr;
}

fn_compiler_context::expressions_state_type::~expressions_state_type()
{
    if (pctx_) {
        restore();
    }
}

void fn_compiler_context::expressions_state_type::detach() noexcept
{
    pctx_ = nullptr;
}

void fn_compiler_context::expressions_state_type::restore_and_detach()
{
    restore();
    detach();
}

void fn_compiler_context::expressions_state_type::restore()
{
    pctx_->expr_stack_.resize(stack_size_);
    auto& elist = pctx_->expressions();
    while (elist.size() > cursize_) {
        pctx_->u().release(elist.pop_back());
    }
    //pctx_->expressions().resize(cursize_);
    pctx_->context_type = cur_type_;
}
#endif

semantic::expression_span fn_compiler_context::store_semantic_expressions(semantic::managed_expression_list&& el)
{
    semantic::expression_span res = el;
    expression_store_.splice_back(el);
    return res;
}

void fn_compiler_context::append_expression(semantic::expression&& e)
{
    unit_.push_back_expression(expression_store_, expressions(), std::move(e));
}

void fn_compiler_context::append_expressions(semantic::expression_span sp)
{
    expressions() = expression_store_.concat(expressions(), std::move(sp));
}

void fn_compiler_context::append_expressions(semantic::expression_list_t& el, semantic::expression_span sp)
{
    expression_store().splice_back(el, sp);
    append_expressions(sp);
}

void fn_compiler_context::append_stored_expressions(semantic::expression_span sp)
{
    stored_expressions() = expression_store_.concat(stored_expressions(), std::move(sp));
}

void fn_compiler_context::append_stored_expressions(semantic::expression_list_t& el, semantic::expression_span sp)
{
    expression_store().splice_back(el, sp);
    append_stored_expressions(sp);
}

void fn_compiler_context::append_return(semantic::expression_span return_expressions, entity_identifier value_or_type, bool is_const_result)
{
    // return_expressions should contain a cast to return value_or type, if result_value_or_type is defined
    append_expression(semantic::return_statement{
        .result = return_expressions,
        .value_or_type = value_or_type,
        .is_const_result = is_const_result
    });
    semantic::return_statement * pretst = &get<semantic::return_statement>(expressions().back());
    return_statements_.emplace_back(pretst);
}

//void fn_compiler_context::adopt_and_append(semantic::expression_list_t& el, syntax_expression_result_t& er)
//{
//    append_stored_expressions(el, er.stored_expressions);
//    append_expressions(el, er.expressions);
//}

fn_compiler_context_scope::fn_compiler_context_scope(fn_compiler_context& ctx)
    : ctx_{ ctx }
{
    ctx_.push_scope();
    ctx_.push_binding(bound_temporaries_);
}

fn_compiler_context_scope::~fn_compiler_context_scope()
{
    ctx_.pop_binding(&bound_temporaries_);
    ctx_.pop_scope();
}

local_variable& fn_compiler_context_scope::new_temporary(identifier name, entity_identifier type)
{
    local_variable& lv = get<local_variable>(bound_temporaries_.emplace_back(
        annotated_identifier(name),
        local_variable{ .type = std::move(type), .varid = ctx_.u().new_variable_identifier(), .is_weak = false }));
    return lv;
}

}
