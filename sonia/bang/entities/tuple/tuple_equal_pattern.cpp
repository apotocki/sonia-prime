//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_equal_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

class tuple_equal_match_descriptor : public functional_match_descriptor
{
public:
    tuple_equal_match_descriptor(entity const& lhs, entity const& rhs, lex::resource_location loc) noexcept
        : functional_match_descriptor{ std::move(loc) }
        , lhs_entity_type{ lhs }
        , rhs_entity_type{ rhs }
    {}

    entity const& lhs_entity_type;
    entity const& rhs_entity_type;
};

std::expected<functional_match_descriptor_ptr, error_storage>
tuple_equal_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();

    auto call_session = call.new_session(ctx);

    // Get lhs tuple
    syntax_expression_t const* plhs_expr;
    auto lhs_arg = call_session.use_next_positioned_argument(&plhs_expr);
    if (!lhs_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location, "missing first tuple argument"sv));
    }

    // Get rhs tuple
    syntax_expression_t const* prhs_expr;
    auto rhs_arg = call_session.use_next_positioned_argument(&prhs_expr);
    if (!rhs_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location, "missing second tuple argument"sv));
    }

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    // Both must be tuple types
    entity_identifier lhs_type = lhs_arg->is_const_result
        ? get_entity(u, lhs_arg->value()).get_type()
        : lhs_arg->type();
    entity_identifier rhs_type = rhs_arg->is_const_result
        ? get_entity(u, rhs_arg->value()).get_type()
        : rhs_arg->type();

    entity const& lhs_entity_type = get_entity(u, lhs_type);
    entity const& rhs_entity_type = get_entity(u, rhs_type);

    auto* lhs_sig = lhs_entity_type.signature();
    auto* rhs_sig = rhs_entity_type.signature();

    if (!lhs_sig || lhs_sig->name != u.get(builtin_qnid::tuple)) {
        return std::unexpected(make_error<type_mismatch_error>(call.location, lhs_entity_type.id, "a tuple"sv));
    }
    if (!rhs_sig || rhs_sig->name != u.get(builtin_qnid::tuple)) {
        return std::unexpected(make_error<type_mismatch_error>(call.location, rhs_entity_type.id, "a tuple"sv));
    }

    // Check field count and names
    if (lhs_sig->field_count() != rhs_sig->field_count()) {
        return std::unexpected(make_error<basic_general_error>(call.location, "tuple field count mismatch"sv));
    }
    for (size_t i = 0; i < lhs_sig->field_count(); ++i) {
        auto const& lhs_field = *lhs_sig->get_field(i);
        auto const& rhs_field = *rhs_sig->get_field(i);
        if (lhs_field.name() != rhs_field.name()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "tuple field name mismatch"sv));
        }
    }

    auto pmd = make_shared<tuple_equal_match_descriptor>(lhs_entity_type, rhs_entity_type, call.location);
    pmd->get_match_result(0).append_result(*lhs_arg);
    pmd->get_match_result(1).append_result(*rhs_arg);
    pmd->void_spans = std::move(call_session.void_spans);
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage>
tuple_equal_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    tuple_equal_match_descriptor& eq_md = static_cast<tuple_equal_match_descriptor&>(md);
    auto& lhs_er = md.get_match_result(0).results.front();
    auto& rhs_er = md.get_match_result(1).results.front();

    entity_signature const& lhs_sig = *eq_md.lhs_entity_type.signature();
    entity_signature const& rhs_sig = *eq_md.rhs_entity_type.signature();

    fn_compiler_context_scope fn_scope{ ctx };

    semantic::expression_span void_spans = md.merge_void_spans(el);

    lhs_er.temporaries.insert(lhs_er.temporaries.end(), rhs_er.temporaries.begin(), rhs_er.temporaries.end());

    syntax_expression_result_t result{
        .temporaries = std::move(lhs_er.temporaries),
        .stored_expressions = el.concat(lhs_er.stored_expressions, rhs_er.stored_expressions),
        .expressions = void_spans,
        .value_or_type = u.get(builtin_eid::boolean),
        .is_const_result = false
    };
    
    local_variable* lhs_tuple_var = nullptr, * rhs_tuple_var = nullptr;
    identifier lhs_tuple_var_name, rhs_tuple_var_name;

    // For each field, compare using builtin_qnid::eq
    semantic::expression_span * current_branch_owner_code = nullptr;
    for (size_t i = 0; i < lhs_sig.field_count(); ++i) {
        auto const& lhs_field = lhs_sig.field(i);
        auto const& rhs_field = rhs_sig.field(i);

        if (lhs_field.entity_id() == rhs_field.entity_id() && lhs_field.is_const() == rhs_field.is_const() && lhs_field.is_const()) {
            // If both fields are the same const entity, we can skip the comparison
            continue;
        }

        if (!rhs_field.is_const() && !rhs_tuple_var) {
            rhs_tuple_var_name = u.new_identifier();
            rhs_tuple_var = &fn_scope.new_temporary(rhs_tuple_var_name, eq_md.rhs_entity_type.id);
        }

        pure_call_t eq_call{ md.location };
        if (lhs_field.is_const()) {
            // If lhs field is const, we can use it directly
            eq_call.emplace_back(annotated_entity_identifier{ lhs_field.entity_id(), md.location }); // lhs argument location is needed
        } else {
            if (!lhs_tuple_var) {
                lhs_tuple_var_name = u.new_identifier();
                lhs_tuple_var = &fn_scope.new_temporary(lhs_tuple_var_name, eq_md.lhs_entity_type.id);
            }
            pure_call_t get_call{ md.location };
            get_call.emplace_back(annotated_identifier{ u.get(builtin_id::self), md.location },
                variable_reference{ annotated_qname{ qname{ lhs_tuple_var_name, false } }, false });
            get_call.emplace_back(annotated_identifier{ u.get(builtin_id::property) }, annotated_integer{ mp::integer{ i } });
            auto match = ctx.find(builtin_qnid::get, get_call, el);
            if (!match) {
                return std::unexpected(append_cause(
                    make_error<basic_general_error>(md.location, "internal error: can't get tuple element"sv, annotated_integer{ mp::integer{ i } }),
                    std::move(match.error())));
            }
            auto res = match->apply(ctx);
            if (!res) {
                return std::unexpected(append_cause(
                    make_error<basic_general_error>(md.location, "internal error: can't get tuple element"sv, annotated_integer{ mp::integer{ i } }),
                    std::move(res.error())));
            }
            result.stored_expressions = el.concat(result.stored_expressions, res->stored_expressions);
            semantic::managed_expression_list el{ u };
            el.deep_copy(res->expressions);
            eq_call.emplace_back(indirect_value{
                .location = md.location,
                .type = lhs_field.entity_id(),
                .store = indirect_value_store_t{ in_place_type<semantic::indirect_expression_list>, std::move(el) }
            });
        }

        if (rhs_field.is_const()) {
            // If rhs field is const, we can use it directly
            eq_call.emplace_back(annotated_entity_identifier{ rhs_field.entity_id(), md.location }); // rhs argument location is needed
        } else {
            if (!rhs_tuple_var) {
                rhs_tuple_var_name = u.new_identifier();
                rhs_tuple_var = &fn_scope.new_temporary(rhs_tuple_var_name, eq_md.rhs_entity_type.id);
            }
            pure_call_t get_call{ md.location };
            get_call.emplace_back(annotated_identifier{ u.get(builtin_id::self), md.location },
                variable_reference{ annotated_qname{ qname{ rhs_tuple_var_name, false } }, false });
            get_call.emplace_back(annotated_identifier{ u.get(builtin_id::property) }, annotated_integer{ mp::integer{ i } });
            auto match = ctx.find(builtin_qnid::get, get_call, el);
            if (!match) {
                return std::unexpected(append_cause(
                    make_error<basic_general_error>(md.location, "internal error: can't get tuple element"sv, annotated_integer{ mp::integer{ i } }),
                    std::move(match.error())));
            }
            auto res = match->apply(ctx);
            if (!res) {
                return std::unexpected(append_cause(
                    make_error<basic_general_error>(md.location, "internal error: can't get tuple element"sv, annotated_integer{ mp::integer{ i } }),
                    std::move(res.error())));
            }
            result.stored_expressions = el.concat(result.stored_expressions, res->stored_expressions);
            semantic::managed_expression_list el{ u };
            el.deep_copy(res->expressions);
            eq_call.emplace_back(indirect_value{
                .location = md.location,
                .type = rhs_field.entity_id(),
                .store = indirect_value_store_t{ in_place_type<semantic::indirect_expression_list>, std::move(el) }
            });
        }
        auto match = ctx.find(builtin_qnid::eq, eq_call, el, expected_result_t{ u.get(builtin_eid::boolean), false });
        if (!match) {
            return std::unexpected(append_cause(
                make_error<binary_relation_error>(md.location, "can't compare tuple fields"sv, lhs_field.entity_id(), rhs_field.entity_id()),
                std::move(match.error())));
        }
        auto eq_res = match->apply(ctx);
        if (!eq_res) {
            return std::unexpected(append_cause(
                make_error<binary_relation_error>(md.location, "can't compare tuple fields"sv, lhs_field.entity_id(), rhs_field.entity_id()),
                std::move(eq_res.error())));
        }

        if (eq_res->is_const_result) {
            // If the result is constant, we can optimize it
            if (eq_res->value() == u.get(builtin_eid::true_)) {
                continue; // This field is equal, skip it
            } else {
                BOOST_ASSERT(eq_res->value() == u.get(builtin_eid::false_));
                // If any field is not equal, the whole tuple is not equal
                return syntax_expression_result_t{
                    .expressions = void_spans,
                    .value_or_type = u.get(builtin_eid::false_),
                    .is_const_result = true
                };
            }
        }

        // to do: treat eq_res temporaries as temporaries of the current branch

        result.stored_expressions = el.concat(result.stored_expressions, eq_res->stored_expressions);
        if (current_branch_owner_code) {
            // append conditional branch
            u.push_back_expression(el, *current_branch_owner_code, semantic::conditional_t{});
            semantic::conditional_t& cond = get<semantic::conditional_t>(current_branch_owner_code->back());
            
            // removing 'true' from previous field equality check
            u.push_back_expression(el, cond.true_branch, semantic::truncate_values{ 1, false });

            // append the result of current field equality check
            cond.true_branch = el.concat(cond.true_branch, eq_res->expressions);

            // store brunch expressions as stored expressions if they are not primary branch expressions
            if (&result.expressions != current_branch_owner_code) {
                result.stored_expressions = el.concat(result.stored_expressions, *current_branch_owner_code);
            }

            current_branch_owner_code = &cond.true_branch;
        } else {
            // Otherwise, we need to create a new conditional expression
            result.expressions = el.concat(result.expressions, eq_res->expressions);
            current_branch_owner_code = &result.expressions;
        }
    }

    if (!current_branch_owner_code) {
        // If we have no branches, all fields are consts and equal, so we can return true
        return syntax_expression_result_t{
            .expressions = void_spans,
            .value_or_type = u.get(builtin_eid::true_),
            .is_const_result = true
        };
    }
    if (&result.expressions != current_branch_owner_code) {
        result.stored_expressions = el.concat(result.stored_expressions, *current_branch_owner_code);
    }

    if (lhs_tuple_var) {
        result.temporaries.emplace_back(lhs_tuple_var->varid, lhs_tuple_var->type, lhs_er.expressions);
    }
    if (rhs_tuple_var) {
        result.temporaries.emplace_back(rhs_tuple_var->varid, rhs_tuple_var->type, rhs_er.expressions);
    }
    return std::move(result);
}

} // namespace sonia::lang::bang
