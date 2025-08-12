//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "array_tail_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/auxiliary.hpp"
#include "sonia/bang/errors/type_mismatch_error.hpp"
#include "sonia/bang/errors/value_mismatch_error.hpp"

namespace sonia::lang::bang {

class array_tail_pattern_match_descriptor : public functional_match_descriptor
{
public:
    inline array_tail_pattern_match_descriptor(prepared_call const& call, entity_signature const* sig) noexcept
        : functional_match_descriptor{ call }
        , arg_sig{ sig }
    {}

    entity_signature const* arg_sig;
};

std::expected<functional_match_descriptor_ptr, error_storage> array_tail_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);
    std::pair<syntax_expression_t const*, size_t> arg_expr;
    auto arg = call_session.use_next_positioned_argument(&arg_expr);
    if (!arg) {
        if (!arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing argument"sv));
        }
        return std::unexpected(std::move(arg.error()));
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
            pmd = make_shared<array_tail_pattern_match_descriptor>(call, psig);
        } else {
            return std::unexpected(make_error<type_mismatch_error>(get_start_location(*get<0>(arg_expr)), er.value(), "an array"sv));
        }
    } else {
        argtype = er.type();
        pmd = make_shared<array_tail_pattern_match_descriptor>(call, nullptr);
    }

    entity const& arg_type_entity = get_entity(u, argtype);
    if (auto psig = arg_type_entity.signature(); !psig || psig->name != u.get(builtin_qnid::array)) {
        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*get<0>(arg_expr)), er.value_or_type, "an array"sv));
    }

    pmd->emplace_back(0, er, get_start_location(*get<0>(arg_expr)));
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> array_tail_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& amd = static_cast<array_tail_pattern_match_descriptor&>(md);
    auto& [_, er, loc] = md.matches.front();
    
    if (er.is_const_result) {
        // For const arrays, build a new data signature with tail elements
        if (!amd.arg_sig || amd.arg_sig->field_count() == 0) {
            return std::unexpected(make_error<value_mismatch_error>(loc, er.value(), "not empty array"sv));
        }

        // Get element type and size from the array type
        entity const& arg_type_entity = get_entity(u, er.type());
        field_descriptor const* tfd = arg_type_entity.signature()->find_field(u.get(builtin_id::of));
        if (!tfd) {
            return std::unexpected(make_error<basic_general_error>(loc, "invalid array type: missing element type"sv));
        }
        entity_identifier element_type = tfd->entity_id();
        size_t array_size = amd.arg_sig->field_count();
        //field_descriptor const* szfd = arg_type_entity.signature()->find_field(u.get(builtin_id::size));
        //if (!szfd) {
        //    return std::unexpected(make_error<basic_general_error>(loc, "invalid array type: missing size field"sv));
        //}
        //
        //generic_literal_entity const* pszent = dynamic_cast<generic_literal_entity const*>(&get_entity(u, szfd->entity_id()));
        //if (!pszent) {
        //    return std::unexpected(make_error<basic_general_error>(loc, "array size must be a compile-time constant"sv));
        //}
        //
        //size_t array_size = pszent->value().as<size_t>();
        
        // Build tail type
        entity_identifier tail_type_id = u.make_array_type_entity(element_type, array_size - 1).id;
        entity_signature tail_sig{ u.get(builtin_qnid::data), tail_type_id };
        
        // Skip the first element, add the rest
        auto fields = amd.arg_sig->fields();
        for (size_t i = 1; i < fields.size(); ++i) {
            tail_sig.push_back(fields[i]);
        }
        
        er.value_or_type = u.make_basic_signatured_entity(std::move(tail_sig)).id;
        er.is_const_result = true;
    } else {
        // For runtime arrays, get element type and compute tail type
        entity const& arg_type_entity = get_entity(u, er.type());
        field_descriptor const* tfd = arg_type_entity.signature()->find_field(u.get(builtin_id::of));
        if (!tfd) {
            return std::unexpected(make_error<basic_general_error>(loc, "invalid array type: missing element type"sv));
        }
        entity_identifier element_type = tfd->entity_id();

        field_descriptor const* szfd = arg_type_entity.signature()->find_field(u.get(builtin_id::size));
        if (!szfd) {
            return std::unexpected(make_error<basic_general_error>(loc, "invalid array type: missing size field"sv));
        }
        
        generic_literal_entity const* pszent = dynamic_cast<generic_literal_entity const*>(&get_entity(u, szfd->entity_id()));
        if (!pszent) {
            return std::unexpected(make_error<basic_general_error>(loc, "array size must be a compile-time constant"sv));
        }
        
        size_t array_size = pszent->value().as<size_t>();
        
        // Check size constraint at apply time
        if (array_size == 0) {
            return std::unexpected(make_error<value_mismatch_error>(loc, er.value_or_type, "not empty array"sv));
        }
        
        entity_identifier tail_type_id = u.make_array_type_entity(element_type, array_size - 1).id;
        // Use the array_tail builtin function
        if (array_size > 1) {
            u.push_back_expression(el, er.expressions, semantic::invoke_function(u.get(builtin_eid::array_tail)));
            er.value_or_type = tail_type_id;
            er.is_const_result = false;
        } else {
            // If the array has only one element, return an empty array
            er.expressions = {};
            entity_signature tail_sig{ u.get(builtin_qnid::data), tail_type_id };
            er.value_or_type = u.make_basic_signatured_entity(std::move(tail_sig)).id;
            er.is_const_result = true;
        }
    }

    return std::move(er);
}

}