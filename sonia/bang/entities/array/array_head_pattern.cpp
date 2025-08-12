//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "array_head_pattern.hpp"

#include <algorithm>

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"
#include "sonia/bang/errors/value_mismatch_error.hpp"

namespace sonia::lang::bang {

class array_head_pattern_match_descriptor : public functional_match_descriptor
{
public:
    inline array_head_pattern_match_descriptor(prepared_call const& call, entity_signature const* sig) noexcept
        : functional_match_descriptor{ call }
        , arg_sig{ sig }
    {}

    entity_signature const* arg_sig;
};

std::expected<functional_match_descriptor_ptr, error_storage> array_head_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);
    std::pair<syntax_expression_t const*, size_t> arg_expr;
    auto arg = call_session.use_next_positioned_argument(&arg_expr);
    if (!arg) {
        if (!arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing argument"sv));
        }
        return std::unexpected(arg.error());
    }
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    syntax_expression_result_t& er = arg->first;
    entity_identifier argtype;
    shared_ptr<functional_match_descriptor> pmd;
    if (er.is_const_result) {
        entity const& arg_entity = get_entity(u, er.value());
        if (auto psig = arg_entity.signature(); psig && psig->name == u.get(builtin_qnid::data)) {
            argtype = arg_entity.get_type();
            pmd = make_shared<array_head_pattern_match_descriptor>(call, psig);
        } else {
            return std::unexpected(make_error<type_mismatch_error>(get_start_location(*get<0>(arg_expr)), er.value(), "an array type"sv));
        }
    } else {
        argtype = er.type();
        pmd = make_shared<array_head_pattern_match_descriptor>(call, nullptr);
    }

    entity const& arg_type_entity = get_entity(u, argtype);
    if (auto psig = arg_type_entity.signature(); !psig || psig->name != u.get(builtin_qnid::array)) {
        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*get<0>(arg_expr)), argtype, "an array type"sv));
    }

    pmd->emplace_back(0, er, get_start_location(*get<0>(arg_expr)));
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> array_head_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& amd = static_cast<array_head_pattern_match_descriptor&>(md);
    auto& [_, er, loc] = md.matches.front();

    if (er.is_const_result) {
        field_descriptor const* pfd = amd.arg_sig->get_field(0);
        if (!pfd) {
            return std::unexpected(make_error<value_mismatch_error>(loc, er.value(), "empty array type"sv));
        }
        er.value_or_type = pfd->entity_id();
    } else {
        entity const& arg_type_entity = get_entity(u, er.type());
        field_descriptor const* tfd = arg_type_entity.signature()->find_field(u.get(builtin_id::of));
        if (!tfd) {
            THROW_INTERNAL_ERROR("array_head_pattern::apply: expected 'of' field in array type"sv);
        }
        er.value_or_type = tfd->entity_id();
        field_descriptor const* szfd = arg_type_entity.signature()->find_field(u.get(builtin_id::size));
        if (!szfd) {
            THROW_INTERNAL_ERROR("array_head_pattern::apply: expected 'size' field in array type"sv);
        }
        generic_literal_entity const* pszent = dynamic_cast<generic_literal_entity const*>(&get_entity(u, szfd->entity_id()));
        if (!pszent) {
            THROW_INTERNAL_ERROR("array_head_pattern::apply: expected 'size' field to be a literal entity"sv);
        }
        size_t arrsz = pszent->value().as<size_t>();
        if (arrsz > 1) {
            u.push_back_expression(el, er.expressions, semantic::push_by_offset{ 0 });
            u.push_back_expression(el, er.expressions, semantic::invoke_function(u.get(builtin_eid::array_at)));
        }
    }
    return std::move(er);
}

}
