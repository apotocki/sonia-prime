//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_size_pattern.hpp"

#include "sonia/bang/functional/generic_pattern_base.ipp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> tuple_size_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);
    syntax_expression_t const* parg_expr;
    auto arg = call_session.use_next_positioned_argument(&parg_expr);
    if (!arg) return std::unexpected(arg.error());
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }
    syntax_expression_result_t& er = *arg;
    entity_identifier argtype;
    shared_ptr<functional_match_descriptor> pmd;
    if (er.is_const_result) {
        entity const& arg_entity = get_entity(u, er.value());
        if (auto psig = arg_entity.signature(); psig && psig->name == u.get(builtin_qnid::tuple)) {
            // argument is typename tuple
            pmd = make_shared<functional_match_descriptor>(call.location);
            pmd->result = field_descriptor{ ctx.u().make_integer_entity(psig->fields().size()).id, true };
        } else {
            argtype = arg_entity.get_type();
        }
    } else {
        argtype = er.type();
    }

    if (!pmd) {
        entity const& tpl_entity = get_entity(u, argtype);
        entity_signature const* psig = tpl_entity.signature();
        if (!psig || psig->name != u.get(builtin_qnid::tuple)) {
            return std::unexpected(make_error<type_mismatch_error>(get_start_location(*parg_expr), argtype, "a tuple"sv));
        }
        pmd = make_shared<functional_match_descriptor>(call.location);
        pmd->result = field_descriptor{ ctx.u().make_integer_entity(psig->fields().size()).id, true };
    }
    pmd->get_match_result(0).append_result(er);
    pmd->void_spans = std::move(call_session.void_spans);
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> tuple_size_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    syntax_expression_result_t & arg = md.get_match_result(0).results.front();

    return syntax_expression_result_t{
        .temporaries = std::move(arg.temporaries),
        .stored_expressions = std::move(arg.stored_expressions),
        .expressions = md.merge_void_spans(el),
        .value_or_type = md.result.entity_id(),
        .is_const_result = true
    };
}

}
