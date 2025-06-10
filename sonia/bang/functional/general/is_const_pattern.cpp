//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "is_const_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> is_const_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    auto call_session = call.new_session(ctx);
    // Accept a single unnamed argument
    auto arg = call_session.use_next_positioned_argument();
    if (!arg) {
        if (!arg.error())
            return std::unexpected(make_error<basic_general_error>(call.location, "missing argument"sv));
        return std::unexpected(arg.error());
    }

    // Verify no more arguments
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    
    auto pmd = make_shared<functional_match_descriptor>(call.location);
    pmd->get_match_result(0).append_result(*arg);
    pmd->void_spans = std::move(call_session.void_spans);
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage> is_const_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& mr = md.get_match_result(0);
    auto& arg_result = mr.results.front();
    
    // Create the result
    return syntax_expression_result_t {
        .temporaries = std::move(arg_result.temporaries),
        .expressions = md.merge_void_spans(el),
        .value_or_type = u.make_bool_entity(arg_result.is_const_result).id,
        .is_const_result = true // Result is always a compile-time constant (true or false)
    };
}

}
