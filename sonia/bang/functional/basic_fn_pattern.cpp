//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "basic_fn_pattern.hpp"

#include <array>
#include <boost/container/flat_set.hpp>

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/base_expression_visitor.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/errors/value_mismatch_error.hpp"
#include "sonia/bang/auxiliary.hpp"

#include "pattern_matcher.hpp"

namespace sonia::lang::bang {

//error_storage basic_fn_pattern::init(fn_compiler_context& ctx, parameter_list_t const& parameters)
//{
//    THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern::init is not implemented yet");
//}

error_storage basic_fn_pattern::init(fn_compiler_context& ctx, fn_pure_t const& fnd)
{
    location_ = fnd.location();
    result_ = fnd.result;

    parameters_.reserve(fnd.parameters.size());

    boost::container::small_flat_map<identifier, lex::resource_location, 8> parameter_ext_names_; // all external names of parameters
    boost::container::small_flat_map<identifier, lex::resource_location, 8> parameter_names_; // all parameters to bind during the call

    auto insert_param_name = [&](auto& map, annotated_identifier const& name) -> error_storage {
        if (auto pair = map.emplace(name.value, name.location); !pair.second) {
            // If the external name is already used, return the error
            return make_error<basic_general_error>(
                name.location, "Duplicate parameter name"sv, name.value, pair.first->second);
        }
        return error_storage{};
    };

    // to do: try to precompile expressions and patterns if possible
    
    // auxiliary
    std::array<char, 16> argname = { '$' };
    size_t argindex = 0;

    for (auto& param : fnd.parameters) {
        auto [external_name, internal_name] = apply_visitor(param_name_retriever{}, param.name);
        parameters_.emplace_back(
            external_name ? *external_name : annotated_identifier{},
            std::initializer_list<annotated_identifier>{},
            param.constraint,
            param.value);

        parameter_descriptor& pd = parameters_.back();
        pd.constraint = param.constraint;

        if (!external_name) {
            bool reversed = false;
            char* epos = mp::to_string(span{ &argindex, 1 }, argname.data() + 1, reversed);
            if (reversed) std::reverse(argname.data() + 1, epos);
            identifier nid = ctx.u().slregistry().resolve(string_view{ argname.data(), epos });
            
            if (!internal_name || internal_name->value != nid) {
                auto loc = apply_visitor(make_functional_visitor<lex::resource_location>([](auto const& f) {
                    if constexpr (std::is_same_v<pattern_t, std::decay_t<decltype(f)>>) {
                        return get_start_location(f);
                    } else { // pair<syntax_expression_t, bool>
                        return get_start_location(f.expression);
                    }
                }), param.constraint);
                if (auto err = insert_param_name(parameter_names_, annotated_identifier{ nid, loc }); err) return err;
                pd.inames.emplace_back(annotated_identifier{ nid, loc });
            }
            
            //matchers_.emplace_back(generated_internal_name);
            if (internal_name) {
                if (auto err = insert_param_name(parameter_names_, *internal_name); err) return err;
                //matchers_.back().push_internal_name(*internal_name);
                pd.inames.emplace_back(*internal_name);
            }
            
            ++argindex;
        } else {
            if (auto err = insert_param_name(parameter_ext_names_, *external_name); err) return err;

            annotated_identifier const& iname = internal_name ? *internal_name : *external_name;
            if (auto err = insert_param_name(parameter_names_, iname); err) return err;
            pd.inames.emplace_back(internal_name ? *internal_name : *external_name);
        }
    }
    return {};
}

std::expected<functional_match_descriptor_ptr, error_storage> basic_fn_pattern::try_match(fn_compiler_context& caller_ctx, prepared_call const& call, expected_result_t const& exp) const
{
    // quick mismatch check
    if (pattern_t const* rpattern = get<pattern_t>(&result_)) {
        if (!exp) {
            return std::unexpected(make_error<basic_general_error>(call.location, "Cannot match pattern without expected result"sv, nullptr, get_start_location(*rpattern)));
        }
    }

    unit& u = caller_ctx.u();

    // prepare binding
    functional_binding_set ct_call_binding; // binding for builtin call constants, e.g. #call_location
    ct_call_binding.emplace_back(
        annotated_identifier{ u.get(builtin_id::call_location) },
        u.make_string_entity(u.print(call.location)).id
    );
    fn_compiler_context callee_ctx{ caller_ctx, call.functional_name() };
    auto pmd = make_shared<functional_match_descriptor>(call);
    callee_ctx.push_binding(ct_call_binding);
    callee_ctx.push_binding(pmd->bindings);
    //SCOPE_EXIT([&ctx] { ctx.pop_binding(); }); // no need, temporary context
    
    entity_signature& call_sig = pmd->signature;

    // If the result is a pattern, we should handle it first.
    if (pattern_t const* rpattern = get<pattern_t>(&result_)) {
        BOOST_ASSERT(exp);
        auto err = pattern_matcher{ callee_ctx, pmd->bindings, call.expressions }.match(*rpattern, annotated_entity_identifier{ exp.type, exp.location });
        if (err) {
            return std::unexpected(append_cause(
                make_error<basic_general_error>(call.location, "Cannot match result pattern"sv, nullptr, get_start_location(*rpattern)),
                std::move(err)
            ));
        }
        call_sig.result.emplace( exp.type, false );
    }
    


    auto call_session = call.new_session(caller_ctx);

    auto param_it = parameters_.begin(), param_end = parameters_.end();
    for (; param_it != param_end; ++param_it) {
        parameter_descriptor const& pd = *param_it;
        syntax_expression_t const* parg_expr;
        expected_result_t argexp{};
        bool ellipsis = false;

        constraint_expression_t const* pconstraint = get<constraint_expression_t>(&pd.constraint);
        entity_identifier pconstraint_value_eid;
        if (pconstraint) {
            auto cnt_res = apply_visitor(ct_expression_visitor{ callee_ctx, call.expressions }, pconstraint->expression);
            if (!cnt_res) {
                return std::unexpected(append_cause(
                    make_error<basic_general_error>(call.location, "Cannot evaluate constraint for parameter"sv, pd.ename.value, pd.ename.location),
                    std::move(cnt_res.error())
                ));
            }
            argexp.location = get_start_location(pconstraint->expression);
            syntax_expression_const_result_t& cnt_res_er = *cnt_res;
            entity const& cnt_res_ent = get_entity(u, cnt_res_er.value);
            if (cnt_res_ent.get_type() == u.get(builtin_eid::typename_)) {
                argexp.type = cnt_res_ent.id;
            } else {
                pconstraint_value_eid = cnt_res_er.value;
                argexp.type = cnt_res_ent.get_type();
                argexp.is_const_result = true;
            }
            ellipsis = pconstraint->ellipsis;
        } // else it's a pattern, so we won't use a type constraint directly
        auto res = [&call_session, &pd](expected_result_t& argexp, syntax_expression_t const** parg) {
            if (pd.ename) {
                return call_session.use_named_argument(pd.ename.value, argexp, parg);
            } else {
                return call_session.use_next_positioned_argument(argexp, parg);
            }
        }(argexp, &parg_expr);

        annotated_identifier const& param_name = pd.ename.self_or(pd.inames.front());

        if (!res) {
            if (res.error()) {
                return std::unexpected(append_cause(
                    make_error<basic_general_error>(call.location, "Cannot match argument"sv, param_name.value, param_name.location),
                    std::move(res.error())
                ));
            }
            if (!pd.default_value) {
                return std::unexpected(make_error<basic_general_error>(call.location, "Argument not found"sv, param_name.value, param_name.location));
            }
            // try default value
            res = apply_visitor(base_expression_visitor{ callee_ctx, call.expressions, argexp }, *pd.default_value);
            if (!res) {
                return std::unexpected(append_cause(
                    make_error<basic_general_error>(call.location, "Cannot evaluate default value for argument"sv, param_name.value, param_name.location),
                    std::move(res.error())
                ));
            }
        }
        
        syntax_expression_result_t& arg_er = res->first;
        if (pconstraint) {
            if (argexp.is_const_result) {
                // check exact value match
                if (arg_er.value() != pconstraint_value_eid) {
                    return std::unexpected(append_cause(
                        make_error<basic_general_error>(call.location, "Argument value does not match constraint"sv, param_name.value, param_name.location),
                        make_error<value_mismatch_error>(get_start_location(*parg_expr), arg_er.value(), pconstraint_value_eid, get_start_location(pconstraint->expression))
                    ));
                }
            }
            // if no errors here, then matched
        } else {
            entity_identifier type_to_match;
            if (arg_er.is_const_result) {
                entity const& arg_res_entity = get_entity(u, arg_er.value());
                type_to_match = arg_res_entity.get_type();
            } else {
                type_to_match = arg_er.type();
            }
            pattern_t const& pattern = get<pattern_t>(pd.constraint);
            auto err = pattern_matcher{ callee_ctx, pmd->bindings, call.expressions }
                .match(pattern, annotated_entity_identifier{ type_to_match, get_start_location(*parg_expr) });
            if (err) {
                return std::unexpected(append_cause(
                    make_error<basic_general_error>(call.location, "Cannot match argument pattern"sv, param_name.value, param_name.location),
                    std::move(err)
                ));
            }
            ellipsis = pattern.ellipsis;
        }
        pmd->weight -= res->second;
        
        // bind names
        if (!ellipsis) {
            if (pd.ename) {
                call_sig.emplace_back(pd.ename.value, arg_er.value_or_type, arg_er.is_const_result);
            } else {
                call_sig.emplace_back(arg_er.value_or_type, arg_er.is_const_result);
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
                pmd->emplace_back(param_it - parameters_.begin(), arg_er);
            }
        } else {
            THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern : ellipsis");
        }
        // parameter_match_result& pmr = pmd->get_match_result(pd.ename.value);
    }

    // Check if there are any unused arguments
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(),
            "argument mismatch"sv, std::move(argterm.value())));
    }

    if (syntax_expression_t const* rexpr = get<syntax_expression_t>(&result_)) {
        auto res = apply_visitor(base_expression_visitor{ callee_ctx, call.expressions, expected_result_t{ entity_identifier{}, true } }, *rexpr);
        if (!res) {
            return std::unexpected(append_cause(
                make_error<basic_general_error>(call.location, "Cannot evaluate result expression"sv, nullptr, get_start_location(*rexpr)),
                std::move(res.error())
            ));
        }
        syntax_expression_result_t& res_er = res->first;
        call_sig.result.emplace(res_er.value(), true);
    }

    pmd->void_spans = std::move(call_session.void_spans);
    return pmd;
}

