//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_get_pattern.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/ast/expression_visitor.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

class tuple_get_match_descriptor : public functional_match_descriptor
{
public:
    using functional_match_descriptor::functional_match_descriptor;

    size_t property_index;
    size_t fields_count;
};

std::expected<functional_match_descriptor_ptr, error_storage> tuple_get_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    unit& u = ctx.u();
    identifier slfid = u.get(builtin_id::self);
    identifier propid = u.get(builtin_id::property);
    
    entity const* pte = nullptr;
    entity const* ppname = nullptr;
    shared_ptr<tuple_get_match_descriptor> pmd;
    auto estate = ctx.expressions_state();

    for (auto const& arg : call.args()) {
        annotated_identifier const* pargname = arg.name();
        if (!pargname) {
            auto const& argexpr = arg.value();
            return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "argument mismatch"sv, argexpr));
        }
        if (pargname->value == slfid && !pte) {
            pmd = make_shared<tuple_get_match_descriptor>(u);
            ctx.push_chain(pmd->call_expressions);
            auto last_expr_it = ctx.expressions().last();
            expression_visitor evis{ ctx };
            auto res = apply_visitor(evis, arg.value());
            if (!res) return std::unexpected(std::move(res.error()));
            if (ctx.context_type) {
                entity const& some_entity = ctx.u().eregistry_get(ctx.context_type);
                if (auto psig = some_entity.signature(); psig && psig->name == u.get(builtin_qnid::tuple)) {
                    pte = &some_entity;
                    pmd->get_match_result(pargname->value).append_result(false, ctx.context_type, last_expr_it, ctx.expressions());
                    continue;
                }
            }
            return std::unexpected(make_error<type_mismatch_error>(pargname->location, ctx.context_type, u.get(builtin_qnid::tuple)));
           //return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch, expected a tuple"sv, pargname->value));
        } else if (pargname->value == propid && !ppname) {
            ct_expression_visitor evis{ ctx };
            auto res = apply_visitor(evis, arg.value());
            if (!res) return std::unexpected(std::move(res.error()));
            ppname = &u.eregistry_get(*res);
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
        return std::unexpected(make_error<basic_general_error>(call.location(), "unmatched parameter: `self`"sv));
    } else if (!ppname) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "unmatched parameter: `property`"sv));
    }

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

std::expected<tuple_get_pattern::application_result_t, error_storage> tuple_get_pattern::generic_apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& tmd = static_cast<tuple_get_match_descriptor&>(md);

    if (tmd.result.is_const()) {
        return tmd.result.entity_id();
    }

    // push call expressions in the right order
    semantic::managed_expression_list exprs{ u };

    // only one named argument is expected
    tmd.for_each_named_match([&exprs, &tmd](identifier name, parameter_match_result const& mr) {
        for (auto rng : mr.expressions) {
            ++rng.second;
            exprs.splice_back(tmd.call_expressions, rng.first, rng.second);
        }
    });

    BOOST_ASSERT(!md.call_expressions); // all arguments were transfered

    if (tmd.fields_count > 1) {
        u.push_back_expression(exprs, semantic::push_value{ tmd.property_index });
        u.push_back_expression(exprs, semantic::invoke_function(u.get(builtin_eid::array_at)));
    }

    ctx.context_type = md.result.entity_id();
    return std::move(exprs);
}

std::expected<entity_identifier, error_storage> tuple_get_pattern::const_apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    auto res = generic_apply(ctx, md);
    if (!res) return std::unexpected(std::move(res.error()));
    using result_t = std::expected<entity_identifier, error_storage>;
    return apply_visitor(make_functional_visitor<result_t>([&ctx, &md](auto && eid_or_el) -> result_t {
        if constexpr (std::is_same_v<std::decay_t<decltype(eid_or_el)>, entity_identifier>) {
            return eid_or_el;
        } else {
            return std::unexpected(make_shared<basic_general_error>(md.location, "can't evaluate as a const expression"sv));
        }
    }), *res);
}

}
