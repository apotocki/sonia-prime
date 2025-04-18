//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_tail_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/expression_visitor.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

class tuple_tail_match_descriptor : public functional_match_descriptor
{
public:
    inline explicit tuple_tail_match_descriptor(unit& u, entity_signature const& sig, lex::resource_location loc) noexcept
        : functional_match_descriptor{ u }
        , result_sig{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) }
        , result_actual_size{ 0 }
        , is_src_head_const{ sig.fields().front().is_const() }
    {
        auto nids = sig.named_fields_indices();
        auto pids = sig.positioned_fields_indices();
        if (!pids.empty() && !pids.front()) pids = pids.subspan(1);
        for (uint32_t i = 1; i < sig.fields().size(); ++i) {
            bool is_positional = !pids.empty() && pids.front() == i;
            if (is_positional) {
                pids = pids.subspan(1);
                result_sig.push_back(sig.fields()[i]);
            } else {
                auto nit = std::ranges::find(nids, i, &std::pair<identifier, uint32_t>::second);
                BOOST_ASSERT(nit != nids.end());
                result_sig.push_back(nit->first, sig.fields()[i]);
            }
            if (!result_sig.fields().back().is_const()) {
                result_actual_size++;
            }
        }

        location = std::move(loc);
    }

    entity_signature result_sig;
    size_t result_actual_size; // non-const fields count
    bool is_src_head_const;
};

std::expected<functional_match_descriptor_ptr, error_storage> tuple_tail_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& exp) const
{
    unit& u = ctx.u();
    shared_ptr<tuple_tail_match_descriptor> pmd;
    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        auto const& argexpr = arg.value();

        auto res = apply_visitor(base_expression_visitor{ ctx }, argexpr);
        if (!res) return std::unexpected(std::move(res.error()));
        auto& [el, reid] = res->first;
        if (!pmd && !pargname) {
            entity_identifier argtype;
            if (!el) {
                entity const& arg_entity = get_entity(ctx.u(), reid);
                if (auto psig = arg_entity.signature(); psig && psig->name == ctx.u().get(builtin_qnid::tuple)) {
                    // argument is typename tuple
                    pmd = make_shared<tuple_tail_match_descriptor>(ctx.u(), *psig, call.location);
                    continue;
                } else {
                    argtype = arg_entity.get_type();
                }
            }
            entity const& tpl_entity = get_entity(ctx.u(), argtype);
            if (auto psig = tpl_entity.signature(); psig && psig->name == ctx.u().get(builtin_qnid::tuple)) {
                if (psig->fields().empty()) {
                    return std::unexpected(make_error<basic_general_error>(call.location, "tuple is empty"sv));
                }
                pmd = make_shared<tuple_tail_match_descriptor>(ctx.u(), *tpl_entity.signature(), call.location);
                if (el) {
                    if (pmd->result_actual_size) {
                        pmd->get_match_result(0).append_result(argtype, el.end(), el);
                        pmd->call_expressions.splice_back(el);
                    }
                }
                continue;
            }
            return std::unexpected(make_error<basic_general_error>(pargname ? pargname->location : get_start_location(argexpr), "argument mismatch"sv, argexpr));
        }
    }
    if (!pmd) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter"sv));
    }
    return pmd;
#if 0
    auto opt_arg_expr = try_match_single_unnamed(ctx, call);
    if (!opt_arg_expr) return std::unexpected(std::move(opt_arg_expr.error()));
    auto estate = ctx.expressions_state();

    auto pmd = make_shared<tuple_tail_match_descriptor>(u);
    ctx.push_chain(pmd->call_expressions);
    auto last_expr_it = ctx.expressions().last();

    expression_visitor evis{ ctx };
    auto res = apply_visitor(evis, **opt_arg_expr);
    if (!res) return std::unexpected(std::move(res.error()));
    if (ctx.context_type) {
        entity const& tpl_entity = u.eregistry_get(ctx.context_type);
        if (auto psig = tpl_entity.signature(); psig && psig->name == u.get(builtin_qnid::tuple)) {
            if (psig->fields().empty()) {
                return std::unexpected(make_error<basic_general_error>(call.location(), "tuple is empty"sv));
            }
        }

        //entity_signature const& src_sig = *tpl_entity.signature();
        pmd = make_shared<tuple_tail_match_descriptor>(u, *tpl_entity.signature(), call.location());
        if (pmd->result_actual_size) {
            pmd->get_match_result(0).append_result(ctx.context_type, last_expr_it, ctx.expressions());
        }

        pmd->is_src_head_const = src_sig.fields().front().is_const();
        pmd->result_actual_size = 0;
        auto nids = src_sig.named_fields_indices();
        auto pids = src_sig.positioned_fields_indices();
        if (!pids.empty() && !pids.front()) pids = pids.subspan(1);
        for (uint32_t i = 1; i < src_sig.fields().size(); ++i) {
            bool is_positional = !pids.empty() && pids.front() == i;
            if (is_positional) {
                pids = pids.subspan(1);
                pmd->result_sig.push_back(src_sig.fields()[i]);
            } else {
                auto nit = std::ranges::find(nids, i, &std::pair<identifier, uint32_t>::second);
                BOOST_ASSERT(nit != nids.end());
                pmd->result_sig.push_back(nit->first, src_sig.fields()[i]);
            }
            if (!pmd->result_sig.fields().back().is_const()) {
                pmd->result_actual_size++;
            }
        }
        if (pmd->result_actual_size) {
            pmd->get_match_result(0).append_result(ctx.context_type, last_expr_it, ctx.expressions());
        }
        pmd->location = call.location();
        return pmd;
    }
    return std::unexpected(make_error<type_mismatch_error>(get_start_location(**opt_arg_expr), ctx.context_type, u.get(builtin_qnid::tuple)));
#endif
}

std::expected<syntax_expression_result_t, error_storage> tuple_tail_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& tmd = static_cast<tuple_tail_match_descriptor&>(md);

    indirect_signatured_entity smpl{ tmd.result_sig };
    entity& tplent = ctx.u().eregistry_find_or_create(smpl, [&u, &tmd]() {
        return make_shared<basic_signatured_entity>(std::move(tmd.result_sig));
    });
    
    if (tmd.result_actual_size == 0) {
        if (tplent.id() == u.get(builtin_eid::void_)) {
            return syntax_expression_result_t{ semantic::managed_expression_list{ u }, tplent.id() }; // return void
        }
        empty_entity valueref{ tplent.id() };
        entity& value_ent = ctx.u().eregistry_find_or_create(valueref, [&tplent]() {
            return make_shared<empty_entity>(tplent.id());
        });
        return make_result(u, value_ent.id());
    }
    
    semantic::managed_expression_list exprs{ u };
    tmd.for_each_positional_match([&exprs, &tmd](parameter_match_result const& mr) {
        for (auto const& [eid, optspan] : mr.results) {
            BOOST_ASSERT(optspan);
            exprs.splice_back(tmd.call_expressions, *optspan);
        }
    });

    BOOST_ASSERT(!tmd.call_expressions); // all arguments were transfered

    if (!tmd.is_src_head_const) {
        u.push_back_expression(exprs, semantic::invoke_function(u.get(builtin_eid::array_tail)));
    }
    return syntax_expression_result_t{ std::move(exprs), tplent.id() };
}

}
