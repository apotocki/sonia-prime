//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "value_type_match_visitor.hpp"
#include "signature_matcher.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/expression_visitor.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

value_type_match_visitor::value_type_match_visitor(fn_compiler_context& caller, fn_compiler_context& callee, syntax_expression_t const& e, functional_binding& b)
    : caller_ctx{ caller }, callee_ctx{ callee }, expr{ e }, binding{ b }
{}

value_type_match_visitor::result_type value_type_match_visitor::operator()(annotated_qname_identifier const& aqi) const
{
    functional const& fnl = callee_ctx.u().fregistry().resolve(aqi.value);
    entity_identifier eid = fnl.default_entity(callee_ctx);
    if (eid) {
        return this->operator()(eid, aqi.location);
    }

    return std::unexpected(make_error<basic_general_error>(aqi.location, "not a variable or constant"sv, expr));
}

value_type_match_visitor::result_type value_type_match_visitor::operator()(entity_identifier const& eid, lex::resource_location eidloc) const
{
    entity const& param_entity = callee_ctx.u().eregistry_get(eid);
#if 0
    if (pack_entity const* pent = dynamic_cast<pack_entity const*>(&param_entity); pent) {
        //syntax_expression_t const& expr = expr_it.next_expression();
        expression_visitor evis{ caller_ctx, { pent->element_type(), get_start_location(expr) } };
        auto res = apply_visitor(evis, expr);
        if (!res) return std::unexpected(std::move(res.error()));
    } else {
#endif
        BOOST_ASSERT(eid);
        expression_visitor evis{ caller_ctx, { eid, eidloc } };
        auto res = apply_visitor(evis, expr);
        if (!res) {
            return std::unexpected(std::move(res.error()));
        } else if (res.value()) { --weight; }
    //}
    return caller_ctx.context_type;

    //GLOBAL_LOG_INFO() << ctx.u().print(eid);
    //GLOBAL_LOG_INFO() << ctx.u().print(expr);
    //THROW_NOT_IMPLEMENTED_ERROR("arg_resolve_visitor(entity_identifier) not implemented expression");
}

value_type_match_visitor::result_type value_type_match_visitor::operator()(variable_identifier const& var) const
{
    auto optent = callee_ctx.lookup_entity(var.name);

    return apply_visitor(make_functional_visitor<result_type>([this, &var](auto eid_or_var) -> result_type
        {
            if constexpr (std::is_same_v<std::decay_t<decltype(eid_or_var)>, entity_identifier>) {
                if (!eid_or_var) { // if not defined
                    if (var.implicit) {
                        // bind variable
                        auto res = apply_visitor(expression_visitor{ caller_ctx }, expr);
                        if (!res) return std::unexpected(std::move(res.error()));
                        identifier varid = *var.name.value.begin();
                        binding.emplace_back(annotated_identifier{ varid, var.name.location }, caller_ctx.context_type);
                        return caller_ctx.context_type;
                    }
                    return std::unexpected(make_error<undeclared_identifier_error>(var.name));
                }
                auto res = apply_visitor(expression_visitor{ caller_ctx, annotated_entity_identifier{ eid_or_var, var.name.location } }, expr);
                if (!res) return std::unexpected(std::move(res.error()));

                weight -= *res ? 1 : 0;
                return caller_ctx.context_type;
            } else { // entity_ptr, that is variable_entity
                return std::unexpected(make_error<basic_general_error>(var.name.location, "argument mismatch"sv, var));
            }
        }), optent);

    //if (!*opteid) {
    //    if (var.implicit) {
    //        // bind variable
    //        auto res = apply_visitor(expression_visitor{ caller_ctx }, expr);
    //        if (!res) return std::unexpected(std::move(res.error()));
    //        identifier varid = *var.name.value.begin();
    //        binding.emplace_back(annotated_identifier{ varid, var.name.location }, caller_ctx.context_type);
    //        return caller_ctx.context_type;
    //    }
    //    return std::unexpected(make_error<undeclared_identifier_error>(var.name));
    //}

    //auto res = apply_visitor(expression_visitor{ caller_ctx, annotated_entity_identifier{*opteid, var.name.location} }, expr);
    //if (!res) return std::unexpected(std::move(res.error()));

    //weight -= *res ? 1 : 0;
    //return caller_ctx.context_type;

#if 0
    if (var.implicit || (var.name.value.is_relative() && var.name.value.size() == 1)) {
        // check for function parameter
        identifier varid = *var.name.value.begin();
        if (auto opteids = binding.lookup(varid); opteids) {
            // bind result of expression to bind var
            auto res = apply_visitor(expression_visitor{ ctx }, expr);
            if (!res) return std::unexpected(std::move(res.error()));
            THROW_NOT_IMPLEMENTED_ERROR("bind variable_identifier");
        }
        //if (auto it = fd_.varparams().find(varid); it != fd_.varparams().end()) {
        //    return pattern_variable{ varid };
        //}
    }



    functional const* fl = ctx.lookup_functional(var.name.value);
    if (!fl) return std::unexpected(make_error<undeclared_identifier_error>(var.name));
        
    entity_identifier eid = fl->default_entity(ctx);
    if (!eid) return std::unexpected(make_error<undeclared_identifier_error>(var.name));

    auto res = apply_visitor(expression_visitor{ ctx, annotated_entity_identifier{eid, var.name.location} }, expr);
    if (!res) return std::unexpected(std::move(res.error()));

    weight -= *res ? 1 : 0;
    return ctx.context_type;
#endif
}

value_type_match_visitor::result_type value_type_match_visitor::operator()(function_call_t const& fc) const
{
    unit& u = caller_ctx.u();

    ct_expression_visitor sv{ callee_ctx, annotated_entity_identifier{ u.get(builtin_eid::qname) } };
    auto qn_ent_id = apply_visitor(sv, fc.fn_object);
    if (!qn_ent_id) return std::unexpected(std::move(qn_ent_id.error()));
    qname_identifier_entity qname_ent = static_cast<qname_identifier_entity const&>(u.eregistry_get(*qn_ent_id));

    // check if can evaluate signature_pattern_
    auto match = callee_ctx.find(qname_ent.value(), fc);
    if (match) {
        if (auto result = match->const_apply(callee_ctx); result) {
            expression_visitor evis{ caller_ctx, annotated_entity_identifier{ *result, fc.location() } };
            auto res = apply_visitor(evis, expr);
            if (!res) return std::unexpected(std::move(res.error()));
            weight -= *res ? 1 : 0;
            return caller_ctx.context_type;
        }
    }

    // can't evaluate signature_pattern_ as a function, consider as a pattern
    auto res = apply_visitor(expression_visitor{ caller_ctx }, expr);
    if (!res) return std::unexpected(std::move(res.error()));

    entity const& type_ent = u.eregistry_get(caller_ctx.context_type);
    entity_signature const* psig = type_ent.signature();
    if (!psig) {
        return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, expr));
    }

    if (qname_ent.value() != psig->name) {
        return std::unexpected(make_error<basic_general_error>(fc.location(), "argument mismatch"sv, expr));
    }

    size_t arg_index = 0;
    for (auto const& arg : fc.args()) {
        annotated_identifier const* argname = arg.name();
        field_descriptor const* pfd = argname ? psig->find_field(argname->value) : psig->find_field(arg_index++);
        if (!pfd) {
            return std::unexpected(make_error<basic_general_error>(fc.location(), "argument pattern mismatch"sv, expr));
        }
        BOOST_ASSERT(pfd->is_const());
        signature_matcher_visitor smvis{ callee_ctx, binding, pfd->entity_id() };
        if (auto err = apply_visitor(smvis, arg.value()); err) return std::unexpected(std::move(err));
    }

    return type_ent.id();

    //signature_matcher_visitor sm{ fc, binding };
    //auto res = sm.try_match(ctx, expr);
    //if (!res) return std::unexpected(std::move(res.error()));
    //weight -= *res ? 1 : 0;
    //return ctx.context_type;

    //// to do: check if can apply fc
    //// if no, consider as a pattern:

    //// is it really a pattern?
    //variable_identifier const* pfn_name = get<variable_identifier>(&fc.fn_object);
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
    ////fc.fn_object // variable_identifier
    //THROW_NOT_IMPLEMENTED_ERROR("value_type_match_visitor(function_call_t) not implemented expression");
}