std::pair<syntax_expression_result_t, size_t> basic_fn_pattern::apply_arguments(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    syntax_expression_result_t result{ .expressions = md.merge_void_spans(el) };
    for (auto& [_, mr] : md.matches) {
        result.temporaries.insert(result.temporaries.end(), mr.temporaries.begin(), mr.temporaries.end());
        result.stored_expressions = el.concat(result.stored_expressions, mr.stored_expressions);
        result.expressions = el.concat(result.expressions, mr.expressions);
    }
    return { result, md.matches.size() };
}

std::ostream& basic_fn_pattern::print(unit const& u, std::ostream& ss) const
{
    //THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern::print is not implemented yet");
    size_t posargnum = 0;
    bool first = true;
    ss << '(';
    for (parameter_descriptor const& pd : parameters_) {
        if (first) first = false;
        else ss << ", "sv;

        if (pd.ename) {
            u.print_to(ss, pd.ename.value) << ": "sv;
        }
        apply_visitor(make_functional_visitor<void>([&u, &ss](auto const& m) {
            if constexpr (std::is_same_v<pattern_t, std::decay_t<decltype(m)>>) {
                u.print_to(ss, m);
            } else if constexpr (std::is_same_v<constraint_expression_t, std::decay_t<decltype(m)>>) {
                u.print_to(ss << "~ "sv, m.expression);
                if (m.ellipsis) {
                    ss << "..."sv;
                }
            } else {
                THROW_INTERNAL_ERROR("basic_fn_pattern::print: unknown parameter matcher type"sv);
            }
        }), pd.constraint);
    }
    
    if (0 == result_.which()) {
        ss << ")->auto"sv;
    } else if (auto const* pexpr = get<syntax_expression_t>(&result_)) {
        u.print_to(ss << ")~>"sv, *pexpr);
    } else if (auto const* rpattern = get<pattern_t>(&result_)) {
        u.print_to(ss << ")->"sv, *rpattern);
    }
    return ss;
}

#if 0
// return appended argument expressions count
std::pair<semantic::expression_span, size_t> basic_fn_pattern::apply_any_argument(unit& u, parameter_match_result& pmr, semantic::expression_list_t& el) const
{
#if 0
    std::pair<semantic::expression_span, size_t> result{ semantic::expression_span{}, 0 };
    for (auto& ser : pmr.results) {
        get<0>(result) = el.concat(get<0>(result), ser.expressions);
        if (ser.is_const_result) {
            u.push_back_expression(el, get<0>(result), semantic::push_value(ser.value()));
        }
        ++get<1>(result);
    }
    return result;
#endif
    THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern::apply_any_argument is not implemented yet");
}

std::pair<semantic::expression_span, size_t> basic_fn_pattern::apply_argument(unit&, parameter_match_result& pmr, semantic::expression_list_t& el) const
{
    THROW_NOT_IMPLEMENTED_ERROR("basic_fn_pattern::apply_argument is not implemented yet");
    //return apply_mut_argument(pmr, el);
}
#endif

}
