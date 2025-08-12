//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "runtime_cast_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> runtime_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    auto call_session = call.new_session(ctx);
    auto arg = call_session.use_next_positioned_argument(expected_result_t{ .type = exp.type, .location = call.location, .modifier = value_modifier_t::runtime_value });
    if (!arg) return std::unexpected(arg.error());
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    auto pmd = make_shared<functional_match_descriptor>(call);
    pmd->emplace_back(0, arg->first);
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage> runtime_cast_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    auto & [_, ser, loc] = md.matches.front();

    BOOST_ASSERT(!ser.is_const_result);
    //if (ser.is_const_result) {
    //    ctx.u().push_back_expression(el, ser.expressions, semantic::push_value{ ser.value() });
    //    ser.value_or_type = get_entity(ctx.u(), ser.value()).get_type();
    //    ser.is_const_result = false;
    //}
    return std::move(ser);
}

}
