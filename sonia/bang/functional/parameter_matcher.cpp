//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "parameter_matcher.hpp"

#include "sonia/bang/unit.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/base_expression_visitor.hpp"
#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/auxiliary.hpp"

#include "pattern_matcher.hpp"

#include "sonia/bang/errors/value_mismatch_error.hpp"

namespace sonia::lang::bang {

parameter_matcher::parameter_matcher(
    fn_compiler_context& caller_ctx,
    prepared_call const& c,
    basic_fn_pattern::parameters_t const& ps)
        : param_bit{ ps.begin() }, param_it{ ps.begin() }, param_end{ ps.end() }
        , call{ c }
        , call_session{ c.new_session(caller_ctx) }
{

}

// annotated_entity_identifier const& type, functional_binding_set& binding, semantic::expression_list_t& expressions) const
error_storage parameter_matcher::match(fn_compiler_context& callee_ctx)
{
    unit& u = callee_ctx.u();

    for (; param_it != param_end; ++param_it) {
        basic_fn_pattern::parameter_descriptor const& pd = *param_it;
        annotated_identifier const& param_name = pd.ename.self_or(pd.inames.front());
        entity_identifier pconstraint_value_eid;
        expected_result_t argexp{ .modifier = parameter_constraint_modifier_t::none };

        error_storage err = apply_visitor(make_functional_visitor<error_storage>([&](auto const& v) {
            if constexpr (std::is_same_v<pattern_t, std::decay_t<decltype(v)>>) {
                argexp.modifier = v.modifier;
                return error_storage{};
            } else if constexpr (std::is_same_v<constraint_expression_t, std::decay_t<decltype(v)>>) {
                auto argexp_res = resolve_expression_expected_result(callee_ctx, param_name, v, pconstraint_value_eid);
                if (!argexp_res) return std::move(argexp_res.error());
                argexp = std::move(*argexp_res);
                return error_storage{};
            } else {
                return make_error<basic_general_error>(v.location, "Unsupported parameter constraint type"sv);
            }
        }), pd.constraint);
        if (err) return err;

        if ((argexp.modifier & parameter_constraint_modifier_t::ellipsis) == parameter_constraint_modifier_t::ellipsis) {
            if (pd.ename) {
                //handle_named_ellipsis(callee_ctx, call, pd.ename.value, pmd->bindings);
                THROW_NOT_IMPLEMENTED_ERROR("parameter_matcher : ellipsis with named parameter");
            } else {
                if (pconstraint_value_eid) {
                    return make_error<basic_general_error>(
                        call.location, "Ellipsis with constraint value is not supported"sv, param_name.value, param_name.location);
                }
                err = handle_positioned_ellipsis(callee_ctx, argexp);// callee_ctx, call, pmd->bindings);
                if (err) return err;
            }
            continue;
        }

        std::pair<syntax_expression_t const*, size_t> arg_expr_idx;
        auto res = [this, &pd, &arg_expr_idx](expected_result_t& argexp) {
            if (pd.ename) {
                return call_session.use_named_argument(pd.ename.value, argexp, &arg_expr_idx);
            } else {
                return call_session.use_next_positioned_argument(argexp, &arg_expr_idx);
            }
        }(argexp);

        if (!res) {
            if (res.error()) {
                return append_cause(
                    make_error<basic_general_error>(call.location, "Cannot match argument"sv, param_name.value, param_name.location),
                    std::move(res.error())
                );
            }
            if (!pd.default_value) {
                return make_error<basic_general_error>(call.location, "Argument not found"sv, param_name.value, param_name.location);
            }
            // try default value
            res = apply_visitor(base_expression_visitor{ callee_ctx, call.expressions, argexp }, *pd.default_value);
            if (!res) {
                return append_cause(
                    make_error<basic_general_error>(call.location, "Cannot evaluate default value for argument"sv, param_name.value, param_name.location),
                    std::move(res.error())
                );
            }
        }

        syntax_expression_result_t& arg_er = res->first;
        err = apply_visitor(make_functional_visitor<error_storage>([&](auto const& constraint) -> error_storage {
            if constexpr (std::is_same_v<constraint_expression_t, std::decay_t<decltype(constraint)>>) {
                if (argexp.can_be_only_constexpr()) {
                    // check exact value match
                    if (arg_er.value() != pconstraint_value_eid) {
                        return append_cause(
                            make_error<basic_general_error>(call.location, "Argument value does not match constraint"sv, param_name.value, param_name.location),
                            make_error<value_mismatch_error>(get_start_location(*get<0>(arg_expr_idx)), arg_er.value(), pconstraint_value_eid, get_start_location(constraint.expression))
                        );
                    }
                }
            } else if constexpr (std::is_same_v<pattern_t, std::decay_t<decltype(constraint)>>) {
                entity_identifier type_to_match;
                if (arg_er.is_const_result) {
                    entity const& arg_res_entity = get_entity(u, arg_er.value());
                    type_to_match = arg_res_entity.get_type();
                } else {
                    type_to_match = arg_er.type();
                }
                auto err = pattern_matcher{ callee_ctx, pmd->bindings, call.expressions }
                    .match(constraint, annotated_entity_identifier{ type_to_match, get_start_location(*get<0>(arg_expr_idx)) });
                if (err) {
                    return append_cause(
                        make_error<basic_general_error>(call.location, "Cannot match argument pattern"sv, param_name.value, param_name.location),
                        std::move(err)
                    );
                }
            } else {
                return make_error<basic_general_error>(call.location, "Unsupported parameter constraint type"sv, param_name.value, param_name.location);
            }
            return error_storage{};
        }), pd.constraint);
        if (err) return err;
        
        pmd->weight -= res->second;

        if (pd.ename) {
            pmd->signature.emplace_back(pd.ename.value, arg_er.value_or_type, arg_er.is_const_result);
        } else {
            pmd->signature.emplace_back(arg_er.value_or_type, arg_er.is_const_result);
        }
        if (arg_er.is_const_result) {
            for (auto const& iname : pd.inames) {
                pmd->bindings.emplace_back(
                    annotated_identifier{ iname.value, iname.location }, arg_er.value()
                );
            }
            if (arg_er.expressions) pmd->void_spans.emplace_back(arg_er.expressions);
        } else {
            local_variable var{ .type = arg_er.type(), .varid = u.new_variable_identifier(), .is_weak = false };
            for (auto const& iname : pd.inames) {
                pmd->bindings.emplace_back(
                    annotated_identifier{ iname.value, iname.location }, var
                );
            }
            pmd->emplace_back(param_it - param_bit, arg_er);
        }
    }

    // Check if there are any unused arguments
    if (auto argterm = call_session.unused_argument(); argterm) {
        return make_error<basic_general_error>(argterm.location(),
            "argument mismatch"sv, std::move(argterm.value()));
    }
    pmd->void_spans = std::move(call_session.void_spans);
    return {};
}

std::expected<expected_result_t, error_storage>
parameter_matcher::resolve_expression_expected_result(fn_compiler_context& callee_ctx, annotated_identifier const& pn, constraint_expression_t const& constraint, entity_identifier& pconstraint_value_eid)
{
    unit& u = callee_ctx.u();
    auto cnt_res = apply_visitor(base_expression_visitor{ callee_ctx, call.expressions, expected_result_t{ .modifier = parameter_constraint_modifier_t::const_type } }, constraint.expression);
    if (!cnt_res) {
        return std::unexpected(append_cause(
            make_error<basic_general_error>(call.location, "Cannot evaluate constraint for parameter"sv, pn.value, pn.location),
            std::move(cnt_res.error())
        ));
    }
    expected_result_t expr_exp{ .location = get_start_location(constraint.expression), .modifier = constraint.modifier };
    syntax_expression_result_t& cnt_res_er = cnt_res->first;
    entity const& cnt_res_ent = get_entity(u, cnt_res_er.value());
    
    if (cnt_res_ent.get_type() == u.get(builtin_eid::typename_)) {
        expr_exp.type = cnt_res_ent.id;
    } else {
        pconstraint_value_eid = cnt_res_er.value();
        expr_exp.type = cnt_res_ent.get_type();
        if (expr_exp.can_be_only_runtime_type()) {
            return std::unexpected(make_error<basic_general_error>(
                pn.location, "Constraint value is a compile-time constant, but the modifier requires runtime type"sv, nullptr, get_start_location(constraint.expression)));
        }
        expr_exp.modifier = (expr_exp.modifier & parameter_constraint_modifier_t::ellipsis) | parameter_constraint_modifier_t::const_type;
    }
    return expr_exp;
}

//error_storage try_match_argument(fn_compiler_context& callee_ctx)
//{
//
//}

error_storage parameter_matcher::handle_positioned_ellipsis(fn_compiler_context& callee_ctx, expected_result_t argexp)
{
    unit& u = callee_ctx.u();

    basic_fn_pattern::parameter_descriptor const& epd = *param_it;
    
    small_vector<std::pair<annotated_identifier, syntax_expression_result_t>, 8> accumulated_results;
    
    small_vector<basic_fn_pattern::parameter_descriptor const*, 8> param_descriptors{ &epd };
    small_vector<expected_result_t, 8> param_exps{ argexp };

    // get next not-ellipsis parameter or named parameter
    parameter_iteator next_param_it = param_it;
    for (++next_param_it;; ++next_param_it) {
        if (next_param_it == param_end) {
            --next_param_it;
            break;
        }
        bool ellipsis;
        basic_fn_pattern::parameter_descriptor const& pd = *next_param_it;
        constraint_expression_t const* pconstraint = get<constraint_expression_t>(&pd.constraint);
        if (pconstraint) {
            entity_identifier pconstraint_value_eid;
            annotated_identifier const& param_name = pd.ename.self_or(pd.inames.front());
            auto argexp_res = resolve_expression_expected_result(callee_ctx, param_name, *pconstraint, pconstraint_value_eid);
            if (!argexp_res) { // error resolving constraint, rollback
                --next_param_it;
                break;
            }
            param_exps.emplace_back(std::move(*argexp_res));
            ellipsis = (pconstraint->modifier & parameter_constraint_modifier_t::ellipsis) == parameter_constraint_modifier_t::ellipsis;
            if (ellipsis && pconstraint_value_eid) {
                return make_error<basic_general_error>(
                    call.location, "Ellipsis with constraint value is not supported"sv, param_name.value, param_name.location);
            }
        } else {
            ellipsis = (get<pattern_t>(pd.constraint).modifier & parameter_constraint_modifier_t::ellipsis) == parameter_constraint_modifier_t::ellipsis;
            param_exps.emplace_back();
        }
        param_descriptors.emplace_back(&pd);
        if (pd.ename || !ellipsis) break;
        
        //bool ellipsis = apply_visitor(make_functional_visitor<bool>([](auto const& v) {
        //    return v.ellipsis;
        //}), pd.constraint);
    }

    basic_fn_pattern::parameter_descriptor const& pd = *next_param_it;
    annotated_identifier const& param_name = pd.ename.self_or(pd.inames.front());

    size_t ellipsis_group_index = param_exps.size() - 1;

    for (;;) {
        std::tuple<identifier, syntax_expression_t const*, size_t> arg_expr;
        auto res = call_session.use_next_argument(param_exps[ellipsis_group_index], &arg_expr);
        // if error and ellipsis_group_index != 0, reuse argument for previous parameter
        if (!res && res.error()) {
            if (ellipsis_group_index) {
                --ellipsis_group_index;
                call_session.reuse_argument(get<2>(arg_expr));
                continue;
            }
            annotated_identifier const& ellipsis_param_name = epd.ename.self_or(epd.inames.front());
            return append_cause(
                make_error<basic_general_error>(call.location, "Cannot match ellipsis argument"sv, ellipsis_param_name.value, ellipsis_param_name.location),
                std::move(res.error())
            );
        }

        if (!res.error()) { // no more arguments
            finalize_ellipsis(u, accumulated_results);
            break;
        }
        if (pattern_t const* ppattern = get<pattern_t>(&param_descriptors[ellipsis_group_index]->constraint)) {
            entity_identifier type_to_match;
            syntax_expression_result_t& arg_er = res->first;
            if (arg_er.is_const_result) {
                entity const& arg_res_entity = get_entity(u, arg_er.value());
                type_to_match = arg_res_entity.get_type();
            } else {
                type_to_match = arg_er.type();
            }
            // if pattern, match it (including concepts)
            auto err = pattern_matcher{ callee_ctx, pmd->bindings, call.expressions }
                .match(*ppattern, annotated_entity_identifier{ type_to_match, get_start_location(*get<1>(arg_expr)) });
            // if error and ellipsis_group_index != 0, reuse argument for previous parameter
            if (err) {
                if (ellipsis_group_index) {
                    --ellipsis_group_index;
                    call_session.reuse_argument(get<2>(arg_expr));
                    continue;
                }
                annotated_identifier const& ellipsis_param_name = epd.ename.self_or(epd.inames.front());
                return append_cause(
                    make_error<basic_general_error>(call.location, "Cannot match ellipsis argument pattern"sv, ellipsis_param_name.value, ellipsis_param_name.location),
                    std::move(err)
                );
            }
        }

        if (!ellipsis_group_index) {
            accumulated_results.emplace_back(pd.ename ? pd.ename : annotated_identifier{ .location = param_name.location }, std::move(res->first));
            pmd->weight -= res->second;
            ellipsis_group_index = param_exps.size() - 1;
        } else { // matched by next to ellipsis parameter
            // finalize ellipsis
            finalize_ellipsis(u, accumulated_results);

            // to do: optimize, next argument hs already matched, do not match it again
            call_session.reuse_argument(get<2>(arg_expr));
            break;
        }
    }

    return {};
}

void parameter_matcher::finalize_ellipsis(unit& u, span<std::pair<annotated_identifier, syntax_expression_result_t>> ellipsis_span)
{
    basic_fn_pattern::parameter_descriptor const& epd = *param_it;
    entity_signature ellipsis_type_sig(u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_));
    for (auto& [name, er] : ellipsis_span) {
        if (er.is_const_result) {
            if (name) {
                ellipsis_type_sig.emplace_back(name.value, er.value(), true);
            } else {
                ellipsis_type_sig.emplace_back(er.value(), true);
            }
        } else {
            identifier unnamedid = u.new_identifier();
            entity_identifier unnamedid_entid = u.make_qname_entity(qname{ unnamedid, false }).id;
            
            if (name) {
                ellipsis_type_sig.emplace_back(name.value, unnamedid_entid, true);
            } else {
                ellipsis_type_sig.emplace_back(unnamedid_entid, true);
            }

            local_variable var{ .type = er.type(), .varid = u.new_variable_identifier(), .is_weak = false };
            pmd->bindings.emplace_back( annotated_identifier{ unnamedid, name.location }, var );
            pmd->emplace_back(param_it - param_bit, er);
        }
    }
    basic_signatured_entity const& ellipsis_type = u.make_basic_signatured_entity(std::move(ellipsis_type_sig));
    entity_identifier ellipsis_type_unit_eid = u.make_empty_entity(ellipsis_type).id;
    if (epd.ename) {
        pmd->signature.emplace_back(epd.ename.value, ellipsis_type_unit_eid, true);
    } else {
        pmd->signature.emplace_back(ellipsis_type_unit_eid, true);
    }
    for (auto const& iname : epd.inames) {
        pmd->bindings.emplace_back(
            annotated_identifier{ iname.value, iname.location }, ellipsis_type_unit_eid
        );
    }
}

}
