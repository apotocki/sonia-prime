//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <expected>

#include "sonia/bang/entities/functional.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/base_expression_visitor.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

template <typename HandlerT>
std::expected<functional::pattern::application_result_t, error_storage> match_type(fn_compiler_context& caller_ctx, syntax_expression_t const& expr, entity_identifier const& eid, lex::resource_location eidloc, HandlerT const& hf)
{
    auto res = apply_visitor(base_expression_visitor{ caller_ctx, { eid, eidloc } }, expr);
    if (!res) return std::unexpected(std::move(res.error()));
    
    using result_type = std::expected<functional::pattern::application_result_t, error_storage>;
    return apply_visitor(make_functional_visitor<result_type>([&caller_ctx, &hf, &eidloc, &expr, casted = res->second](auto& v) -> result_type {
        if constexpr (std::is_same_v<std::decay_t<decltype(v)>, entity_identifier>) {
            if (v == caller_ctx.u().get(builtin_eid::void_)) {
                return std::unexpected(make_error<type_mismatch_error>(get_start_location(expr), v, "not void"sv, eidloc));
            }
            entity const& ent = caller_ctx.u().eregistry_get(v);
            if (auto err = hf(ent.get_type(), casted); err) return std::unexpected(std::move(err));
        } else {
            if (caller_ctx.context_type != caller_ctx.u().get(builtin_eid::void_)) {
                if (auto err = hf(caller_ctx.context_type, casted); err) return std::unexpected(std::move(err));
            }
        }
        return std::move(v);
    }), res->first);
}

}
