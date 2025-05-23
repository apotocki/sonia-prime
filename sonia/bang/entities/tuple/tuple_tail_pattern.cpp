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

#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

class tuple_tail_match_descriptor : public functional_match_descriptor
{
public:
    inline explicit tuple_tail_match_descriptor(entity_signature const& sig, bool is_typename, lex::resource_location loc) noexcept
        : functional_match_descriptor{ std::move(loc) }
        , arg_sig{ sig }
        , is_argument_typename{ is_typename }
        //result_sig{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) }
        //, result_actual_size{ 0 }
        //, is_src_head_const{ sig.fields().front().is_const() }
    {
        ////auto nids = sig.named_fields_indices();
        //auto pids = sig.positioned_fields_indices();
        //if (!pids.empty() && !pids.front()) pids = pids.subspan(1);
        //for (uint32_t i = 1; i < sig.fields().size(); ++i) {
        //    result_sig.push_back(sig.fields()[i]);
        //    bool is_positional = !pids.empty() && pids.front() == i;
        //    if (is_positional) {
        //        pids = pids.subspan(1);
        //    }
        //    
        //    if (!result_sig.fields().back().is_const()) {
        //        result_actual_size++;
        //    }
        //}

        //location = std::move(loc);
    }
    entity_signature const& arg_sig;
    bool is_argument_typename;
    //entity_signature result_sig;
    //size_t result_actual_size; // non-const fields count
    //bool is_src_head_const;
};

std::expected<functional_match_descriptor_ptr, error_storage> tuple_tail_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& exp) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);
    syntax_expression_t const* parg_expr;
    auto arg = call_session.use_next_positioned_argument(exp, false, &parg_expr);
    if (!arg) return std::unexpected(arg.error());
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    auto& ser = *arg;
    entity_identifier argtype;
    if (ser.is_const_result) {
        entity const& arg_entity = get_entity(u, ser.value());
        if (auto psig = arg_entity.signature(); psig && psig->name == u.get(builtin_qnid::tuple)) {
            // argument is typename tuple
            auto pmd = make_shared<tuple_tail_match_descriptor>(*psig, true, call.location);
            pmd->get_match_result(0).append_result(ser);
            pmd->void_spans = std::move(call_session.void_spans);
            return pmd;
        } else {
            argtype = arg_entity.get_type();
        }
    } else {
        argtype = ser.type();
    }
    entity const& tpl_entity = get_entity(u, argtype);
    entity_signature const* psig = tpl_entity.signature();
    if (!psig || psig->name != ctx.u().get(builtin_qnid::tuple)) {
        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*parg_expr), argtype, "a tuple"sv));
    }
    if (psig->fields().empty()) {
        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*parg_expr), argtype, "a not empty tuple"sv));
    }
    auto pmd = make_shared<tuple_tail_match_descriptor>(*tpl_entity.signature(), false, call.location);
    pmd->get_match_result(0).append_result(ser);
    pmd->void_spans = std::move(call_session.void_spans);
    //if (!ser.is_const_result) {
    //    if (pmd->result_actual_size) {
    //        pmd->get_match_result(0).append_result(argtype, ser.expressions);
    //    }
    //}
    return pmd;
    /*
    shared_ptr<tuple_tail_match_descriptor> pmd;
    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        auto const& argexpr = arg.value();

        auto res = apply_visitor(base_expression_visitor{ ctx, call.expressions }, argexpr);
        if (!res) return std::unexpected(std::move(res.error()));
        auto& ser = res->first;
        if (!pmd && !pargname) {
            entity_identifier argtype;
            if (ser.is_const_result) {
                entity const& arg_entity = get_entity(ctx.u(), ser.value());
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
                if (!ser.is_const_result) {
                    if (pmd->result_actual_size) {
                        pmd->get_match_result(0).append_result(argtype, ser.expressions);
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
    */
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

std::expected<syntax_expression_result_t, error_storage> tuple_tail_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& tmd = static_cast<tuple_tail_match_descriptor&>(md);

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
        return ser;
    }
    ser.value_or_type = u.make_empty_entity(result_tuple).id;
    ser.is_const_result = true;
    return ser;


    //    // return const tuple
    //    ser.value_or_type = result_tuple.id;
    //    BOOST_ASSERT(ser.is_const_result);
    //    return ser;
    //}
    //
    //if (tmd.result_actual_size == 0) {
    //    if (tplent.id == u.get(builtin_eid::void_)) {
    //        return syntax_expression_result_t{ .value_or_type = tplent.id, .is_const_result = true }; // return void
    //    }

    //    return syntax_expression_result_t{
    //        .expressions = md.merge_void_spans(el),
    //        .value_or_type = u.make_empty_entity(tplent).id,
    //        .is_const_result = true
    //    };
    //}
    //
    //semantic::expression_span exprs = md.merge_void_spans(el);
    //tmd.for_each_positional_match([&exprs, &el](parameter_match_result const& mr) {
    //    for (auto const& ser : mr.results) {
    //        exprs = el.concat(exprs, ser.expressions);
    //    }
    //});

    //if (!tmd.is_src_head_const) {
    //    u.push_back_expression(el, exprs, semantic::invoke_function(u.get(builtin_eid::array_tail)));
    //}
    //return syntax_expression_result_t{
    //    .expressions = std::move(exprs),
    //    .value_or_type = tplent.id,
    //    .is_const_result = false
    //};
}

}
