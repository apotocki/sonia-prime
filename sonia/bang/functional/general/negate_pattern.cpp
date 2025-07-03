//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "negate_pattern.hpp"

#include "sonia/bang/functional/generic_pattern_base.ipp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> 
negate_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);
    
    // Expect a boolean argument
    expected_result_t bool_exp{ u.get(builtin_eid::boolean), call.location };
    std::pair<syntax_expression_t const*, size_t> arg_expr;
    auto arg = call_session.use_next_positioned_argument(bool_exp, &arg_expr);
    if (!arg) {
        if (!arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "expected a boolean argument"sv));
        }
        return std::unexpected(arg.error());
    }
    
    // Check if there are any unused arguments
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), 
            "argument mismatch"sv, std::move(argterm.value())));
    }
    
    // Create match descriptor
    auto pmd = make_shared<functional_match_descriptor>(call);
    pmd->emplace_back(0, arg->first);
    pmd->void_spans = std::move(call_session.void_spans);
    pmd->signature.result.emplace(u.get(builtin_eid::boolean), arg->first.is_const_result);
    
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage>
negate_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& [_, arg_er] = md.matches.front();
    
    syntax_expression_result_t result{
        .temporaries = std::move(arg_er.temporaries),
        .stored_expressions = arg_er.stored_expressions,
        .expressions = md.merge_void_spans(el),
        .is_const_result = arg_er.is_const_result
    };
    
    // If argument is constant, compute the result at compile time
    if (arg_er.is_const_result) {
        entity_identifier value = arg_er.value();
        bool is_true = (value == u.get(builtin_eid::true_));
        result.value_or_type = is_true ? u.get(builtin_eid::false_) : u.get(builtin_eid::true_);
        return result;
    }
    
    // For non-const values, generate a runtime negate operation
    result.value_or_type = u.get(builtin_eid::boolean);
    result.expressions = el.concat(result.expressions, arg_er.expressions);
    u.push_back_expression(el, result.expressions, semantic::invoke_function(u.get(builtin_eid::negate)));
    
    return result;
}

} // namespace sonia::lang::bang
