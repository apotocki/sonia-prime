//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "fn_compiler_context.hpp"
#include "preliminary_type_visitor.hpp"

namespace sonia::lang::bang {

error_storage fn_compiler_context::build_fieldset(fn_pure_decl const& pure_decl, fieldset & fs)
{
    auto& params = pure_decl.parameters;
    preliminary_type_visitor2 tqvis{ *this };

    std::vector<fieldset::named_field> nfields;
    std::vector<fieldset::positioned_field> pfields;

    boost::container::small_vector<annotated_identifier, 16> aux;
    aux.reserve(params.size());

    for (auto& parampair : params) {
        entity_identifier constraint_eid = apply_visitor(tqvis, parampair.type.value);
        entity const& constraint_entity = unit_.eregistry().get(constraint_eid);
        bool constraint_is_typename = constraint_entity.is(*this, unit_.get_typename_entity_identifier());
        param_constraint_type ct = constraint_is_typename ?
            (parampair.type.is_const ? param_constraint_type::const_constraint : param_constraint_type::type_constaint)
            : param_constraint_type::value_constaint;

        if (parampair.name.internal_name) aux.push_back(*parampair.name.internal_name);

        if (parampair.name.external_name) {
            aux.push_back(*parampair.name.external_name);
            nfields.emplace_back(
                *parampair.name.external_name,
                parampair.name.internal_name,
                constraint_eid,
                ct);
        } else {
            pfields.emplace_back(
                parampair.name.internal_name,
                constraint_eid,
                ct);
        }
    }

    // check for name duplicates
    std::stable_sort(aux.begin(), aux.end(), [](auto const& l, auto const& r) {
        return l.value < r.value;
        //return l.ename && (!r.ename || l.ename->value < r.ename->value);wa
    });
    auto it = std::unique(aux.begin(), aux.end(), [](auto const& l, auto const& r) {
        return l.value == r.value;
    });
    if (it != aux.end()) {
        return make_error<basic_general_error>(it->location, "duplicate parameter name"sv);
    }

    std::sort(nfields.begin(), nfields.end(), [](auto const& l, auto const& r) {
        return l.ename.value < r.ename.value;
        //return l.ename && (!r.ename || l.ename->value < r.ename->value);wa
    });

    fs.set_nfields(std::move(nfields));
    fs.set_pfields(std::move(pfields));

    return {};
}

std::expected<function_descriptor, error_storage> fn_compiler_context::build_function_descriptor(fn_pure_decl& pure_decl)
{
    qname fn_qname = ns() / pure_decl.name();

    //if (!fn_qname.has_prefix(ns())) {
    //    return std::unexpected(make_error<basic_general_error>(pure_decl.location(), "not a nested scope identifier"sv, (qname)pure_decl.name()));
    //}

    function_descriptor res{ unit_.qnregistry().resolve(fn_qname) };

    if (pure_decl.result) {
        preliminary_type_visitor2 tqvis{ *this };
        res.set_result_type(apply_visitor(tqvis, *pure_decl.result));
    } else {
        res.set_result_type(unit_.get_void_entity_identifier());
    }

    if (auto err = build_fieldset(pure_decl, res); err) return std::unexpected(std::move(err));

    return res;

    //auto& params = pure_decl.parameters;
    //std::vector<function_descriptor::named_field> nfields;
    //std::vector<function_descriptor::positioned_field> pfields;

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

functional const* fn_compiler_context::lookup_functional(qname_identifier name) const
{
    if (name.is_absolute()) {
        return unit_.fregistry().find(name);
    }
    qname checkns = ns_;
    qname_view name_qn = unit_.qnregistry().resolve(name);
    for (size_t sz = checkns.parts().size();;) {
        checkns.append(name_qn);
        functional const* f = unit_.fregistry().find(unit_.qnregistry().resolve(checkns));
        if (f || !sz) return f;
        --sz;
        checkns.truncate(sz);
    }
    return nullptr;
}

}
