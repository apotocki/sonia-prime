//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_get_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/ast/expression_visitor.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"
//#include "sonia/bang/entities/functions/match_utility.ipp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> tuple_get_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const&) const
{
    unit& u = ctx.u();
    identifier slfid = u.get(builtin_id::self);
    identifier propid = u.get(builtin_id::property);
    
    entity const* pte = nullptr;
    entity const* ppname = nullptr;
    shared_ptr<tuple_get_match_descriptor> pmd;
    auto estate = ctx.expressions_state();

    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        auto const& argexpr = arg.value();
        if (!pargname) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "argument mismatch"sv, argexpr));
        }
        if (pargname->value == slfid) {
            if (pte) {
                return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch"sv, pargname->value));
            }
            auto res = apply_visitor(base_expression_visitor{ ctx, call.expressions }, argexpr);
            if (!res) return std::unexpected(std::move(res.error()));
            auto& ser = res->first;
            entity const& arg_entity = get_entity(u, ser.value_or_type);
            if (ser.is_const_result) {
                entity const& arg_type_entity = get_entity(u, arg_entity.get_type());
                if (auto psig = arg_type_entity.signature(); psig && psig->name == ctx.u().get(builtin_qnid::tuple)) {
                    // argument is typename tuple
                    pte = &arg_type_entity;
                    pmd = make_shared<tuple_get_match_descriptor>();
                    auto& mr = pmd->get_match_result(pargname->value);
                    mr.append_result(ser);
                    continue;
                } else {
                    return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "argument mismatch"sv, argexpr));
                    //argtype = arg_entity.get_type();
                }
            }

            auto psig = arg_entity.signature();
            if (!psig || psig->name != ctx.u().get(builtin_qnid::tuple)) {
                return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch"sv, pargname->value));
            }

            pte = &arg_entity;
            pmd = make_shared<tuple_get_match_descriptor>();
            auto& mr = pmd->get_match_result(pargname->value);
            mr.append_result(ser);
            
        } else if (pargname->value == propid && !ppname) {
            ct_expression_visitor evis{ ctx, call.expressions };
            auto res = apply_visitor(evis, arg.value());
            if (!res) return std::unexpected(std::move(res.error()));
            BOOST_ASSERT(!res->expressions); // not impelemented const value expressions
            ppname = &get_entity(u, res->value);
            /*
            entity const& some_entity = ctx.u().eregistry_get(*res);
            ppname = dynamic_cast<identifier_entity const*>(&some_entity);
            if (ppname) continue;
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch, expected an identifier"sv, pargname->value));
            */
        } else {
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch"sv, pargname->value));
        }
    }

    if (!pte) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter: `self`"sv));
    } else if (!ppname) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter: `property`"sv));
    }   
    return check_match(std::move(pmd), call, *pte, *ppname);
}

std::expected<functional_match_descriptor_ptr, error_storage> tuple_get_pattern::check_match(shared_ptr<tuple_get_match_descriptor> pmd, prepared_call const& call, entity const& te, entity const& pname) const
{
    entity_signature const* pes = te.signature();
    BOOST_ASSERT(pes);
    size_t index;
    field_descriptor const* fd;
    if (identifier_entity const* pie = dynamic_cast<identifier_entity const*>(&pname); pie) {
        fd = pes->find_field(pie->value(), &index);
    } else if (integer_literal_entity const* pile = dynamic_cast<integer_literal_entity const*>(&pname); pile) {
        fd = pes->find_field((size_t)pile->value(), &index);
    } else {
        return std::unexpected(make_error<basic_general_error>(pname.location, "argument mismatch, expected an identifier or index"sv, pname.id));
    }
    
    if (!fd) {
        return std::unexpected(make_error<basic_general_error>(call.location, "no such field"sv, pname.id));
    }

    // get non const fields count and index
    size_t fields_count = pes->fields().size();

    field_descriptor const* b_fd = &pes->fields().front(), *e_fd = b_fd + pes->fields().size();
    for (; b_fd != fd; ++b_fd) {
        if (b_fd->is_const()) { --index; --fields_count; }
    }
    for (; b_fd != e_fd; ++b_fd) {
        if (b_fd->is_const()) { --fields_count; }
    }

    pmd->property_index = index;
    pmd->fields_count = fields_count;
    
    BOOST_ASSERT(pes->field_count() > 1 || !index);
    
    pmd->result = *fd;
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> tuple_get_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& tmd = static_cast<tuple_get_match_descriptor&>(md);

    if (tmd.result.is_const()) {
        return syntax_expression_result_t{
            .expressions = md.merge_void_spans(el),
            .value_or_type = tmd.result.entity_id(),
            .is_const_result = true
        };
    }

    // push call expressions in the right order
    semantic::expression_span exprs = md.merge_void_spans(el);

    // only one named argument is expected
    tmd.for_each_named_match([&exprs, &el](identifier name, parameter_match_result const& mr) {
        for (auto const& ser : mr.results) {
            exprs = el.concat(exprs, ser.expressions);
        }
    });

    if (tmd.fields_count > 1) {
        u.push_back_expression(el, exprs, semantic::push_value{ tmd.property_index });
        u.push_back_expression(el, exprs, semantic::invoke_function(u.get(builtin_eid::array_at)));
    }

    return syntax_expression_result_t{
        .expressions = std::move(exprs),
        .value_or_type = md.result.entity_id(),
        .is_const_result = false,
    };
}

}
