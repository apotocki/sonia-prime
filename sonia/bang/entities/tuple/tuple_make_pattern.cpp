//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_make_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/ast/expression_visitor.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> tuple_make_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();
    entity_identifier veid = u.get(builtin_eid::void_);
    size_t pos_arg_num = 0;
    auto pmd = make_shared<functional_match_descriptor>();
    for (auto const& arg : call.args) {
        auto res = apply_visitor(base_expression_visitor{ ctx, call.expressions }, arg.value());
        if (!res) return std::unexpected(std::move(res.error()));
        auto& ser = res->first;
        if (ser.is_const_result && ser.value() == veid) {
            if (ser.expressions)
                pmd->void_spans.push_back(ser.expressions);
            continue;
        }

        parameter_match_result* pmr;
        if (annotated_identifier const* pargname = arg.name(); pargname) {
            pmr = &pmd->push_match_result(pargname->value);
        } else {
            pmr = &pmd->get_match_result(pos_arg_num++);
        }
        pmr->append_result(ser);
    }
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> tuple_make_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    
    size_t mut_argcount = 0; // number of mutable arguments

    entity_signature signature{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) };

    syntax_expression_result_t result{ .expressions = md.merge_void_spans(el) };
    
    md.for_each_match([&signature, &result, &el, &mut_argcount](variant<identifier, size_t> name_or_pos, parameter_match_result const& mr) {
        BOOST_ASSERT(mr.results.size() == 1);
        syntax_expression_result_t er = mr.results.front();
        result.temporaries.insert(result.temporaries.end(), er.temporaries.begin(), er.temporaries.end());
        result.expressions = el.concat(result.expressions, er.expressions);
        if (!er.is_const_result) ++mut_argcount;
        if (name_or_pos.which() == 0) { // named element
            identifier name = get<identifier>(name_or_pos);
            signature.emplace_back(name, er.value_or_type, er.is_const_result);
        } else {
            signature.emplace_back(er.value_or_type, er.is_const_result);
        }
    });
    entity const& tuple_type_ent = u.make_basic_signatured_entity(std::move(signature));
    BOOST_ASSERT(tuple_type_ent.signature() && tuple_type_ent.signature()->name == u.get(builtin_qnid::tuple));
    if (mut_argcount > 1) {
        u.push_back_expression(el, result.expressions, semantic::push_value{ mp::integer{ mut_argcount } });
        u.push_back_expression(el, result.expressions, semantic::invoke_function(u.get(builtin_eid::arrayify)));
    }
    if (mut_argcount) {
        result.value_or_type = tuple_type_ent.id;
        result.is_const_result = false;
    } else {
        result.value_or_type = u.make_empty_entity(tuple_type_ent).id;
        result.is_const_result = true;
    }

    return result;
}
//
//    // prepare subtuples for named elements
//    using named_element_t = std::tuple<identifier, entity_identifier, semantic::expression_span, bool>;
//    boost::container::small_flat_set<named_element_t, 8, tuple_1st_element_comparator> named_elements;
//    named_elements.reserve(md.named_matches_count());
//
//    md.for_each_named_match([&u, &el, &mut_argcount, &named_elements](identifier name, parameter_match_result const& mr) {
//        BOOST_ASSERT(mr.results.size() > 0);
//        if (mr.results.size() == 1) { // skip, will be processed in for_each_match
//            return;
//        }
//
//        entity_signature sub_signature{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) };
//        semantic::expression_span exprs;
//        size_t start_mut_args = mut_argcount;
//        for (auto const& ser : mr.results) {
//            exprs = el.concat(exprs, ser.expressions);
//            sub_signature.emplace_back(ser.value_or_type, ser.is_const_result);
//            if (!ser.is_const_result) {
//                ++mut_argcount;
//            }
//        }
//
//        entity const& e = u.make_basic_signatured_entity(std::move(sub_signature));
//        named_elements.emplace(name, e.id, std::move(exprs), start_mut_args == mut_argcount);
//    });
//
//
//    // push call expressions in the right order
//    semantic::expression_span exprs = md.merge_void_spans(el);
//
//    entity_signature signature{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) };
//
//    md.for_each_match([&signature, &exprs, &el, &mut_argcount, &named_elements](variant<identifier, size_t> name_or_pos, parameter_match_result const& mr) {
//        if (name_or_pos.which() == 0) { // named element
//            identifier name = get<identifier>(name_or_pos);
//            if (auto it = named_elements.find(name); it != named_elements.end()) {
//                named_element_t& ne = const_cast<named_element_t&>(*it);
//                exprs = el.concat(exprs, get<2>(ne));
//                signature.emplace_back(name, get<1>(ne), get<3>(ne));
//            } else {
//                BOOST_ASSERT(mr.results.size() == 1);
//                auto const& ser = mr.results.front();
//                exprs = el.concat(exprs, ser.expressions);
//                signature.emplace_back(name, ser.value_or_type, ser.is_const_result);
//                if (!ser.is_const_result) { ++mut_argcount; }
//            }
//        } else {
//            BOOST_ASSERT(mr.results.size() == 1);
//            auto const& ser = mr.results.front();
//            exprs = el.concat(exprs, ser.expressions);
//            signature.emplace_back(ser.value_or_type, ser.is_const_result);
//            if (!ser.is_const_result) { ++mut_argcount; }
//        }
//    });
//
//    entity const& tuple_type_ent = u.make_basic_signatured_entity(std::move(signature));
//    BOOST_ASSERT(tuple_type_ent.signature() && tuple_type_ent.signature()->name == u.get(builtin_qnid::tuple));
//
//    if (mut_argcount > 1) {
//        u.push_back_expression(el, exprs, semantic::push_value{ mp::integer{ mut_argcount } });
//        u.push_back_expression(el, exprs, semantic::invoke_function(u.get(builtin_eid::arrayify)));
//    }
//
//    if (mut_argcount) {
//        return syntax_expression_result_t{ .expressions = std::move(exprs), .value_or_type = tuple_type_ent.id, .is_const_result = false };
//    }
//
//    return syntax_expression_result_t{ .expressions = std::move(exprs), .value_or_type = u.make_empty_entity(tuple_type_ent).id, .is_const_result = true };
//}

}
