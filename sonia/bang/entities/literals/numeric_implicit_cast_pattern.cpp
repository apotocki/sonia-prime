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

std::expected<functional_match_descriptor_ptr, error_storage> numeric_implicit_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& e) const
{
    unit& u = ctx.u();
    if (!e) {
        return std::unexpected(make_error<basic_general_error>(call.location, "expected a numeric result"sv));
    }
    entity_identifier teid = e.value;
    int ntype = 0;
    if (teid == u.get(builtin_eid::f16)) { ntype = 1; }
    else if (teid == u.get(builtin_eid::f32)) { ntype = 2; }
    else if (teid == u.get(builtin_eid::f64)) { ntype = 3; }
    else if (teid == u.get(builtin_eid::decimal)) { ntype = 4; }
    else {
        return std::unexpected(make_error<basic_general_error>(call.location, "expected a numeric result"sv, e));
    }

    functional_match_descriptor_ptr pmd;

    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        auto const& argexpr = arg.value();
        if (pargname) { // named arguments are not expected
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch"sv, argexpr));
        } else if (pmd) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "argument mismatch"sv, argexpr));
        }
        auto res = apply_visitor(base_expression_visitor{ ctx }, argexpr);
        if (!res) return std::unexpected(std::move(res.error()));
        auto& ser = res->first;
        if (ser.is_const_result) {
            entity const& argent = u.eregistry_get(ser.value());
            argument_wrapper_visitor vis;
            argent.visit(vis);
            if (vis.value.which() == 0) {
                return std::unexpected(make_error<value_mismatch_error>(get_start_location(argexpr), ser.value(), "a numeric literal"sv));
            }
            pmd = make_shared<numeric_implicit_cast_match_descriptor>(std::move(vis.value));
            pmd->result = field_descriptor{ e.value, true };

            //integer_literal_entity
        } else {
            if (ser.type() == u.get(builtin_eid::integer)) {
                pmd = make_shared<numeric_implicit_cast_match_descriptor>();
                pmd->get_match_result(0).append_result(ser.type(), ser.expressions);
                call.splice_back(ser.expressions);
                pmd->result = field_descriptor{ e.value, false };
            } else {
                return std::unexpected(make_error<type_mismatch_error>(get_start_location(argexpr), ser.type(), "integer"sv));
            }
        }
    }

    if (!pmd) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter"sv));
    }
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> numeric_implicit_cast_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t&, functional_match_descriptor& md) const
{
    auto& nmd = static_cast<numeric_implicit_cast_match_descriptor&>(md);
    if (nmd.result.is_const()) {
        return make_result(ctx.u(), apply_visitor(make_functional_visitor<entity_identifier>([&ctx, type = nmd.result.entity_id()](auto const& v) -> entity_identifier {
            if constexpr (std::is_same_v<integer_literal_entity, std::decay_t<decltype(v)>>) {
                return ctx.u().make_integer_entity(v.value(), type).id();
            } else if constexpr (std::is_same_v<decimal_literal_entity, std::decay_t<decltype(v)>>) {
                return ctx.u().make_decimal_entity(v.value(), type).id();
            } else {
                THROW_INTERNAL_ERROR("numeric_implicit_cast_pattern::apply, null is not expected");
            }
        }), nmd.arg));
    }
    THROW_NOT_IMPLEMENTED_ERROR("numeric_implicit_cast_pattern::apply");
}

}
