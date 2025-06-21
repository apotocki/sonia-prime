//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_head_pattern.hpp"
#include "tuple_pattern_base.hpp"

#include <algorithm>

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

// head( make_tuple(name0: value0, ...) ) => make_tuple(name0^^::identifier, value0)
// head( make_tuple(const value0, ...) ) => const value0
// head( make_tuple(value0, ...) ) => value0

// head( typename tuple(name0: value0, ...) ) => typename tuple(name0^^::identifier, value0)
// head( typename tuple(const value0, ...) ) => const value0
// head( typename tuple(type, ...) ) => type

std::expected<functional_match_descriptor_ptr, error_storage> tuple_head_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    // Use shared base logic
    return try_match_tuple(ctx, call, exp);
}

std::expected<syntax_expression_result_t, error_storage> tuple_head_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& tmd = static_cast<tuple_pattern_match_descriptor&>(md);
    auto& [_, ser] = md.matches.front();

    entity_identifier rtype;
    field_descriptor const& head_field = tmd.arg_sig.fields().front();
    if (head_field.name()) {
        entity_signature rsig{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) };
        rsig.emplace_back(u.make_identifier_entity(head_field.name()).id, true);
        rsig.emplace_back(head_field.entity_id(), head_field.is_const());
        rtype = u.make_basic_signatured_entity(std::move(rsig)).id;
    } else {
        rtype = head_field.entity_id();
    }

    if (tmd.is_argument_typename) {
        return syntax_expression_result_t{
            .temporaries = std::move(ser.temporaries),
            .expressions = md.merge_void_spans(el),
            .value_or_type = rtype,
            .is_const_result = true
        };
    }
    if (ser.is_const_result || head_field.is_const()) {
        return syntax_expression_result_t{
            .temporaries = std::move(ser.temporaries),
            .expressions = md.merge_void_spans(el),
            .value_or_type = head_field.name() ? u.make_empty_entity(rtype).id : rtype,
            .is_const_result = true
        };
    }
    ser.expressions = el.concat(md.merge_void_spans(el), ser.expressions);
    ser.value_or_type = rtype; // single non-const field, so use it as result
    size_t src_size = std::ranges::count_if(tmd.arg_sig.fields(), [](field_descriptor const& fd) { return !fd.is_const(); });
    if (src_size > 1) {
        u.push_back_expression(el, ser.expressions, semantic::push_value{ mp::integer{ 0 } });
        u.push_back_expression(el, ser.expressions, semantic::invoke_function(u.get(builtin_eid::array_at)));
    }
    return std::move(ser);
}

}
