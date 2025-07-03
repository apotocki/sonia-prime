//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "const_literal_implicit_cast_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/base_expression_visitor.hpp"
#include "sonia/bang/errors/value_mismatch_error.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"
#include "sonia/utility/invocation/invocation.hpp"

namespace sonia::lang::bang {

struct const_literal_argument_visitor : entity_visitor_adapter
{
    mutable variant<std::nullptr_t, integer_literal_entity, decimal_literal_entity, generic_literal_entity> value;

    void operator()(integer_literal_entity const& e) const override { value = e; }
    void operator()(decimal_literal_entity const& e) const override { value = e; }
    void operator()(generic_literal_entity const& e) const override
    {
        switch (static_cast<builtin_eid>(e.get_type().value)) {
        case builtin_eid::boolean:
        case builtin_eid::integer:
        case builtin_eid::decimal:
        case builtin_eid::string:
            value = e;
            break;
        default:
            break;
        }
    }
};

class const_literal_implicit_cast_match_descriptor : public functional_match_descriptor
{
public:
    using functional_match_descriptor::functional_match_descriptor;

    template <typename ArgT>
    const_literal_implicit_cast_match_descriptor(prepared_call const& call, ArgT && arg)
        : functional_match_descriptor{ call }
        , arg{ std::forward<ArgT>(arg) }
    {}

    variant<std::nullptr_t, integer_literal_entity, decimal_literal_entity, generic_literal_entity> arg;
};

std::expected<functional_match_descriptor_ptr, error_storage>
const_literal_implicit_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    if (can_be_only_constexpr(exp.modifier)) {
        return std::unexpected(make_error<basic_general_error>(call.location, "expected a runtime literal result"sv));
    }
    
    unit& u = ctx.u();

    if (exp.type && exp.type != u.get(builtin_eid::any) &&
        exp.type != u.get(builtin_eid::integer) && exp.type != ctx.u().get(builtin_eid::decimal) &&
        exp.type != u.get(builtin_eid::string) &&
        exp.type != u.get(builtin_eid::f16) &&
        exp.type != u.get(builtin_eid::f32) &&
        exp.type != u.get(builtin_eid::f64) &&
        exp.type != u.get(builtin_eid::boolean) &&
        exp.type != u.get(builtin_eid::i8) &&
        exp.type != u.get(builtin_eid::u8) &&
        exp.type != u.get(builtin_eid::i16) &&
        exp.type != u.get(builtin_eid::u16) &&
        exp.type != u.get(builtin_eid::i32) &&
        exp.type != u.get(builtin_eid::u32) &&
        exp.type != u.get(builtin_eid::i64) &&
        exp.type != u.get(builtin_eid::u64))
    {
        return std::unexpected(make_error<type_mismatch_error>(call.location, exp.type, "a literal type"sv));
    }
    
    auto call_session = call.new_session(ctx);
    std::pair<syntax_expression_t const*, size_t> self_expr;
    auto src_arg = call_session.use_next_positioned_argument(&self_expr);
    if (!src_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument"sv));
    }
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    // Only allow constant arguments
    syntax_expression_result_t & src_arg_er = src_arg->first;
    if (!src_arg_er.is_const_result) {
        return std::unexpected(make_error<basic_general_error>(get_start_location(*get<0>(self_expr)), "argument must be a constant literal"sv));
    }
    entity const& src_arg_entity = get_entity(u, src_arg_er.value());
    