value_type_match_visitor::result_type value_type_match_visitor::operator()(placeholder const& ph) const
{
    auto res = apply_visitor(base_expression_visitor{ caller_ctx }, expr);
    if (!res) return std::unexpected(std::move(res.error()));
    return apply_visitor(make_functional_visitor<result_type>([this, &ph /*, casted = res->second*/](auto& v) -> result_type {
        weight = -1; // template match
        if constexpr (std::is_same_v<semantic::managed_expression_list, std::decay_t<decltype(v)>>) {
            if (caller_ctx.context_type == caller_ctx.u().get(builtin_eid::void_)) {
                BOOST_ASSERT(v.empty());
                return entity_identifier{}; // ignore void
            }
            caller_ctx.expressions().splice_back(v);
        } else {
            if (v == caller_ctx.u().get(builtin_eid::void_)) {
                // expected value of any type instead of typename void
                return std::unexpected(make_error<type_mismatch_error>(get_start_location(expr), v, caller_ctx.u().get(builtin_eid::any), ph.location));
            }

            caller_ctx.append_expression(semantic::push_value{ v });
            entity const& e = caller_ctx.u().eregistry_get(v);
            caller_ctx.context_type = e.get_type();
            return caller_ctx.context_type;
        }
        return caller_ctx.context_type;
    }), res->first);
}

}
