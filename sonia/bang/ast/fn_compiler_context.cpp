//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "fn_compiler_context.hpp"
#include "preliminary_type_visitor.hpp"

#include <boost/container/flat_map.hpp>

namespace sonia::lang::bang {

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
            return std::unexpected(make_error<basic_general_error>(get_start_location(element_type_expression_),
                ("identifier is not a type, see declaration at %1%"_fmt % u().print(fnl.location())).str()));
        }
        return this->operator()(element_type);
    }

    result_type operator()(entity_identifier eid)
    {
        functional& ellipsis_fnl = u().fregistry().resolve(u().get_ellipsis_qname_identifier());
        named_expression_term_list_t ellipsis_args;
        ellipsis_args.emplace_back(entity_expression{ eid });
        pure_call_t ellipsis_call{ lex::resource_location{}, std::move(ellipsis_args) };
        functional::match_descriptor md;
        auto ptrn = ellipsis_fnl.find(ctx, ellipsis_call, md);
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

    result_type operator()(entity_expression const& ee)
    {
        return ee.id;
    }

    result_type operator()(variable_identifier const& var)
    {
        if (var.implicit || (var.name.value.is_relative() && var.name.value.size() == 1)) {
            // check for function parameter
            identifier varid = *var.name.value.begin();
            if (auto it = fd_.varparams().find(varid); it != fd_.varparams().end()) {
                return pattern_variable{ varid };
            }
            if (auto it = fd_.variables().find(varid); it != fd_.variables().end()) {
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

//std::expected<int, error_storage> fn_compiler_context::build_fieldset(fn_pure_t const& pure_decl, patern_fieldset_t& fs)
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

error_storage fn_compiler_context::build_function_descriptor(fn_pure_t const& pure_decl, function_descriptor& fd)
{
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
        if (param.modifier == parameter_constraint_modifier_t::typename_constraint || param.modifier == parameter_constraint_modifier_t::value_constraint) {
            if (param.name.internal_name) {
                if (auto err = put_var(*param.name.internal_name); err) { return err; }
            } else if (param.name.external_name) {
                if (auto err = put_var(*param.name.external_name); err) { return err; }
            }
        }
        if (param.name.external_name) {
            if (auto err = check_name(*param.name.external_name); err) { return err; }
        }
        if (param.name.internal_name) {
            if (auto err = check_name(*param.name.internal_name); err) { return err; }
        }
    }

    fn_compiler_context fnctx{ u(), fn_qname };
    parameter_visitor pvis{ fnctx, fd };

    for (auto& param : params) {
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
        fld.iname = param.name.internal_name;
        //bool constraint_is_typename = constraint_entity.is(*this, unit_.get_typename_entity_identifier());
        //param_constraint_type ct = constraint_is_typename ?
        //    (parampair.type.is_const ? param_constraint_type::const_constraint : param_constraint_type::type_constaint)
        //    : param_constraint_type::value_constaint;

        if (param.name.external_name) {
            nfields.emplace_back(*param.name.external_name, std::move(fld));
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

    return {};

    //boost::container::small_vector<annotated_identifier, 16> aux;
    //aux.reserve(params.size());

    //for (auto& parampair : params) {
    //    entity_identifier constraint_eid = apply_visitor(tqvis, parampair.type.value);
    //    entity const& constraint_entity = unit_.eregistry().get(constraint_eid);
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

functional const* fn_compiler_context::lookup_functional(qname_view name) const
{
    if (name.is_absolute()) {
        return unit_.fregistry().find(name);
    }
    qname checkns = ns_;
    size_t sz = checkns.parts().size();
    for (;;) {
        checkns.append(name);
        functional* f = unit_.fregistry().find(checkns);
        if (f || !sz) return f;
        --sz;
        checkns.truncate(sz);
    }
}

variable_entity& fn_compiler_context::new_variable(annotated_identifier name, entity_identifier t, variable_entity::kind k)
{
    qname var_qname = ns() / name.value;
    functional& fnl = unit_.fregistry().resolve(var_qname);
    if (fnl.default_entity()) {
        throw exception(unit_.print(identifier_redefinition_error{ annotated_qname_identifier{fnl.id(), name.location}, fnl.location() }));
    }
    auto ve = sonia::make_shared<variable_entity>(std::move(t), fnl.id(), k);
    entity& ent = unit_.eregistry().find_or_create(*ve, [&ve]() { return std::move(ve); });
    fnl.set_default_entity(ent.id());
    fnl.set_location(name.location);
    auto& vent = static_cast<variable_entity&>(ent);
    vent.set_index(allocate_local_variable_index());
    return vent;
    //THROW_NOT_IMPLEMENTED_ERROR("fn_compiler_context new_variable");
}

}