    // string to string check
    if (exp.type == u.get(builtin_eid::string)) {
        if (src_arg_entity.get_type() != u.get(builtin_eid::string)) {
            return std::unexpected(make_error<type_mismatch_error>(get_start_location(*get<0>(self_expr)), src_arg_entity.get_type(), exp.type));
        }
    } else if (src_arg_entity.get_type() == u.get(builtin_eid::string) && exp.type != u.get(builtin_eid::any)) {
        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*get<0>(self_expr)), src_arg_entity.get_type(), exp.type));
    }

    const_literal_argument_visitor vis;
    src_arg_entity.visit(vis);
    if (vis.value.which() == 0) {
        return std::unexpected(make_error<value_mismatch_error>(get_start_location(*get<0>(self_expr)), src_arg_er.value(), "a literal"sv));
    }
    
    auto pmd = sonia::make_shared<const_literal_implicit_cast_match_descriptor>(call, std::move(vis.value));
    pmd->signature.result.emplace(exp.type ? exp.type : src_arg_entity.get_type(), false);
    pmd->emplace_back(0, src_arg_er);
    pmd->void_spans = std::move(call_session.void_spans);
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage>
const_literal_implicit_cast_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    auto& nmd = static_cast<const_literal_implicit_cast_match_descriptor&>(md);
    auto& [_, src] = md.matches.front();
    src.expressions = el.concat(md.merge_void_spans(el), src.expressions);
    BOOST_ASSERT(nmd.arg.which());
    
    unit& u = ctx.u();
    entity_identifier target_type = nmd.signature.result->entity_id();
    
    return apply_visitor(make_functional_visitor<std::expected<syntax_expression_result_t, error_storage>>([&](auto const& v) -> std::expected<syntax_expression_result_t, error_storage> {
        if constexpr (std::is_same_v<integer_literal_entity, std::decay_t<decltype(v)>>) {
            auto const& source_val = v.value();
            
            // Handle conversions to different numeric types using switch-case
            switch (static_cast<builtin_eid>(target_type.value)) {
            case builtin_eid::any:
            case builtin_eid::integer: {
                smart_blob result{ bigint_blob_result(source_val) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::i8: {
                if (!source_val.is_fit<int8_t>()) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "integer value cannot be converted to i8 without loss of precision"sv));
                }
                smart_blob result{ i8_blob_result(static_cast<int8_t>(source_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::u8: {
                if (!source_val.is_fit<uint8_t>() || source_val.sgn() < 0) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "integer value cannot be converted to u8 without loss of precision"sv));
                }
                smart_blob result{ ui8_blob_result(static_cast<uint8_t>(source_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::i16: {
                if (!source_val.is_fit<int16_t>()) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "integer value cannot be converted to i16 without loss of precision"sv));
                }
                smart_blob result{ i16_blob_result(static_cast<int16_t>(source_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::u16: {
                if (!source_val.is_fit<uint16_t>() || source_val.sgn() < 0) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "integer value cannot be converted to u16 without loss of precision"sv));
                }
                smart_blob result{ ui16_blob_result(static_cast<uint16_t>(source_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::i32: {
                if (!source_val.is_fit<int32_t>()) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "integer value cannot be converted to i32 without loss of precision"sv));
                }
                smart_blob result{ i32_blob_result(static_cast<int32_t>(source_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::u32: {
                if (!source_val.is_fit<uint32_t>() || source_val.sgn() < 0) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "integer value cannot be converted to u32 without loss of precision"sv));
                }
                smart_blob result{ ui32_blob_result(static_cast<uint32_t>(source_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::i64: {
                if (!source_val.is_fit<int64_t>()) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "integer value cannot be converted to i64 without loss of precision"sv));
                }
                smart_blob result{ i64_blob_result(static_cast<int64_t>(source_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::u64: {
                if (!source_val.is_fit<uint64_t>() || source_val.sgn() < 0) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "integer value cannot be converted to u64 without loss of precision"sv));
                }
                smart_blob result{ ui64_blob_result(static_cast<uint64_t>(source_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::f16: {
                smart_blob result{ f16_blob_result(sonia::float16{static_cast<float>(source_val)}) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::f32: {
                smart_blob result{ f32_blob_result(static_cast<float>(source_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::f64: {
                smart_blob result{ f64_blob_result(static_cast<double_t>(source_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::decimal: {
                smart_blob result{ decimal_blob_result(mp::decimal_view{source_val, 0}) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            default:
                return std::unexpected(make_error<type_mismatch_error>(md.call_location, target_type, "a numeric type"sv));
            }
        } else if constexpr (std::is_same_v<decimal_literal_entity, std::decay_t<decltype(v)>>) {
            auto const& source_val = v.value();
            
            // Handle conversions from decimal to various types using switch-case
            switch (static_cast<builtin_eid>(target_type.value)) {
            case builtin_eid::any:
            case builtin_eid::decimal: {
                smart_blob result{ decimal_blob_result(source_val) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::i8: {
                // Check if decimal can be converted to integer without fractional part
                if (source_val.exponent().sgn() < 0) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value has fractional part and cannot be converted to i8"sv));
                }
                auto const& integral_val = source_val.significand();
                if (!integral_val.is_fit<int8_t>()) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value cannot be converted to i8 without loss of precision"sv));
                }
                smart_blob result{ i8_blob_result(static_cast<int8_t>(integral_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::u8: {
                if (source_val.exponent().sgn() < 0) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value has fractional part and cannot be converted to u8"sv));
                }
                auto const& integral_val = source_val.significand();
                if (!integral_val.is_fit<uint8_t>() || source_val.is_negative()) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value cannot be converted to u8 without loss of precision"sv));
                }
                smart_blob result{ ui8_blob_result(static_cast<uint8_t>(integral_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::i16: {
                if (source_val.exponent().sgn() < 0) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value has fractional part and cannot be converted to i16"sv));
                }
                auto const& integral_val = source_val.significand();
                if (!integral_val.is_fit<int16_t>()) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value cannot be converted to i16 without loss of precision"sv));
                }
                smart_blob result{ i16_blob_result(static_cast<int16_t>(integral_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::u16: {
                if (source_val.exponent().sgn() < 0) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value has fractional part and cannot be converted to u16"sv));
                }
                auto const& integral_val = source_val.significand();
                if (!integral_val.is_fit<uint16_t>() || source_val.is_negative()) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value cannot be converted to u16 without loss of precision"sv));
                }
                smart_blob result{ ui16_blob_result(static_cast<uint16_t>(integral_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::i32: {
                if (source_val.exponent().sgn() < 0) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value has fractional part and cannot be converted to i32"sv));
                }
                auto const& integral_val = source_val.significand();
                if (!integral_val.is_fit<int32_t>()) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value cannot be converted to i32 without loss of precision"sv));
                }
                smart_blob result{ i32_blob_result(static_cast<int32_t>(integral_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::u32: {
                if (source_val.exponent().sgn() < 0) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value has fractional part and cannot be converted to u32"sv));
                }
                auto const& integral_val = source_val.significand();
                if (!integral_val.is_fit<uint32_t>() || source_val.is_negative()) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value cannot be converted to u32 without loss of precision"sv));
                }
                smart_blob result{ ui32_blob_result(static_cast<uint32_t>(integral_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::i64: {
                if (source_val.exponent().sgn() < 0) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value has fractional part and cannot be converted to i64"sv));
                }
                auto const& integral_val = source_val.significand();
                if (!integral_val.is_fit<int64_t>()) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value cannot be converted to i64 without loss of precision"sv));
                }
                smart_blob result{ i64_blob_result(static_cast<int64_t>(integral_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::u64: {
                if (source_val.exponent().sgn() < 0) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value has fractional part and cannot be converted to u64"sv));
                }
                auto const& integral_val = source_val.significand();
                if (!integral_val.is_fit<uint64_t>() || source_val.is_negative()) {
                    return std::unexpected(make_error<basic_general_error>(md.call_location, "decimal value cannot be converted to u64 without loss of precision"sv));
                }
                smart_blob result{ ui64_blob_result(static_cast<uint64_t>(integral_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::f16: {
                smart_blob result{ f16_blob_result(sonia::float16{static_cast<float>(source_val)}) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::f32: {
                smart_blob result{ f32_blob_result(static_cast<float>(source_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            case builtin_eid::f64: {
                smart_blob result{ f64_blob_result(static_cast<double>(source_val)) };
                u.push_back_expression(el, src.expressions, semantic::push_value{ std::move(result) });
                break;
            }
            default:
                return std::unexpected(make_error<type_mismatch_error>(md.call_location, target_type, "a numeric type"sv));
            }
        } else if constexpr (std::is_same_v<generic_literal_entity, std::decay_t<decltype(v)>>) {
            switch (static_cast<builtin_eid>(v.get_type().value)) {
            case builtin_eid::boolean:
                u.push_back_expression(el, src.expressions, semantic::push_value{ smart_blob(v.value()) });
                break;
            case builtin_eid::string:
                u.push_back_expression(el, src.expressions, semantic::push_value{ smart_blob(v.value()) });
                break;
            default:
                return std::unexpected(make_error<type_mismatch_error>(md.call_location, target_type, "a literal type"sv));
            }
        } else {
            return std::unexpected(make_error<basic_general_error>(md.call_location, "const_literal_implicit_cast_pattern::apply, null is not expected"sv));
        }
        
        src.is_const_result = false;
        src.value_or_type = target_type;
        return std::move(src);
    }), nmd.arg);
}

} // namespace sonia::lang::bang
