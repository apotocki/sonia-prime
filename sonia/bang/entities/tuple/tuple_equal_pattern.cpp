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
    tuple_equal_match_descriptor(prepared_call const& call, entity const& lhs, entity const& rhs) noexcept
        : functional_match_descriptor{ call }
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
    std::pair<syntax_expression_t const*, size_t> lhs_expr;
    auto lhs_arg = call_session.use_next_positioned_argument(&lhs_expr);
    if (!lhs_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location, "missing first tuple argument"sv));
    }

    // Get rhs tuple
    std::pair<syntax_expression_t const*, size_t> rhs_expr;
    auto rhs_arg = call_session.use_next_positioned_argument(&rhs_expr);
    if (!rhs_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location, "missing second tuple argument"sv));
    }

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    syntax_expression_result_t & lhs_arg_er = lhs_arg->first;
    syntax_expression_result_t & rhs_arg_er = rhs_arg->first;

    // Both must be tuple types
    entity_identifier lhs_type = lhs_arg_er.is_const_result
        ? get_entity(u, lhs_arg_er.value()).get_type()
        : lhs_arg_er.type();
    entity_identifier rhs_type = rhs_arg_er.is_const_result
        ? get_entity(u, rhs_arg_er.value()).get_type()
        : rhs_arg_er.type();

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

    auto pmd = make_shared<tuple_equal_match_descriptor>(call, lhs_entity_type, rhs_entity_type);
    pmd->emplace_back(0, lhs_arg_er);
    pmd->emplace_back(1, rhs_arg_er);
    pmd->void_spans = std::move(call_session.void_spans);
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage>
tuple_equal_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    tuple_equal_match_descriptor& eq_md = static_cast<tuple_equal_match_descriptor&>(md);
    auto& lhs_er = get<1>(md.matches[0]);
    auto& rhs_er = get<1>(md.matches[1]);

    entity_signature const& lhs_sig = *eq_md.lhs_entity_type.signature();
    entity_signature const& rhs_sig = *eq_md.rhs_entity_type.signature();

    semantic::expression_span void_spans = md.merge_void_spans(el);

    // Check field count and names
    if (lhs_sig.field_count() != rhs_sig.field_count()) {
        return syntax_expression_result_t{
            .expressions = void_spans,
            .value_or_type = u.get(builtin_eid::false_),
            .is_const_result = true
        };
    }
    for (size_t i = 0; i < lhs_sig.field_count(); ++i) {
        auto const& lhs_field = *lhs_sig.get_field(i);
        auto const& rhs_field = *rhs_sig.get_field(i);
        if (lhs_field.name() != rhs_field.name()) {
            return syntax_expression_result_t{
                .expressions = void_spans,
                .value_or_type = u.get(builtin_eid::false_),
                .is_const_result = true
            };
        }
    }

    fn_compiler_context_scope fn_scope{ ctx };

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

    // Helper lambda to append field value (const or non-const) for tuple fields
    auto append_tuple_field_value = [&](pure_call_t& call, const auto& field, size_t fidx, local_variable*& tuple_var, identifier& tuple_var_name, entity const& tuple_entity_type) -> error_storage {
        if (field.is_const()) {
            // Use the const entity directly
            call.emplace_back(annotated_entity_identifier{ field.entity_id(), md.call_location });
        } else {
            if (!tuple_var) {
                tuple_var_name = u.new_identifier();
                tuple_var = &fn_scope.new_temporary(tuple_var_name, tuple_entity_type.id);
            }
            pure_call_t get_call{ md.call_location };
            get_call.emplace_back(annotated_identifier{ u.get(builtin_id::self), md.call_location },
                variable_reference{ annotated_qname{ qname{ tuple_var_name, false } }, false });
            get_call.emplace_back(annotated_identifier{ u.get(builtin_id::property) }, annotated_integer{ mp::integer{ fidx } });
            auto match = ctx.find(builtin_qnid::get, get_call, el);
            if (!match) {
                return append_cause(
                    make_error<basic_general_error>(md.call_location, "internal error: can't get tuple element"sv, annotated_integer{ mp::integer{ fidx } }),
                    std::move(match.error()));
            }
            auto res = match->apply(ctx);
            if (!res) {
                return append_cause(
                    make_error<basic_general_error>(md.call_location, "internal error: can't get tuple element"sv, annotated_integer{ mp::integer{ fidx } }),
                    std::move(res.error()));
            }
            result.stored_expressions = el.concat(result.stored_expressions, res->stored_expressions);
            semantic::managed_expression_list mel{ u };
            mel.deep_copy(res->expressions);
            call.emplace_back(indirect_value{
                .location = md.call_location,
                .type = field.entity_id(),
                .store = indirect_value_store_t{ in_place_type<semantic::indirect_expression_list>, std::move(mel) }
            });
        }
        return {};
    };

    semantic::expression_span* current_branch_owner_code = nullptr;

    // Helper lambda to compare a single tuple field and update result accordingly
    auto compare_tuple_field = [&](size_t i) -> std::expected<bool, error_storage> {
        auto const& lhs_field = lhs_sig.field(i);
        auto const& rhs_field = rhs_sig.field(i);

        // If both fields are the same const entity, skip comparison
        if (lhs_field.entity_id() == rhs_field.entity_id() && lhs_field.is_const() == rhs_field.is_const() && lhs_field.is_const()) {
            return true;
        }

        pure_call_t eq_call{ md.call_location };

        // Append lhs field value
        if (auto lhs_err = append_tuple_field_value(eq_call, lhs_field, i, lhs_tuple_var, lhs_tuple_var_name, eq_md.lhs_entity_type)) {
            // Add field name and index to error
            return std::unexpected(append_cause(
                make_error<basic_general_error>(md.call_location, ("failed to get lhs tuple field(%1%) value"_fmt % i).str(), lhs_field.name()),
                std::move(lhs_err)));
        }

        // Append rhs field value
        if (auto rhs_err = append_tuple_field_value(eq_call, rhs_field, i, rhs_tuple_var, rhs_tuple_var_name, eq_md.rhs_entity_type)) {
            // Add field name and index to error
            return std::unexpected(append_cause(
                make_error<basic_general_error>(md.call_location, ("failed to get rhs tuple field(%1%) value"_fmt % i).str(), rhs_field.name()),
                std::move(rhs_err)));
        }

        // Compare fields using builtin_qnid::eq
        auto match = ctx.find(builtin_qnid::eq, eq_call, el, expected_result_t{ .type = u.get(builtin_eid::boolean) });
        if (!match) {
            return std::unexpected(append_cause(
                make_error<binary_relation_error>(md.call_location, ("can't compare tuple fields at index %1%"_fmt % i).str(), lhs_field.entity_id(), rhs_field.entity_id()),
                std::move(match.error())));
        }
        auto eq_res = match->apply(ctx);
        if (!eq_res) {
            return std::unexpected(append_cause(
                make_error<binary_relation_error>(md.call_location, ("can't compare tuple fields at index %1%"_fmt % i).str(), lhs_field.entity_id(), rhs_field.entity_id()),
                std::move(eq_res.error())));
        }

        // If the result is constant, optimize
        if (eq_res->is_const_result) {
            if (eq_res->value() == u.get(builtin_eid::true_)) {
                return true; // This field is equal, skip it
            } else {
                BOOST_ASSERT(eq_res->value() == u.get(builtin_eid::false_)); // NOLINT
                // If any field is not equal, the whole tuple is not equal
                result = syntax_expression_result_t{
                    .expressions = void_spans,
                    .value_or_type = u.get(builtin_eid::false_),
                    .is_const_result = true
                };
                return false;
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

            // store branch expressions as stored expressions if they are not primary branch expressions
            if (&result.expressions != current_branch_owner_code) {
                result.stored_expressions = el.concat(result.stored_expressions, *current_branch_owner_code);
            }

            current_branch_owner_code = &cond.true_branch;
        } else {
            // Otherwise, we need to create a new conditional expression
            result.expressions = el.concat(result.expressions, eq_res->expressions);
            current_branch_owner_code = &result.expressions;
        }
        return true;
    };

    // For each field, compare using builtin_qnid::eq
    // Returns true only if all fields are equal, otherwise returns false at first mismatch
    for (size_t i = 0; i < lhs_sig.field_count(); ++i) {
        auto cmp = compare_tuple_field(i);
        if (!cmp) {
            return std::unexpected(std::move(cmp.error()));
        }
        if (!cmp.value()) {
            // Early exit: a field is not equal
            return std::move(result);
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

    // Store any remaining branch expressions
    if (&result.expressions != current_branch_owner_code) {
        result.stored_expressions = el.concat(result.stored_expressions, *current_branch_owner_code);
    }

    if (lhs_tuple_var) {
        result.temporaries.emplace_back(lhs_tuple_var_name, std::move(*lhs_tuple_var), lhs_er.expressions);
    }
    if (rhs_tuple_var) {
        result.temporaries.emplace_back(rhs_tuple_var_name, std::move(*rhs_tuple_var), rhs_er.expressions);
    }
    return std::move(result);
}

} // namespace sonia::lang::bang
