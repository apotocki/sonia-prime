//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "vector_make_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/ast/base_expression_visitor.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> vector_make_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);

    // retrieve element type
    prepared_call::argument_descriptor_t elementargdescr;
    auto res = call_session.use_named_argument(u.get(builtin_id::of), expected_result_t{ .type = u.get(builtin_eid::typename_), .modifier = value_modifier_t::constexpr_value }, &elementargdescr);
    if (!res) {
        if (res.error()) return std::unexpected(std::move(res.error()));
        return std::unexpected(make_error<basic_general_error>(call.location, "expected element type parameter"sv));
    }

    auto& element_arg = res->first;
    
    auto pmd = make_shared<vector_make_match_descriptor>(call);
    pmd->element_type = element_arg.value();
    pmd->emplace_back(0, element_arg);
    pmd->signature.emplace_back(u.get(builtin_id::of), element_arg.value_or_type, true);

    expected_result_t exp_elem_res{
        .type = pmd->element_type,
        .modifier = exp.modifier
    };

    bool const_elements = true;
    // Remaining arguments must be unnamed and be able to implicit cast to the element type
    for (size_t arg_num = 1;;++arg_num) {
        prepared_call::argument_descriptor_t argdescr;
        auto res = call_session.use_next_positioned_argument(exp_elem_res, &argdescr);
        if (!res) {
            if (res.error()) return std::unexpected(std::move(res.error()));
            break;
        }
        auto& ser = res->first;
        
        pmd->signature.emplace_back(ser.value_or_type, ser.is_const_result);
        pmd->emplace_back(arg_num++, ser);
        if (!ser.is_const_result) {
            const_elements = false;
        }
    }
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    entity_identifier vec_type_id = u.make_vector_type_entity(element_arg.value()).id;
    if (const_elements) {
        // If all elements are const, we can use the const vector value
        entity_signature vector_elements{ u.get(builtin_qnid::data), vec_type_id };
        for (auto const& fd : pmd->signature.fields().subspan(1) ) {
            vector_elements.push_back(fd);
        }
        pmd->signature.result.emplace(u.make_basic_signatured_entity(std::move(vector_elements)).id, true);
    } else {
        // Otherwise, use the non-const vector type
        pmd->signature.result.emplace(vec_type_id, false);
    }

    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> vector_make_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& vmd = static_cast<vector_make_match_descriptor&>(md);
    
    syntax_expression_result_t result{
        .value_or_type = md.signature.result->entity_id(),
        .is_const_result = md.signature.result->is_const()
    };
    
    // Merge all element expressions
    for (auto& [_, er, loc] : md.matches) {
        append_semantic_result(el, result, er);
    }

    if (!result.is_const_result) {
        size_t element_count = md.matches.size() - 1;
        u.push_back_expression(el, result.expressions, semantic::push_value{ smart_blob{ ui64_blob_result(element_count) } });
        u.push_back_expression(el, result.expressions, semantic::invoke_function(u.get(builtin_eid::arrayify)));
    }
        
    return result;
}

}
