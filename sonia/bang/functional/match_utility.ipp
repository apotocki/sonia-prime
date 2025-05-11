//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <expected>

#include "sonia/bang/entities/functional.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/base_expression_visitor.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

template <typename HandlerT>
std::expected<syntax_expression_result_t, error_storage> match_type(fn_compiler_context& caller_ctx, semantic::expression_list_t& expressions,
    syntax_expression_t const& expr, entity_identifier const& eid, lex::resource_location eidloc, HandlerT const& hf)
{
    auto res = apply_visitor(base_expression_visitor{ caller_ctx, expressions, { eid, eidloc } }, expr);
    if (!res) return std::unexpected(std::move(res.error()));
    syntax_expression_result_t& ser = res->first;
    if (ser.is_const_result) {
        entity const& ent = get_entity(caller_ctx.u(), ser.value());
        if (auto err = hf(ent.get_type(), res->second); err) return std::unexpected(std::move(err));
    } else {
        if (auto err = hf(ser.type(), res->second); err) return std::unexpected(std::move(err));
    }
    return std::move(ser);
}

}
