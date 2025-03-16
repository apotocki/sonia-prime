//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_get_pattern.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/ast/expression_visitor.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"
//#include "sonia/bang/entities/functions/match_utility.ipp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> tuple_get_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    unit& u = ctx.u();
    identifier slfid = u.get(builtin_id::self);
    identifier propid = u.get(builtin_id::property);
    
    entity const* pte = nullptr;
    entity const* ppname = nullptr;
    shared_ptr<tuple_get_match_descriptor> pmd;
    auto estate = ctx.expressions_state();

    for (auto const& arg : call.args()) {
        annotated_identifier const* pargname = arg.name();
        auto const& argexpr = arg.value();
        if (!pargname) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "argument mismatch"sv, argexpr));
        }
        if (pargname->value == slfid) {
            auto res = apply_visitor(base_expression_visitor{ ctx }, argexpr);
            if (!res) return std::unexpected(std::move(res.error()));
            
            auto err = apply_visitor(make_functional_visitor<error_storage>([&ctx, &argexpr, &pte, &pmd, pargname](auto & v) -> error_storage {
                entity_identifier argtype;
                if constexpr (std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
                    if (v == ctx.u().get(builtin_eid::void_)) {
                        return make_error<type_mismatch_error>(get_start_location(argexpr), v, "not void"sv);
                    }
                } else {
                    if (ctx.context_type == ctx.u().get(builtin_eid::void_)) return {}; // skip
                    argtype = ctx.context_type;
                }
                
                if (!pte) {
                    if constexpr (std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
                        entity const& arg_entity = ctx.u().eregistry_get(v);
                        if (auto psig = arg_entity.signature(); psig && psig->name == ctx.u().get(builtin_qnid::tuple)) {
                            // argument is typename tuple
                            pmd = make_shared<tuple_get_match_descriptor>(ctx.u());
                            auto& mr = pmd->get_match_result(pargname->value);
                            mr.append_result(v);
                            return {};
                        } else {
                            argtype = arg_entity.get_type();
                        }
                    }

                    entity const& some_entity = ctx.u().eregistry_get(argtype);
                    if (auto psig = some_entity.signature(); psig && psig->name == ctx.u().get(builtin_qnid::tuple)) {
                        pte = &some_entity;
                        pmd = make_shared<tuple_get_match_descriptor>(ctx.u());
                        auto& mr = pmd->get_match_result(pargname->value);
                        if constexpr (std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
                            mr.append_result(argtype);
                        } else {
                            mr.append_result(argtype, v.end(), v);
                            pmd->call_expressions.splice_back(v);
                        }
                        return {};
                    }
                }
                return make_error<basic_general_error>(pargname->location, "argument mismatch"sv, pargname->value);
            }), res->first);
            if (err) return std::unexpected(std::move(err));
        } else if (pargname->value == propid && !ppname) {
            ct_expression_visitor evis{ ctx };
            auto res = apply_visitor(evis, arg.value());
            if (!res) return std::unexpected(std::move(res.error()));
            ppname = &u.eregistry_get(*res);
            /*
            entity const& some_entity = ctx.u().eregistry_get(*res);
            ppname = dynamic_cast<identifier_entity const*>(&some_entity);
            if (ppname) continue;
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch, expected an identifier"sv, pargname->value));
            */
        } else {
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch"sv, pargname->value));
        }
    }

    if (!pte) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "unmatched parameter: `self`"sv));
    } else if (!ppname) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "unmatched parameter: `property`"sv));
    }   
    return check_match(std::move(pmd), call, *pte, *ppname);
}

std::expected<functional_match_descriptor_ptr, error_storage> tuple_get_pattern::check_match(shared_ptr<tuple_get_match_descriptor> pmd, pure_call_t const& call, entity const& te, entity const& pname) const
{
    entity_signature const* pes = te.signature();
    BOOST_ASSERT(pes);
    size_t index;
    field_descriptor const* fd;
    if (identifier_entity const* pie = dynamic_cast<identifier_entity const*>(&pname); pie) {
        fd = pes->find_field(pie->value(), &index);
    } else if (integer_literal_entity const* pile = dynamic_cast<integer_literal_entity const*>(&pname); pile) {
        fd = pes->find_field((size_t)pile->value(), &index);
    } else {
        return std::unexpected(make_error<basic_general_error>(pname.location(), "argument mismatch, expected an identifier or index"sv, pname.id()));
    }
    
    if (!fd) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "no such field"sv, pname.id()));
    }

    // get non const fields count and index
    size_t fields_count = pes->fields().size();

    field_descriptor const* b_fd = &pes->fields().front(), *e_fd = b_fd + pes->fields().size();
    for (; b_fd != fd; ++b_fd) {
        if (b_fd->is_const()) { --index; --fields_count; }
    }
    for (; b_fd != e_fd; ++b_fd) {
        if (b_fd->is_const()) { --fields_count; }
    }

    pmd->property_index = index;
    pmd->fields_count = fields_count;
    
    BOOST_ASSERT(pes->field_count() > 1 || !index);
    
    pmd->result = *fd;
    return pmd;
}

std::expected<tuple_get_pattern::application_result_t, error_storage> tuple_get_pattern::generic_apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& tmd = static_cast<tuple_get_match_descriptor&>(md);

    if (tmd.result.is_const()) {
        return tmd.result.entity_id();
    }

    // push call expressions in the right order
    semantic::managed_expression_list exprs{ u };

    // only one named argument is expected
    tmd.for_each_named_match([&exprs, &tmd](identifier name, parameter_match_result const& mr) {
        for (auto const& [_, optspan] : mr.results) {
            BOOST_ASSERT(optspan);
            exprs.splice_back(tmd.call_expressions, *optspan);
        }
    });

    BOOST_ASSERT(!md.call_expressions); // all arguments were transfered

    if (tmd.fields_count > 1) {
        u.push_back_expression(exprs, semantic::push_value{ tmd.property_index });
        u.push_back_expression(exprs, semantic::invoke_function(u.get(builtin_eid::array_at)));
    }

    ctx.context_type = md.result.entity_id();
    return std::move(exprs);
}

std::expected<entity_identifier, error_storage> tuple_get_pattern::const_apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    auto res = generic_apply(ctx, md);
    if (!res) return std::unexpected(std::move(res.error()));
    using result_t = std::expected<entity_identifier, error_storage>;
    return apply_visitor(make_functional_visitor<result_t>([&ctx, &md](auto && eid_or_el) -> result_t {
        if constexpr (std::is_same_v<std::decay_t<decltype(eid_or_el)>, entity_identifier>) {
            return eid_or_el;
        } else {
            return std::unexpected(make_shared<basic_general_error>(md.location, "can't evaluate as a const expression"sv));
        }
    }), *res);
}

}
