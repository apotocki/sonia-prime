//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "functional.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/ellipsis/pack_entity.hpp"

namespace sonia::lang::bang {

std::expected<functional::pattern const*, error_storage> functional::find(fn_compiler_context& ctx, pure_call_t& call) const
{
    alt_error err;
    mp::decimal priority = 0;
    using alternative_t = std::pair<pattern const*, fn_compiler_context::expr_vec_t>;
    boost::container::small_vector<alternative_t, 2> alternatives;
    auto estate = ctx.expressions_state();
    fn_compiler_context::expr_vec_t branch;

    for (auto const& p : patterns_) {
        auto cmp = priority <=> p->get_weight();
        if (cmp == std::strong_ordering::greater) continue;
        ctx.push_chain(branch);
        if (auto res = p->is_matched(ctx, call); res) {
            if (alternatives.empty()) { // no need to store errors if a match exists
                //err.alternatives.emplace_back(
                //    make_error<function_call_match_error>(annotated_qname_identifier{ id_, call.location() }, sig, std::move(optres))
                //);
                err.alternatives.emplace_back(std::move(res));
            }
        } else {
            if (cmp == std::strong_ordering::less) {
                priority = p->get_weight();
                alternatives.clear();
            }
            alternatives.emplace_back(p.get(), std::move(branch));
        }
        estate.restore();
        branch.clear();
    }

    if (alternatives.empty()) {
        if (err.alternatives.size() == 1) {
            return std::unexpected(std::move(err.alternatives.front()));
        }
        if (err.alternatives.empty()) {
            return std::unexpected(make_error<function_call_match_error>(annotated_qname_identifier{ id_, call.location() }, nullptr, nullptr));
        }
        return std::unexpected(make_error<alt_error>(std::move(err)));
    }
    if (alternatives.size() > 1) {
        return std::unexpected(make_error<ambiguity_error>());
    }
    estate.detach();
    ctx.append_expression(std::move(alternatives.front().second));
    fn_compiler_context::expr_vec_t& args = get<fn_compiler_context::expr_vec_t>(ctx.expressions().back());
    ctx.push_chain(args);
    return &*alternatives.front().first;
}

error_storage fn_pattern::is_matched(fn_compiler_context& ctx, pure_call_t& call) const
{
    for (auto const& tpl : call.named_args) { // {argname, expr, exprloc}
        THROW_NOT_IMPLEMENTED_ERROR("fn_pattern::is_matched, named argument");
    }
    auto psp = fn_.positioned_fields();
    auto posarg = psp.begin();
    for (auto tplit = call.positioned_args.begin(), tpleit = call.positioned_args.end(); tplit != tpleit;) { // {expr, exprloc}
        //auto const& tpl = *tplit;
        if (posarg == psp.end()) {
            return make_error<basic_general_error>(std::get<1>(*tplit), "positioned argument mismatch"sv);
        }
        entity_identifier param_type = posarg->constraint;
        entity const& param_entity = ctx.u().eregistry().get(param_type);
        if (pack_entity const* pent = dynamic_cast<pack_entity const*>(&param_entity); pent) {
            uint64_t pack_size = 0;
            //ctx.append_expression(semantic::push_value{ uint64_t{pack_size} });
            //auto pack_size_expr_pointer = ctx.current_expressions_pointer();
            for (;;) {
                expression_visitor evis{ ctx, expected_result_t{ pent->element_type(), std::get<1>(*tplit) } };
                if (auto opterr = apply_visitor(evis, std::get<0>(*tplit)); opterr) {
                    ++posarg;
                    if (posarg == psp.end()) return std::move(opterr);
                    break;
                }
                ++pack_size;
                if (++tplit == tpleit) {
                    ++posarg;
                    break;
                }
            }
            ctx.append_expression(semantic::push_value{ uint64_t{pack_size} });
            //ctx.set_expression(pack_size_expr_pointer, semantic::push_value{ uint64_t{pack_size} });
        } else {
            expression_visitor evis{ ctx, expected_result_t{ param_type, std::get<1>(*tplit) } };
            if (auto opterr = apply_visitor(evis, std::get<0>(*tplit)); opterr) return std::move(opterr);
            ++tplit;
            ++posarg;
        }
    }
    while (posarg != psp.end()) {
        entity_identifier param_type = posarg->constraint;
        entity const& param_entity = ctx.u().eregistry().get(param_type);
        if (pack_entity const* pent = dynamic_cast<pack_entity const*>(&param_entity); pent) {
            ctx.append_expression(semantic::push_value{ uint64_t{0} }); // set empty argument pack
            ++posarg;
            continue;
        }
        return make_error<basic_general_error>(call.location(), "positioned argument mismatch"sv);
    }
    // to do: replace expressions according signature
    return {};
}

std::expected<entity_identifier, error_storage> functional::pattern::apply(fn_compiler_context& ctx) const
{
    auto r = const_apply(ctx);
    if (!r.has_value()) return std::unexpected(std::move(r.error()));
    ctx.append_expression(semantic::push_value{r.value()});
    entity const& e = ctx.u().eregistry().get(r.value());
    return e.get_type();
}

std::expected<entity_identifier, error_storage> fn_pattern::const_apply(fn_compiler_context& ctx) const
{
    // , std::span<semantic::expression_type> args
    THROW_NOT_IMPLEMENTED_ERROR("fn_pattern::const_apply");
}


std::expected<entity_identifier, error_storage> external_fn_pattern::const_apply(fn_compiler_context& ctx) const
{
    THROW_NOT_IMPLEMENTED_ERROR("external_fn_pattern::const_apply");
}

std::expected<entity_identifier, error_storage> external_fn_pattern::apply(fn_compiler_context& ctx) const
{
    ctx.append_expression(semantic::invoke_external_function{ extfnid_ });
    ctx.pop_chain(); // function call chain
    return fn_.result_type();
}

}
