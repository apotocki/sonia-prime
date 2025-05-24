//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "assert_pattern.hpp"

#include "sonia/bang/functional/generic_pattern_base.ipp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

class assert_match_descriptor : public functional_match_descriptor
{
public:
    using functional_match_descriptor::functional_match_descriptor;

    small_vector<error_storage, 4> reserved_errors;
};

std::expected<functional_match_descriptor_ptr, error_storage> assert_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& exp) const
{
    auto call_session = call.new_session(ctx);
    annotated_entity_identifier bool_exp{ ctx.u().get(builtin_eid::boolean), call.location };
    syntax_expression_t const* pargexpr;
    auto firstarg = call_session.use_next_positioned_argument(bool_exp, false, &pargexpr);
    if (!firstarg) return std::unexpected(firstarg.error());
    auto pmd = make_shared<assert_match_descriptor>(call.location);
    size_t argnum = 0;

    auto append_arg = [&pmd, &argnum, pargexpr](unit& u, syntax_expression_result_t& res) {
        pmd->get_match_result(argnum++).append_result(res);
        if (res.is_const_result && res.value() == u.get(builtin_eid::true_)) {
            pmd->reserved_errors.emplace_back(); // just dummy, no error doesn't need details
        } else {
           // assert failed
            pmd->reserved_errors.emplace_back(
                make_error<basic_general_error>(get_start_location(*pargexpr), "Assertion failed!"sv, *pargexpr)
            );
        }
    };
    append_arg(ctx.u(), *firstarg);
    
    while (call_session.has_more_positioned_arguments()) {
        auto nextarg = call_session.use_next_positioned_argument(bool_exp, false, &pargexpr);
        if (!nextarg) return std::unexpected(firstarg.error());
        append_arg(ctx.u(), *nextarg);
    }
    
    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "Argument mismatch!"sv, std::move(argterm.value())));
    }
    
    pmd->void_spans = std::move(call_session.void_spans);
    return std::move(pmd);
}

std::expected<syntax_expression_result_t, error_storage> assert_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    assert_match_descriptor& amd = static_cast<assert_match_descriptor&>(md);
    alt_error errs;
    syntax_expression_result_t res{
        .expressions = md.merge_void_spans(el),
        .value_or_type = u.get(builtin_eid::void_),
        .is_const_result = true
    };
    
    for (size_t arg_i = 0; arg_i < amd.reserved_errors.size(); ++arg_i) {
        parameter_match_result& mr = amd.get_match_result(arg_i);
        auto arg_er = mr.results.front();
        BOOST_ASSERT(mr.results.size() == 1);
        if (arg_er.is_const_result) {
            if (arg_er.value() == u.get(builtin_eid::false_)) {
                // assert failed, we have error already
                errs.alternatives.emplace_back(std::move(amd.reserved_errors[arg_i]));
            }
        } else if (errs.alternatives.empty()) {
            res.temporaries.insert(res.temporaries.end(), arg_er.temporaries.begin(), arg_er.temporaries.end());
            res.expressions = el.concat(res.expressions, arg_er.expressions);
            std::ostringstream msgss;
            u.print_to(msgss, *amd.reserved_errors[arg_i]);
            u.push_back_expression(el, res.expressions, semantic::push_value{ small_string{ msgss.str() } });
            u.push_back_expression(el, res.expressions, semantic::invoke_function(u.get(builtin_eid::assert)));
        }
    }

    if (!errs.alternatives.empty()) {
        return std::unexpected(make_error<alt_error>(std::move(errs)));
    }
    return std::move(res);
}

}
