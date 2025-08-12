//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_set_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/auxiliary.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

class tuple_set_match_descriptor : public functional_match_descriptor
{
    size_t property_index_;

public:
    using functional_match_descriptor::functional_match_descriptor;

    inline size_t property_index() const noexcept { return property_index_; }
    inline void set_property_index(size_t index) noexcept { property_index_ = index; }
};

/**
    inline fn::set(self: tuple(), property: const __identifier @has_property($T), _) -> typeof(self) => implementation defined
*/

std::expected<functional_match_descriptor_ptr, error_storage> tuple_set_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();
    
    auto call_session = call.new_session(ctx);
    std::pair<syntax_expression_t const*, size_t> slf_arg_expr;

    auto slf_arg = call_session.use_named_argument(u.get(builtin_id::self), expected_result_t{}, &slf_arg_expr);
    if (!slf_arg) {
        if (!slf_arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing `self` argument"sv));
        }
        return std::unexpected(std::move(slf_arg.error()));
    }

    std::pair<syntax_expression_t const*, size_t> prop_arg_expr;
    alt_error prop_errors;
    auto property_arg = call_session.use_named_argument(u.get(builtin_id::property), expected_result_t{ u.get(builtin_eid::integer) }, &prop_arg_expr);
    if (!property_arg && property_arg.error()) {
        prop_errors.alternatives.emplace_back(std::move(property_arg.error()));
        call_session.reuse_argument(get<1>(prop_arg_expr));
        property_arg = call_session.use_named_argument(u.get(builtin_id::property), expected_result_t{ u.get(builtin_eid::identifier) }, &prop_arg_expr);
    }
    if (!property_arg) {
        if (!property_arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument: `property`"sv));
        }
        if (prop_errors.alternatives.empty()) {
            return std::unexpected(std::move(property_arg.error()));
        } else {
            prop_errors.alternatives.emplace_back(std::move(property_arg.error()));
            return std::unexpected(make_error<alt_error>(std::move(prop_errors)));
        }
    }

    std::pair<syntax_expression_t const*, size_t> prop_val_arg_expr;
    auto valarg = call_session.use_next_positioned_argument(&prop_val_arg_expr);
    if (!valarg) {
        if (!valarg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing required value argument"sv));
        }
        return std::unexpected(std::move(valarg.error()));
    }

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    shared_ptr<tuple_set_match_descriptor> pmd = make_shared<tuple_set_match_descriptor>(call);
    auto& ser = slf_arg->first;
    if (ser.is_const_result) {
        return std::unexpected(make_error<basic_general_error>(get_start_location(*get<0>(slf_arg_expr)), "argument mismatch, expected a mutable tuple"sv, ser.value()));
    }

    entity const& some_entity = get_entity(u, ser.type());
    auto psig = some_entity.signature();
    if (!psig || psig->name != u.get(builtin_qnid::tuple)) {
        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*get<0>(slf_arg_expr)), ctx.context_type, u.get(builtin_qnid::tuple)));
    }

    pmd->emplace_back(0, ser);
    pmd->emplace_back(1, property_arg->first);
    pmd->emplace_back(2, valarg->first);
    
    THROW_NOT_IMPLEMENTED_ERROR("tuple_set_pattern::try_match");
#if 0
    entity_signature const* pes = pte->signature();
    BOOST_ASSERT(pes);
    size_t index;
    field_descriptor const* fd;
    if (identifier_entity const* pie = dynamic_cast<identifier_entity const*>(ppname); pie) {
        fd = pes->find_field(pie->value(), &index);
    } else if (integer_literal_entity const* pile = dynamic_cast<integer_literal_entity const*>(ppname); pile) {
        fd = pes->find_field((size_t)pile->value(), &index);
    } else {
        return std::unexpected(make_error<basic_general_error>(ppname->location(), "argument mismatch, expected an identifier or index"sv, ppname->id()));
    }
    
    if (!fd) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "no such field"sv, ppname->id()));
    }

    pmd->set_property_index(index);
    BOOST_ASSERT(pes->field_count() > 1 || !index);

    pmd->result = fd->entity_id();
    return pmd;
#endif
}

std::expected<syntax_expression_result_t, error_storage> tuple_set_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();

    // push call expressions in the right order
    //semantic::managed_expression_list exprs{ u };
    syntax_expression_result_t result{ .value_or_type = md.signature.result->entity_id(), .is_const_result = false };

    // only one named argument is expected
    result.expressions = el.concat(result.expressions, get<1>(md.matches.front()).expressions);

    if (size_t propindex = static_cast<tuple_set_match_descriptor&>(md).property_index(); propindex) {
        u.push_back_expression(el, result.expressions, semantic::push_value{ smart_blob{ ui64_blob_result(propindex) } });
        u.push_back_expression(el, result.expressions, semantic::invoke_function(u.get(builtin_eid::array_at)));
    }

    return result;
}

}
