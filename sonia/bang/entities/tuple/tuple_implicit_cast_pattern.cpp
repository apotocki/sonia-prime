//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_implicit_cast_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage>
tuple_implicit_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& dest_type) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);

    if (!dest_type) {
        return std::unexpected(make_error<basic_general_error>(call.location, "missing destination type for implicit cast"sv));
    }
    entity const& dest_type_entity = get_entity(u, dest_type.type);
    auto* dst_sig = dest_type_entity.signature();
    if (!dst_sig || dst_sig->name != u.get(builtin_qnid::tuple)) {
        return std::unexpected(make_error<type_mismatch_error>(call.location, dest_type.type, "a tuple"sv));
    }

    // Get (source tuple)
    syntax_expression_t const* pself_expr;
    auto src_arg = call_session.use_next_positioned_argument(&pself_expr);
    if (!src_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument"sv));
    }

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    // Both must be tuple types
    entity const& src_entity = get_entity(u, src_arg->is_const_result ? src_arg->value() : src_arg->type());
    auto* src_sig = src_entity.signature();

    if (!src_sig || src_sig->name != u.get(builtin_qnid::tuple)) {
        return std::unexpected(make_error<type_mismatch_error>(call.location, src_entity.id, "a tuple"sv));
    }

    // Check field count and names
    if (src_sig->field_count() != dst_sig->field_count()) {
        return std::unexpected(make_error<basic_general_error>(call.location, "tuple field count mismatch"sv));
    }
    for (size_t i = 0; i < src_sig->field_count(); ++i) {
        auto const& src_field = *src_sig->get_field(i);
        auto const& dst_field = *dst_sig->get_field(i);
        if (src_field.name() != dst_field.name()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "tuple field name mismatch"sv));
        }
    }

#if 0
    // Try implicit cast for each field
    //local_variable* src_tuple_var = nullptr;
    //identifier src_tuple_var_name;
    //if (!src_tuple_var) {
    //    src_tuple_var_name = u.new_identifier();
    //    src_tuple_var = &cast_call.new_temporary(u, src_tuple_var_name, src_entity.id, src_arg->expressions);
    //}
    for (size_t i = 0; i < src_sig->field_count(); ++i) {
        auto const& src_field = *src_sig->get_field(i);
        auto const& dst_field = *dst_sig->get_field(i);
        if (src_field.entity_id() == dst_field.entity_id() && src_field.is_const() == dst_field.is_const()) {
            continue; // No cast needed
        }
        pure_call_t cast_call{ call.location };
        if (src_field.is_const()) {
            cast_call.emplace_back(annotated_entity_identifier{ src_field.entity_id(), call.location });
        } else {
            cast_call.emplace_back(indirect_value{
                .location = call.location,
                .type = src_field.entity_id(),
                .store = indirect_value_store_t{ in_place_type<semantic::indirect_expression_list>, semantic::managed_expression_list el{ u } }
            });
        }
        auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, call.expressions, dst_field.entity_id());

        get(builtin_qnid::implicit_cast); // Ensure implicit_cast is available
        if (!ctx.can_implicitly_cast(src_field.entity_id(), dst_field.entity_id())) {
            return std::unexpected(make_error<type_mismatch_error>(call.location, "tuple field type cannot be implicitly cast"));
        }
    }
#endif
    // All checks passed, create a match descriptor
    auto pmd = make_shared<functional_match_descriptor>(call.location);
    pmd->get_match_result(0).append_result(*src_arg);
    pmd->void_spans = std::move(call_session.void_spans);
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage>
tuple_implicit_cast_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    THROW_NOT_IMPLEMENTED_ERROR("tuple_implicit_cast_pattern::apply is not implemented yet");
#if 0
    unit& u = ctx.u();
    auto& self_er = md.get_match_result(0).results.front();
    auto& type_er = md.get_match_result(1).results.front();

    entity const& src_entity = get_entity(u, self_er.is_const_result ? self_er.value() : self_er.type());
    entity const& type_entity = get_entity(u, type_er.is_const_result ? type_er.value() : type_er.type());

    auto* src_sig = src_entity.signature();
    auto* dst_sig = type_entity.signature();

    std::vector<entity_identifier> casted_fields;
    std::vector<semantic::expression_span> field_exprs;
    std::vector<syntax_expression_result_t::temporary_t> temporaries = self_er.temporaries;

    // For each field, apply implicit cast
    for (size_t i = 0; i < src_sig->size(); ++i) {
        auto const& src_field = (*src_sig)[i];
        auto const& dst_field = (*dst_sig)[i];

        // Build a fake call to implicit_cast for this field
        // (Assume ctx has a helper for this, or use expression_implicit_cast_visitor)
        expression_implicit_cast_visitor cast_visitor(ctx, dst_field.entity_id());
        auto cast_result = cast_visitor.visit(src_field.entity_id());
        if (!cast_result) {
            return std::unexpected(make_error<type_mismatch_error>(md.location, "cannot implicitly cast tuple field"));
        }
        casted_fields.push_back(dst_field.entity_id());
        // For simplicity, skip collecting field_exprs here; in a real implementation, collect and merge expressions.
    }

    // Build the result tuple type
    entity_signature result_sig{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) };
    for (size_t i = 0; i < dst_sig->size(); ++i) {
        auto const& dst_field = (*dst_sig)[i];
        if (dst_field.name())
            result_sig.emplace_back(u.make_identifier_entity(dst_field.name()).id, true);
        result_sig.emplace_back(dst_field.entity_id(), dst_field.is_const());
    }
    entity_identifier result_type = u.make_basic_signatured_entity(std::move(result_sig)).id;

    // Merge expressions (simplified)
    semantic::expression_span exprs = md.merge_void_spans(el);
    exprs = el.concat(exprs, self_er.expressions);

    return syntax_expression_result_t{
        .temporaries = std::move(temporaries),
        .expressions = std::move(exprs),
        .value_or_type = result_type,
        .is_const_result = false
    };
#endif
}

} // namespace sonia::lang::bang
