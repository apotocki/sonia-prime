//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_tail_pattern.hpp"
#include "tuple_pattern_base.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/expression_visitor.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> tuple_tail_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    // Use shared base logic
    return try_match_tuple(ctx, call, exp);
}

std::expected<syntax_expression_result_t, error_storage> tuple_tail_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& tmd = static_cast<tuple_pattern_match_descriptor&>(md);

    entity_signature result_sig{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) };
    entity_signature const& arg_sig = tmd.arg_sig;

    auto bit = arg_sig.fields().begin(), eit = arg_sig.fields().end();
    BOOST_ASSERT(bit != eit);
    field_descriptor const& first_field = *bit;

    size_t mut_field_count = 0;
    for (++bit; bit != eit; ++bit) {
        result_sig.push_back(*bit);
        if (!bit->is_const()) {
            ++mut_field_count;
        }
    }
    entity const& result_tuple = u.make_basic_signatured_entity(std::move(result_sig));
    auto & ser = tmd.get_match_result(0).results.front();
    ser.expressions = el.concat(md.merge_void_spans(el), ser.expressions);

    if (tmd.is_argument_typename) {
        // return typename tuple
        ser.value_or_type = result_tuple.id;
        BOOST_ASSERT(ser.is_const_result);
        return ser;
    }
    if (!first_field.is_const()) {
        BOOST_ASSERT(ser.expressions);
        if (mut_field_count) {
            u.push_back_expression(el, ser.expressions, semantic::invoke_function(u.get(builtin_eid::array_tail)));
        } else {
            u.push_back_expression(el, ser.expressions, semantic::truncate_values(1, false));
        }
    }
    if (mut_field_count) {
        ser.value_or_type = result_tuple.id;
        BOOST_ASSERT(!ser.is_const_result);
        return std::move(ser);
    }
    ser.value_or_type = u.make_empty_entity(result_tuple).id;
    ser.is_const_result = true;
    return std::move(ser);
}

}
