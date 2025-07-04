//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "value_type_match_visitor.hpp"
#include "signature_matcher.hpp"
#include "match_utility.ipp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/expression_visitor.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"


namespace sonia::lang::bang {

value_type_match_visitor::value_type_match_visitor(fn_compiler_context& caller, fn_compiler_context& callee, semantic::expression_list_t& ael, syntax_expression_t const& e, functional_binding& b) noexcept
    : caller_ctx{ caller }, callee_ctx{ callee }, expressions{ ael }
    , expr{ e }, binding{ b }
{}

value_type_match_visitor::result_type value_type_match_visitor::operator()(annotated_qname_identifier const& aqi) const
{
    functional const& fnl = callee_ctx.u().fregistry_resolve(aqi.value);
    entity_identifier eid = fnl.default_entity(callee_ctx);
    if (eid) {
        return match_type(eid, aqi.location);
    }

    return std::unexpected(make_error<basic_general_error>(aqi.location, "not a variable or constant"sv, expr));
}

value_type_match_visitor::result_type value_type_match_visitor::match_type(entity_identifier const& eid, lex::resource_location eidloc) const
{
    auto res = apply_visitor(base_expression_visitor{ caller_ctx, expressions, { eid, eidloc } }, expr);
    if (!res) return std::unexpected(std::move(res.error()));
    if (res->second) { --weight; }
    return std::move(res->first);
}

value_type_match_visitor::result_type value_type_match_visitor::operator()(variable_reference const& var) const
{
    auto optent = callee_ctx.lookup_entity(var.name);

    return apply_visitor(make_functional_visitor<result_type>([this, &var](auto eid_or_var) -> result_type
    {
        if constexpr (std::is_same_v<std::decay_t<decltype(eid_or_var)>, entity_identifier>) {
            if (!eid_or_var) { // if not defined
                if (!var.implicit) return std::unexpected(make_error<undeclared_identifier_error>(var.name));
                // bind variable
                return sonia::lang::bang::match_type(caller_ctx, expressions, expr, entity_identifier{}, var.name.location, [this, var](entity_identifier matched_type, bool casted) -> error_storage {
                    if (casted) { --weight; }
                    identifier varid = *var.name.value.begin();
                    binding.emplace_back(annotated_identifier{ varid, var.name.location }, matched_type);
                    return {};
                });
            }
            return match_type(eid_or_var, var.name.location);
        } else { // entity_ptr, that is variable_entity
            return std::unexpected(make_error<basic_general_error>(var.name.location, "argument mismatch"sv, var));
        }
    }), optent);
}

value_type_match_visitor::result_type value_type_match_visitor::operator()(function_call_t const& fc) const
{
    unit& u = caller_ctx.u();

    ct_expression_visitor sv{ callee_ctx, expressions, expected_result_t{ .type = u.get(builtin_eid::qname), .modifier = value_modifier_t::constexpr_value } };
    auto qn_ent_id = apply_visitor(sv, fc.fn_object);
    if (!qn_ent_id) return std::unexpected(std::move(qn_ent_id.error()));
    qname_identifier_entity qname_ent = static_cast<qname_identifier_entity const&>(get_entity(u, qn_ent_id->value));

    // check if can evaluate signature_pattern as a const expression
    
    auto match = callee_ctx.find(qname_ent.value(), fc, expressions);
    if (match) {
        if (auto gresult = match->apply(callee_ctx); gresult) {
            if (auto result = ct_expression_visitor{ callee_ctx, expressions }.handle(std::pair{std::move(*gresult), false}); result) {
                return match_type(result->value, fc.location);
            }
        }
    }

    // can't evaluate signature_pattern as a function, consider as a pattern
    return sonia::lang::bang::match_type(caller_ctx, expressions, expr, entity_identifier{}, fc.location, [this, &qname_ent, &fc](entity_identifier matched_type, bool casted) -> error_storage {
        if (casted) { --weight; }
        entity const& type_ent = get_entity(caller_ctx.u(), matched_type);
        entity_signature const* psig = type_ent.signature();
        if (!psig) {
            return make_error<basic_general_error>(fc.location, "argument mismatch"sv, expr);
        }
        if (qname_ent.value() != psig->name) {
            return make_error<basic_general_error>(fc.location, "argument mismatch"sv, expr);
        }

        size_t arg_index = 0;
        for (auto const& arg : fc.args) {
            annotated_identifier const* argname = arg.name();
            field_descriptor const* pfd = argname ? psig->find_field(argname->value) : psig->find_field(arg_index++);
            if (!pfd) {
                return make_error<basic_general_error>(fc.location, "argument pattern mismatch"sv, expr);
            }
            BOOST_ASSERT(pfd->is_const());
            signature_matcher_visitor smvis{ callee_ctx, binding, pfd->entity_id() };
            if (auto err = apply_visitor(smvis, arg.value()); err) return std::move(err);
        }

        return {};
    });


    //auto res = apply_visitor(expression_visitor{ caller_ctx }, expr);
    //if (!res) return std::unexpected(std::move(res.error()));

    //entity const& type_ent = u.eregistry_get(caller_ctx.context_type);
    //entity_signature const* psig = type_ent.signature();
    //if (!psig) {
    //    return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, expr));
    //}

    //if (qname_ent.value() != psig->name) {
    //    return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, expr));
    //}

    //size_t arg_index = 0;
    //for (auto const& arg : fc.args()) {
    //    annotated_identifier const* argname = arg.name();
    //    field_descriptor const* pfd = argname ? psig->find_field(argname->value) : psig->find_field(arg_index++);
    //    if (!pfd) {
    //        return std::unexpected(make_error<basic_general_error>(fc.location(), "argument pattern mismatch"sv, expr));
    //    }
    //    BOOST_ASSERT(pfd->is_const());
    //    signature_matcher_visitor smvis{ callee_ctx, binding, pfd->entity_id() };
    //    if (auto err = apply_visitor(smvis, arg.value()); err) return std::unexpected(std::move(err));
    //}

    //return type_ent.id();

    //signature_matcher_visitor sm{ fc, binding };
    //auto res = sm.try_match(ctx, expr);
    //if (!res) return std::unexpected(std::move(res.error()));
    //weight -= *res ? 1 : 0;
    //return ctx.context_type;

    //// to do: check if can apply fc
    //// if no, consider as a pattern:

    //// is it really a pattern?
    //variable_reference const* pfn_name = get<variable_reference>(&fc.fn_object);
    //if (!pfn_name || pfn_name->implicit)
    //    return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, expr));
    //functional const* pfn_fl = ctx.lookup_functional(pfn_name->name.value);
    //if (!pfn_fl) return std::unexpected(make_error<undeclared_identifier_error>(pfn_name->name));

    //expression_visitor evis{ ctx };
    //auto res = apply_visitor(evis, expr);
    //if (!res) return std::unexpected(std::move(res.error()));
    //
    //entity const& type_ent = ctx.u().eregistry_get(ctx.context_type);
    //entity_signature const* psig = type_ent.signature();
    //if (!psig) {
    //    return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, expr));
    //}
    //if (pfn_fl->id() != psig->name()) {
    //    return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, expr));
    //}

    //size_t arg_index = 0;
    //for (auto const& arg: fc.args()) {
    //    annotated_identifier const* argname = arg.name();
    //    field_descriptor const* pfd = argname ? psig->find_field(argname->value) : psig->find_field(arg_index++);
    //    if (!pfd) {
    //        return std::unexpected(make_error<basic_general_error>(fc.location(), "argument pattern mismatch"sv, expr));
    //    }
    //}
    ////fc.fn_object // variable_reference
    //THROW_NOT_IMPLEMENTED_ERROR("value_type_match_visitor(function_call_t) not implemented expression");
}

value_type_match_visitor::result_type value_type_match_visitor::operator()(placeholder const& ph) const
{
    weight = -1; // template match
    return match_type(entity_identifier{}, ph.location);

    //auto res = apply_visitor(base_expression_visitor{ caller_ctx }, expr);
    //if (!res) return std::unexpected(std::move(res.error()));
    //if (entity_identifier const* peid = get<entity_identifier>(&res->first); peid && *peid == caller_ctx.u().get(builtin_eid::void_)) {
    //    return std::unexpected(make_error<type_mismatch_error>(get_start_location(expr), *peid, caller_ctx.u().get(builtin_eid::any), ph.location));
    //}
    //return res->first;

    //return apply_visitor(make_functional_visitor<result_type>([this, &ph /*, casted = res->second*/](auto& v) -> result_type {
    //    weight = -1; // template match
    //    if constexpr (std::is_same_v<semantic::managed_expression_list, std::decay_t<decltype(v)>>) {
    //        if (caller_ctx.context_type == caller_ctx.u().get(builtin_eid::void_)) {
    //            return entity_identifier{}; // ignore void
    //        }
    //        return std::move(v);
    //    } else {
    //        if (v == caller_ctx.u().get(builtin_eid::void_)) {
    //            // expected value of any type instead of typename void
    //            return std::unexpected(make_error<type_mismatch_error>(get_start_location(expr), v, caller_ctx.u().get(builtin_eid::any), ph.location));
    //        }
    //        return v;
    //        //caller_ctx.append_expression(semantic::push_value{ v });
    //        //entity const& e = caller_ctx.u().eregistry_get(v);
    //        //caller_ctx.context_type = e.get_type();
    //        //return caller_ctx.context_type;
    //    }
    //    return caller_ctx.context_type;
    //}), res->first);
}

}
