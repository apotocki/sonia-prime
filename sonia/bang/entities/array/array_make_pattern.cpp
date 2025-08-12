//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "array_make_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/ast/base_expression_visitor.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

class array_make_match_descriptor : public functional_match_descriptor
{
public:
    using functional_match_descriptor::functional_match_descriptor;

    entity_identifier element_type;
    bool has_explicit_type;  // true if 'of' parameter was provided
    bool all_const; // true if all elements are const
};

std::expected<functional_match_descriptor_ptr, error_storage> array_make_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);

    auto pmd = make_shared<array_make_match_descriptor>(call);

    // Try to get the optional 'of' parameter first
    prepared_call::argument_descriptor_t of_arg_descr;
    auto of_res = call_session.use_named_argument(u.get(builtin_id::of), 
        expected_result_t{ .type = u.get(builtin_eid::typename_), .modifier = value_modifier_t::constexpr_value }, &of_arg_descr);
    
    if (of_res) {
        // Explicit type specified
        auto& of_arg = of_res->first;
        pmd->element_type = of_arg.value();
        pmd->has_explicit_type = true;
        pmd->emplace_back(0, of_arg, get_start_location(*get<0>(of_arg_descr)));
        pmd->signature.emplace_back(u.get(builtin_id::of), of_arg.value_or_type, true);
    } else if (of_res.error()) {
        return std::unexpected(std::move(of_res.error()));
    } else {
        // No explicit type, will be inferred from elements
        pmd->has_explicit_type = false;
    }

    // Collect element arguments
    boost::container::small_flat_set<entity_identifier, 8> element_types;
    small_vector<entity_identifier, 16> stable_element_types_set;
    size_t element_count = 0;
    pmd->all_const = true;

    expected_result_t exp_element_res{ .type = pmd->element_type, .modifier = exp.modifier };
    // Get all positioned arguments (the array elements)
    for (size_t arg_num = pmd->has_explicit_type ? 1 : 0;;++arg_num) {
        prepared_call::argument_descriptor_t elem_descr;
        auto elem_res = call_session.use_next_positioned_argument(exp_element_res, &elem_descr);
        if (!elem_res) {
            if (elem_res.error()) return std::unexpected(std::move(elem_res.error()));
            break;
        }
        auto& ser = elem_res->first;

        // Collect element type for inference if needed
        entity_identifier elem_type;
        if (ser.is_const_result) {
            elem_type = get_entity(u, ser.value()).get_type();
        } else {
            elem_type = ser.type();
            pmd->all_const = false;
        }
        
        if (element_types.insert(elem_type).second) {
            stable_element_types_set.push_back(elem_type);
        }

        pmd->signature.emplace_back(ser.value_or_type, ser.is_const_result);
        pmd->emplace_back(arg_num, ser, get_start_location(*get<0>(elem_descr)));
        ++element_count;
    }

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    // If no explicit type, infer from elements
    if (!pmd->has_explicit_type) {
        if (element_count == 0) {
            return std::unexpected(make_error<basic_general_error>(call.location, "cannot infer array element type from empty argument list"sv));
        }
        pmd->element_type = u.make_union_type_entity(stable_element_types_set).id;
    }

    if (pmd->has_explicit_type || stable_element_types_set.size() == 1) {
        entity_identifier array_type_id = u.make_array_type_entity(pmd->element_type, element_count).id;
        if (pmd->all_const) {
            auto mspan = span{ pmd->matches };
            if (pmd->has_explicit_type) {
                // Skip the 'of' parameter
                mspan = mspan.subspan(1);
            }

            entity_signature data_sig{ u.get(builtin_qnid::data), array_type_id };
            for (auto& [arg_idx, mr, loc] : mspan) {
                data_sig.emplace_back(mr.value(), true);
            }
            entity_identifier const_array_id = u.make_basic_signatured_entity(std::move(data_sig)).id;
            pmd->signature.result.emplace(const_array_id, true);
        } else {
            // If not all elements are const, the array type is not const
            pmd->signature.result.emplace(array_type_id, false);
        }
    } // else pmd->signature.result is not set

    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> array_make_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& amd = static_cast<array_make_match_descriptor&>(md);
    
    syntax_expression_result_t result{ .is_const_result = amd.all_const };

    auto mspan = span{ md.matches };
    if (amd.has_explicit_type) {
        append_semantic_result(el, result, get<1>(mspan.front()));
        // Skip the 'of' parameter
        mspan = mspan.subspan(1);
    }

    if (md.signature.result) {
        result.value_or_type = md.signature.result->entity_id();
        BOOST_ASSERT(result.is_const_result = md.signature.result->is_const());

        if (result.is_const_result) {
            for (auto& [_, er, loc] : mspan) {
                BOOST_ASSERT(er.is_const_result);
                append_semantic_result(el, result, er);
            }
        } else {
            for (auto& [_, er, loc] : mspan) {
                append_semantic_result(el, result, er);
                if (er.is_const_result) {
                    // If the element is const, we need to push it as a runtime value
                    pure_call_t cast_call{ loc };
                    cast_call.emplace_back(annotated_entity_identifier{ er.value(), loc });
                    auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, el, 
                        expected_result_t{ .type = amd.element_type, .location = loc, .modifier = value_modifier_t::runtime_value });
                    if (!match) {
                        return std::unexpected(std::move(match.error()));
                    }
                    auto res = match->apply(ctx);
                    if (!res) {
                        return std::unexpected(std::move(res.error()));
                    }
                    append_semantic_result(el, result, *res);
                }
            }
            size_t element_count = mspan.size();
            if (element_count > 1) {
                u.push_back_expression(el, result.expressions, semantic::push_value{ smart_blob{ ui64_blob_result(element_count) } });
                u.push_back_expression(el, result.expressions, semantic::invoke_function(u.get(builtin_eid::arrayify)));
            }
        }
        return result;
    }
    
    // result type is union, so we need to handle it differently
    
    entity_identifier array_type_id = u.make_array_type_entity(amd.element_type, mspan.size()).id;
    entity_signature data_sig{ u.get(builtin_qnid::data), array_type_id };

    for (auto& [_, er, loc] : mspan) {
        pure_call_t cast_call{ loc };
        if (er.is_const_result) {
            cast_call.emplace_back(annotated_entity_identifier{ er.value(), loc });
        } else {
            cast_call.emplace_back(make_indirect_value(u, el, std::move(er), loc));
        }
        auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, el,
            expected_result_t{ .type = amd.element_type, .location = loc, .modifier = amd.all_const ? value_modifier_t::constexpr_value : value_modifier_t::runtime_value });
        if (!match) {
            return std::unexpected(std::move(match.error()));
        }
        auto res = match->apply(ctx);
        if (!res) {
            return std::unexpected(std::move(res.error()));
        }
        append_semantic_result(el, result, *res);
        if (amd.all_const) {
            data_sig.emplace_back(res->value(), true);
        }
    }

    if (amd.all_const) {
        result.value_or_type = u.make_basic_signatured_entity(std::move(data_sig)).id;
    } else {
        size_t element_count = mspan.size();
        if (element_count > 1) {
            u.push_back_expression(el, result.expressions, semantic::push_value{ smart_blob{ ui64_blob_result(element_count) } });
            u.push_back_expression(el, result.expressions, semantic::invoke_function(u.get(builtin_eid::arrayify)));
        }
        result.value_or_type = array_type_id;
    }
        
    return result;
}

}
