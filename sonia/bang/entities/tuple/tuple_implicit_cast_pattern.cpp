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

class tuple_implicit_cast_match_descriptor : public functional_match_descriptor
{
public:
    inline explicit tuple_implicit_cast_match_descriptor(prepared_call const& call)
        : functional_match_descriptor{ call }
        , src_entity_type{ nullptr }
        , dst_sig{ nullptr }
    {}

    inline tuple_implicit_cast_match_descriptor(prepared_call const& call, entity const& src, entity_signature const& dst) noexcept
        : functional_match_descriptor{ call }
        , src_entity_type{ &src }
        , dst_sig{ &dst }
    {}

    entity const* src_entity_type;
    
    entity_signature const* dst_sig;
};

std::expected<functional_match_descriptor_ptr, error_storage>
tuple_implicit_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    if (!exp.type) {
        return std::unexpected(make_error<basic_general_error>(call.location, "missing destination type for implicit cast"sv));
    }
    unit& u = ctx.u();
    entity const& dest_type_entity = get_entity(u, exp.type);
    auto* dst_sig = dest_type_entity.signature();
    if (!dst_sig || dst_sig->name != u.get(builtin_qnid::tuple)) {
        return std::unexpected(make_error<type_mismatch_error>(call.location, exp.type, "a tuple"sv));
    }

    auto call_session = call.new_session(ctx);
    // Get (source tuple)
    std::pair<syntax_expression_t const*, size_t> self_expr;
    auto src_arg = call_session.use_next_positioned_argument(&self_expr);
    if (!src_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument"sv));
    }

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    // Both must be tuple types
    entity_identifier src_type;
    syntax_expression_result_t& src_arg_er = src_arg->first;
    if (src_arg_er.is_const_result) {
        entity const& src_entity = get_entity(u, src_arg_er.value());
        src_type = src_entity.get_type();
    } else {
        src_type = src_arg_er.type();
    }

    if (src_type == exp.type) {
        // No cast needed, just return the source as is
        auto pmd = make_shared<tuple_implicit_cast_match_descriptor>(call);
        pmd->emplace_back(0, src_arg_er);
        pmd->void_spans = std::move(call_session.void_spans);
        return pmd;
    }

    entity const& src_entity_type = get_entity(u, src_type);
    auto* src_sig = src_entity_type.signature();

    if (!src_sig || src_sig->name != u.get(builtin_qnid::tuple)) {
        return std::unexpected(make_error<type_mismatch_error>(call.location, src_entity_type.id, "a tuple"sv));
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

    auto pmd = make_shared<tuple_implicit_cast_match_descriptor>(call, src_entity_type, *dst_sig); // ?location = get_start_location(*pself_expr))
    pmd->emplace_back(0, src_arg_er);
    pmd->void_spans = std::move(call_session.void_spans);
    pmd->signature.result.emplace(exp.type, exp.can_be_only_constexpr());
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage>
tuple_implicit_cast_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    tuple_implicit_cast_match_descriptor& self_md = static_cast<tuple_implicit_cast_match_descriptor&>(md);
    auto& [_, src_er] = md.matches.front();

    if (!self_md.src_entity_type) {
        src_er.expressions = el.concat(md.merge_void_spans(el), src_er.expressions);
        return std::move(src_er);
    }
    entity_signature const& src_sig = *self_md.src_entity_type->signature();
    fn_compiler_context_scope fn_scope{ ctx };

    semantic::expression_span fn_code = md.merge_void_spans(el);
    local_variable* src_tuple_var = nullptr;
    identifier src_tuple_var_name;
    size_t mut_field_count = 0;
    for (size_t i = 0; i < src_sig.field_count(); ++i) {
        auto const& src_field = src_sig.field(i);
        auto const& dst_field = self_md.dst_sig->field(i);

        if (!dst_field.is_const()) {
            ++mut_field_count;
        }

        if (src_field.entity_id() == dst_field.entity_id() && src_field.is_const() == dst_field.is_const() && src_field.is_const()) {
            continue; // No cast needed, boath are const and same type
        }

        pure_call_t cast_call{ md.call_location };
        if (src_field.is_const()) {
            cast_call.emplace_back(annotated_entity_identifier{ src_field.entity_id(), md.call_location });
        } else {
            if (!src_tuple_var) {
                src_tuple_var_name = u.new_identifier();
                src_tuple_var = &fn_scope.new_temporary(src_tuple_var_name, self_md.src_entity_type->id);
            }

            pure_call_t get_call{ md.call_location };
            get_call.emplace_back(annotated_identifier{ u.get(builtin_id::self), md.call_location },
                variable_reference{ annotated_qname{ qname{ src_tuple_var_name, false } }, false });
            get_call.emplace_back(annotated_identifier{ u.get(builtin_id::property) }, annotated_integer{ mp::integer{ i } });
            auto match = ctx.find(builtin_qnid::get, get_call, el);
            if (!match) {
                return std::unexpected(append_cause(
                    make_error<basic_general_error>(md.call_location, "internal error: can't get tuple element"sv, annotated_integer{ mp::integer{ i } }),
                    std::move(match.error())));
            }
            auto res = match->apply(ctx);
            if (!res) {
                return std::unexpected(append_cause(
                    make_error<basic_general_error>(md.call_location, "internal error: can't get tuple element"sv, annotated_integer{ mp::integer{ i } }),
                    std::move(res.error())));
            }

            if (src_field.entity_id() == dst_field.entity_id() && !dst_field.is_const()) {
                fn_code = el.concat(fn_code, res->expressions);
                continue; // No cast needed
            }
            semantic::managed_expression_list el{ u };
            el.deep_copy(res->expressions);
            cast_call.emplace_back(indirect_value{
                .location = md.call_location,
                .type = src_field.entity_id(),
                .store = indirect_value_store_t{ in_place_type<semantic::indirect_expression_list>, std::move(el) }
            });
        }
        entity_identifier dest_field_type = dst_field.is_const() ? get_entity(u, dst_field.entity_id()).id : dst_field.entity_id();
        auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, el, expected_result_t{ .type = dest_field_type, .modifier  = dst_field.is_const() ? parameter_constraint_modifier_t::const_type : parameter_constraint_modifier_t::const_or_runtime_type });
        if (!match) {
            return std::unexpected(append_cause(
                make_error<cast_error>(md.call_location, dest_field_type, src_field.entity_id()),
                std::move(match.error())));
        }
        auto fld_res = match->apply(ctx);
        if (!fld_res) {
            return std::unexpected(append_cause(
                make_error<basic_general_error>(md.call_location, "internal error: can't apply implicit cast for tuple field"sv, src_field.entity_id()),
                std::move(fld_res.error())));
        }
        fn_code = el.concat(fn_code, fld_res->expressions);
    }

    if (mut_field_count > 1) {
        u.push_back_expression(el, fn_code, semantic::push_value{ mp::integer{ mut_field_count } });
        u.push_back_expression(el, fn_code, semantic::invoke_function(u.get(builtin_eid::arrayify)));
    }
    if (src_tuple_var) {
        src_er.temporaries.emplace_back(src_tuple_var_name, std::move(*src_tuple_var), src_er.expressions);
    }

    src_er.expressions = fn_code;
    src_er.value_or_type = self_md.signature.result->entity_id();
    src_er.is_const_result = !mut_field_count;

    return std::move(src_er);
}

} // namespace sonia::lang::bang
