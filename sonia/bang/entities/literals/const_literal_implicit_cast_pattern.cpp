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

namespace sonia::lang::bang {

struct const_literal_argument_visitor : entity_visitor_adapter
{
    mutable variant<std::nullptr_t, integer_literal_entity, decimal_literal_entity, string_literal_entity> value;

    void operator()(integer_literal_entity const& e) const override { value = e; }
    void operator()(decimal_literal_entity const& e) const override { value = e; }
    void operator()(string_literal_entity const& e) const override { value = e; }
};

class const_literal_implicit_cast_match_descriptor : public functional_match_descriptor
{
public:
    using functional_match_descriptor::functional_match_descriptor;

    template <typename ArgT>
    explicit const_literal_implicit_cast_match_descriptor(ArgT && arg)
        : arg{ std::forward<ArgT>(arg) }
    {}

    variant<std::nullptr_t, integer_literal_entity, decimal_literal_entity, string_literal_entity> arg;
};

std::expected<functional_match_descriptor_ptr, error_storage>
const_literal_implicit_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    if (!exp || exp.is_const_result) {
        return std::unexpected(make_error<basic_general_error>(call.location, "expected a runtime literal result"sv));
    }
    
    if (exp.type != ctx.u().get(builtin_eid::integer) && exp.type != ctx.u().get(builtin_eid::decimal) &&
        exp.type != ctx.u().get(builtin_eid::string) &&
        exp.type != ctx.u().get(builtin_eid::f16) &&
        exp.type != ctx.u().get(builtin_eid::f32) &&
        exp.type != ctx.u().get(builtin_eid::f64) &&
        exp.type != ctx.u().get(builtin_eid::boolean))
    {
        return std::unexpected(make_error<type_mismatch_error>(call.location, exp.type, "a literal type"sv));
    }
    unit& u = ctx.u();

    auto call_session = call.new_session(ctx);
    syntax_expression_t const* pself_expr;
    auto src_arg = call_session.use_next_positioned_argument(&pself_expr);
    if (!src_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument"sv));
    }
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    // Only allow constant arguments
    if (!src_arg->is_const_result) {
        return std::unexpected(make_error<basic_general_error>(get_start_location(*pself_expr), "argument must be a constant literal"sv));
    }
    entity const& src_arg_entity = get_entity(u, src_arg->value());
    if (src_arg_entity.get_type() != exp.type) {
        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*pself_expr), src_arg->value(), exp.type));
    }
    //// string to string check
    //if (exp.type == u.get(builtin_eid::string)) {
    //    if (src_arg_entity.get_type() == u.get(builtin_eid::string)) {
    //        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*pself_expr), src_arg->value(), exp.type));
    //    }
    //} else if (src_arg_entity.get_type() == u.get(builtin_eid::string)) {
    //    return std::unexpected(make_error<type_mismatch_error>(get_start_location(*pself_expr), src_arg->value(), exp.type));
    //}

    const_literal_argument_visitor vis;
    src_arg_entity.visit(vis);
    if (vis.value.which() == 0) {
        return std::unexpected(make_error<value_mismatch_error>(get_start_location(*pself_expr), src_arg->value(), "a literal"sv));
    }
    
    auto pmd = sonia::make_shared<const_literal_implicit_cast_match_descriptor>(std::move(vis.value));
    pmd->result = field_descriptor{ exp.type, false };
    pmd->get_match_result(0).append_result(*src_arg);
    pmd->void_spans = std::move(call_session.void_spans);
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage>
const_literal_implicit_cast_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    auto& nmd = static_cast<const_literal_implicit_cast_match_descriptor&>(md);
    auto& src = md.get_match_result(0).results.front();
    src.expressions = el.concat(md.merge_void_spans(el), src.expressions);
    BOOST_ASSERT(nmd.arg.which());
    src.value_or_type = nmd.result.entity_id();
    src.is_const_result = false;
    apply_visitor(make_functional_visitor<void>([&u = ctx.u(), &ctx, &el, &src](auto const& v) {
        if constexpr (std::is_same_v<integer_literal_entity, std::decay_t<decltype(v)>>) {
            BOOST_ASSERT(src.type() == u.get(builtin_eid::integer));
            u.push_back_expression(el, src.expressions, semantic::push_value{ v.value() });
        } else if constexpr (std::is_same_v<decimal_literal_entity, std::decay_t<decltype(v)>>) {
            BOOST_ASSERT(src.type() == u.get(builtin_eid::decimal));
            u.push_back_expression(el, src.expressions, semantic::push_value{ v.value() });
        } else if constexpr (std::is_same_v<string_literal_entity, std::decay_t<decltype(v)>>) {
            BOOST_ASSERT(src.type() == u.get(builtin_eid::string));
            u.push_back_expression(el, src.expressions, semantic::push_value{ v.value() });
        } else {
            THROW_INTERNAL_ERROR("numeric_implicit_cast_pattern::apply, null is not expected");
        }
    }), nmd.arg);
    return std::move(src);
    //
    //entity_identifier rid = apply_visitor(make_functional_visitor<entity_identifier>([&ctx, type = nmd.result.entity_id()](auto const& v) -> entity_identifier {
    //    if constexpr (std::is_same_v<integer_literal_entity, std::decay_t<decltype(v)>>) {
    //        BOOST_ASSERT(type == ctx.u().get(builtin_eid::integer));
    //        return ctx.u().make_integer_entity(v.value(), type).id;
    //    } else if constexpr (std::is_same_v<decimal_literal_entity, std::decay_t<decltype(v)>>) {
    //        BOOST_ASSERT(type == ctx.u().get(builtin_eid::decimal));
    //        return ctx.u().make_decimal_entity(v.value(), type).id;
    //    } else if constexpr (std::is_same_v<string_literal_entity, std::decay_t<decltype(v)>>) {
    //        BOOST_ASSERT(type == ctx.u().get(builtin_eid::string));
    //        return ctx.u().make_string_entity(v.value(), type).id;
    //    } else {
    //        THROW_INTERNAL_ERROR("const_literal_implicit_cast_pattern::apply, null is not expected");
    //    }
    //}), nmd.arg);
    //src.value_or_type = rid;
    //src.is_const_result = false;
    //return std::move(src);
}

} // namespace sonia::lang::bang
