//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "numeric_implicit_cast_pattern.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/base_expression_visitor.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/errors/value_mismatch_error.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

struct argument_wrapper_visitor : entity_visitor_adapter
{
    mutable variant<std::nullptr_t, integer_literal_entity, decimal_literal_entity> value;

    void operator()(integer_literal_entity const& e) const override { value = e; }
    void operator()(decimal_literal_entity const& e) const override { value = e; }
};

class numeric_implicit_cast_match_descriptor : public functional_match_descriptor
{
public:
    using functional_match_descriptor::functional_match_descriptor;

    template <typename ArgT>
    explicit numeric_implicit_cast_match_descriptor(ArgT && arg)
        : arg{ std::forward<ArgT>(arg) }
    {}

    variant<std::nullptr_t, integer_literal_entity, decimal_literal_entity> arg;
};

std::expected<functional_match_descriptor_ptr, error_storage> numeric_implicit_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    unit& u = ctx.u();
    entity_identifier teid = exp.type;
    if (!teid) {
        return std::unexpected(make_error<basic_general_error>(call.location, "expected a runtime numeric result"sv));
    }

    int ntype = 0;

    if (teid == u.get(builtin_eid::f16)) { ntype = 1; }
    else if (teid == u.get(builtin_eid::f32)) { ntype = 2; }
    else if (teid == u.get(builtin_eid::f64)) { ntype = 3; }
    else if (teid == u.get(builtin_eid::decimal)) { ntype = 4; }
    else if (teid == u.get(builtin_eid::integer)) { ntype = 5; }
    else {
        return std::unexpected(make_error<basic_general_error>(call.location, "expected a numeric result"sv, exp.type));
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

    syntax_expression_result_t& src_arg_er = src_arg->first;
    // Only allow compatible argument
    if (!exp.is_modifier_compatible(src_arg_er)) {
        return std::unexpected(make_error<basic_general_error>(get_start_location(*get<0>(self_expr)), "argument and result must be both constexpr or both runtime"sv));
    }

    functional_match_descriptor_ptr pmd;
    if (src_arg_er.is_const_result) {
        entity const& argent = get_entity(u, src_arg_er.value());
        argument_wrapper_visitor vis;
        argent.visit(vis);
        if (vis.value.which() == 0) {
            return std::unexpected(make_error<value_mismatch_error>(get_start_location(*get<0>(self_expr)), src_arg_er.value(), "a numeric literal"sv));
        }
        if (argent.get_type() == teid) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(*get<0>(self_expr)), "argument and result types must be different"sv, teid));
        }
        pmd = sonia::make_shared<numeric_implicit_cast_match_descriptor>(std::move(vis.value));
    } else {
        if (src_arg_er.type() == teid) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(*get<0>(self_expr)), "argument and result types must be different"sv, teid));
        }
        if (src_arg_er.type() == u.get(builtin_eid::integer)) {
            pmd = make_shared<numeric_implicit_cast_match_descriptor>();
        } else {
            return std::unexpected(make_error<type_mismatch_error>(get_start_location(*get<0>(self_expr)), src_arg_er.type(), "integer"sv));
        }
    }
    pmd->emplace_back(0, src_arg_er);
    pmd->void_spans = std::move(call_session.void_spans);
    pmd->signature.result.emplace(exp.type, exp.can_be_only_constexpr());
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage> numeric_implicit_cast_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    auto& nmd = static_cast<numeric_implicit_cast_match_descriptor&>(md);
    auto& [_, src] = md.matches.front();
    src.expressions = el.concat(md.merge_void_spans(el), src.expressions);
    if (nmd.arg.which()) { // not nullptr_t
        if (auto& result = *nmd.signature.result; result.is_const()) {
            entity_identifier rid = apply_visitor(make_functional_visitor<entity_identifier>([&ctx, type = result.entity_id()](auto const& v) -> entity_identifier {
                if constexpr (std::is_same_v<integer_literal_entity, std::decay_t<decltype(v)>>) {
                    return ctx.u().make_integer_entity(v.value(), type).id;
                } else if constexpr (std::is_same_v<decimal_literal_entity, std::decay_t<decltype(v)>>) {
                    return ctx.u().make_decimal_entity(v.value(), type).id;
                } else {
                    THROW_INTERNAL_ERROR("numeric_implicit_cast_pattern::apply, null is not expected");
                }
            }), nmd.arg);
            src.value_or_type = rid;
            src.is_const_result = true;
            return syntax_expression_result_t {
                .temporaries = std::move(src.temporaries),
                .expressions = md.merge_void_spans(el),
                .value_or_type = rid,
                .is_const_result = true
            };
        } else {
            apply_visitor(make_functional_visitor<void>([&ctx, &el, &src](auto const& v) {
                if constexpr (std::is_same_v<integer_literal_entity, std::decay_t<decltype(v)>>) {
                    ctx.u().push_back_expression(el, src.expressions, semantic::push_value{ v.value() });
                } else if constexpr (std::is_same_v<decimal_literal_entity, std::decay_t<decltype(v)>>) {
                    ctx.u().push_back_expression(el, src.expressions, semantic::push_value{ v.value() });
                } else {
                    THROW_INTERNAL_ERROR("numeric_implicit_cast_pattern::apply, null is not expected");
                }
            }), nmd.arg);
            src.value_or_type = result.entity_id(),
            src.is_const_result = false;
        }
        return std::move(src);
        //entity_identifier rid = (ctx.u(), apply_visitor(make_functional_visitor<entity_identifier>([&ctx, type = nmd.result.entity_id()](auto const& v) -> entity_identifier {
        //    if constexpr (std::is_same_v<integer_literal_entity, std::decay_t<decltype(v)>>) {
        //        return ctx.u().make_integer_entity(v.value(), type).id;
        //    } else if constexpr (std::is_same_v<decimal_literal_entity, std::decay_t<decltype(v)>>) {
        //        return ctx.u().make_decimal_entity(v.value(), type).id;
        //    } else {
        //        THROW_INTERNAL_ERROR("numeric_implicit_cast_pattern::apply, null is not expected");
        //    }
        //}), nmd.arg));
        
    }
    THROW_NOT_IMPLEMENTED_ERROR("numeric_implicit_cast_pattern::apply");
}

}
