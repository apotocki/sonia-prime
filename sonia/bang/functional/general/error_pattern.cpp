//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "error_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> error_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    auto call_session = call.new_session(ctx);
    
    // Accept a single unnamed string argument
    expected_result_t string_exp{ ctx.u().get(builtin_eid::string), call.location };
    syntax_expression_t const* pargexpr;
    auto arg = call_session.use_next_positioned_argument(string_exp, &pargexpr);
    
    if (!arg) {
        if (!arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing string argument"sv));
        }
        return std::unexpected(arg.error());
    }

    // Verify no more arguments
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    
    auto pmd = make_shared<functional_match_descriptor>(call);
    pmd->emplace_back(0, arg->first);
    pmd->void_spans = std::move(call_session.void_spans);
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage> error_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& [_, er] = md.matches.front();
    
    syntax_expression_result_t result{
        .temporaries = std::move(er.temporaries),
        .stored_expressions = std::move(er.stored_expressions),
        .expressions = md.merge_void_spans(el)
    };
    
    if (er.is_const_result) {
        // Argument is a compile-time constant, return an error immediately
        entity const& ent = get_entity(u, er.value());
        BOOST_ASSERT(ent.get_type() == u.get(builtin_eid::string));
        
        string_literal_entity const& str_ent = static_cast<string_literal_entity const&>(ent);
        
        // Create error with the string content as description
        return std::unexpected(make_error<basic_general_error>(md.call_location, str_ent.value()));
    } else {
        // Argument is a runtime value, produce a call to builtin_eid::error
        result.expressions = el.concat(result.expressions, er.expressions);
        u.push_back_expression(el, result.expressions, semantic::invoke_function(u.get(builtin_eid::error)));
        result.value_or_type = u.get(builtin_eid::void_);
        result.is_const_result = false;
    }
    
    return std::move(result);
}

}
