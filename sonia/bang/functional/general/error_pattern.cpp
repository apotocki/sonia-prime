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
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);
    
    // Accept a single unnamed string argument
    expected_result_t string_exp{ u.get(builtin_eid::string), call.location };
    std::pair<syntax_expression_t const*, size_t> argexpr;
    auto arg = call_session.use_next_positioned_argument(string_exp, &argexpr);
    
    if (!arg) {
        if (!arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing string argument"sv));
        }
        return std::unexpected(arg.error());
    }

    auto locarg = call_session.use_named_argument(u.get(builtin_id::location), string_exp, &argexpr);
    if (!locarg) {
        if (locarg.error()) {
            return std::unexpected(locarg.error());
        }
    }
    // Verify no more arguments
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    
    auto pmd = make_shared<functional_match_descriptor>(call);
    pmd->emplace_back(0, arg->first);
    if (locarg) {
        pmd->emplace_back(1, locarg->first);
    }
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage> error_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& msg_er = get<1>(md.matches.front());
    
    syntax_expression_result_t result{
        .temporaries = std::move(msg_er.temporaries),
        .branches_expressions = std::move(msg_er.branches_expressions)
    };
    
    generic_literal_entity const* pmsg_ent = nullptr;
    generic_literal_entity const* ploc_ent = nullptr;

    if (md.matches.size() > 1) {
        auto& loc_er = get<1>(md.matches.back());
        result.expressions = el.concat(result.expressions, loc_er.expressions);
        if (loc_er.is_const_result) {
            entity const& loc_ent = get_entity(u, loc_er.value());
            BOOST_ASSERT(loc_ent.get_type() == u.get(builtin_eid::string));
            ploc_ent = &static_cast<generic_literal_entity const&>(loc_ent);
        }
    }

    if (msg_er.is_const_result) {
        // Argument is a compile-time constant, return an error immediately
        entity const& ent = get_entity(u, msg_er.value());
        BOOST_ASSERT(ent.get_type() == u.get(builtin_eid::string));
        pmsg_ent = &static_cast<generic_literal_entity const&>(ent);
    }


    if (pmsg_ent && (md.matches.size() == 1 || ploc_ent)) {
        // Create error with the string content as description
        return std::unexpected(make_error<basic_general_error>(
            ploc_ent ? error::location_t{ ploc_ent->value().as<string_view>() } : md.call_location,
            pmsg_ent->value().as<string_view>()));
    } else {
        // Argument is a runtime value, produce a call to builtin_eid::error
        if (ploc_ent) {
            //u.push_back_expression(el, result.expressions, semantic::push_value{ smart_blob{ ploc_ent->value() } });
            u.push_back_expression(el, result.expressions, semantic::push_value{ ploc_ent->id });
        } else if (md.matches.size() == 1) {
            smart_blob locstr{ string_blob_result(u.print(md.call_location)) }; // will allocate
            u.push_back_expression(el, result.expressions, semantic::push_value{ std::move(locstr) });
        } // else the location argument is already on stack
        result.expressions = el.concat(result.expressions, msg_er.expressions);
        if (pmsg_ent) {
            u.push_back_expression(el, result.expressions, semantic::push_value{ pmsg_ent->id });
        } // else the message argument is already on stack
        u.push_back_expression(el, result.expressions, semantic::invoke_function(u.get(builtin_eid::error)));
        result.value_or_type = u.get(builtin_eid::void_);
        result.is_const_result = false;
    }
    
    return std::move(result);
}

}
