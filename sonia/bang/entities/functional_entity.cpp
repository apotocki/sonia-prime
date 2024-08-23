//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "functional_entity.hpp"

#include "../ast/fn_compiler_context.hpp"
#include "../ast/expression_visitor.hpp"

namespace sonia::lang::bang {

function_signature& functional_entity::put_signature(function_signature&& one_more_sig)
{
    // to do: check for duplicated signature
    for (auto const& sig : signatures) {
        if (one_more_sig.position_parameters().size() != sig.position_parameters().size() ||
            one_more_sig.named_parameters().size() != sig.named_parameters().size()) continue;
        if (!sonia::range_equal{}(one_more_sig.position_parameters(), sig.position_parameters())) continue;
        if (!std::ranges::equal(one_more_sig.named_parameters(), sig.named_parameters(), {},
            [](auto const& tpl) { return std::tuple{ std::get<0>(tpl).value, std::get<1>(tpl) }; },
            [](auto const& tpl) { return std::tuple{ std::get<0>(tpl).value, std::get<1>(tpl) }; }
        )) continue;
        throw exception("A procedure with the same signature already exists.");
    }
    
    signatures.emplace_back(std::move(one_more_sig));
    return signatures.back();
}

std::expected<function_signature const*, error_storage> functional_entity::find(fn_compiler_context& ctx, pure_call_t & call) const
{
    THROW_NOT_IMPLEMENTED_ERROR("functional_entity::find");
#if 0
    alt_error aerr;
    auto estate = ctx.expressions_state();
    for (auto & sig: signatures) {
        if (auto optres = is_matched(ctx, sig, call); optres) {
            aerr.alternatives.emplace_back(
                make_error<function_call_match_error>(annotated_qname_identifier{ name_, call.location() }, &sig, std::move(optres))
            );
            estate.restore();
            continue;
        }
        ctx.append_expression(semantic::invoke_function{ ctx.u().qnregistry().concat(name_, sig.mangled_id) });
        estate.detach();
        ctx.context_type = sig.fn_type.result;
        return &sig;
    }
    if (aerr.alternatives.size() == 1) {
        return std::unexpected(std::move(aerr.alternatives.front()));
    }
    return std::unexpected(make_error<alt_error>(std::move(aerr)));
#endif
}

function_signature const* functional_entity::find(fn_compiler_context& ctx,
    span<const bang_type> position_params,
    span<const std::tuple<annotated_identifier, bang_type>> named_params) const
{
    for (auto const& sig : signatures) {
        if (is_matched(ctx, sig, position_params, named_params)) {
            return &sig;
        }
    }
    return nullptr;
}

void function_entity::materialize_call(fn_compiler_context& ctx, pure_call_t& call) const
{
    auto estate = ctx.expressions_state();
    if (auto opterr = is_matched(ctx, signature_, call); opterr) {
        throw exception(ctx.u().print(function_call_match_error{ annotated_qname_identifier{ name(), call.location() }, &signature_, opterr }));
    }
    ctx.append_expression(semantic::invoke_function{ name() });
    estate.detach();
}

bool is_matched(fn_compiler_context& ctx,
    function_signature const& sig,
    span<const bang_type> positioned_params,
    span<const std::tuple<annotated_identifier, bang_type>> named_params)
{
    if (positioned_params.size() != sig.position_parameters().size() || named_params.size() != sig.named_parameters().size()) return false;
    for (bang_type const& param : sig.position_parameters()) {
        if (param != positioned_params.front()) return false;
        positioned_params = positioned_params.subspan(1);
    }
    for (auto const& [aname, tp] : sig.named_parameters()) {
        if (std::get<0>(named_params.front()) != aname) return false;

        if (std::get<1>(named_params.front()) != tp) return false;
        named_params = named_params.subspan(1);
    }
    return true;
}

error_storage is_matched(fn_compiler_context& ctx, function_signature const& sig, pure_call_t& call)
{
    THROW_NOT_IMPLEMENTED_ERROR("functional_entity::is_matched");
#if 0
    if (call.positioned_args.size() != sig.position_parameters().size() || call.named_args.size() != sig.named_parameters().size())
        return make_error<basic_general_error>(call.location(), "argument count mismatch"sv);
    auto positioned_args = std::span{ call.positioned_args };
    auto named_args = std::span{ call.named_args };
    for (bang_type const& argt : sig.position_parameters()) {
        expression_visitor evis{ ctx, expected_result_t{ argt, std::get<1>(positioned_args.front()) } };
        if (auto opterr = apply_visitor(evis, std::get<0>(positioned_args.front())); opterr) return std::move(opterr);
        positioned_args = positioned_args.subspan(1);
    }
    for (auto const& [aname, tp] : sig.named_parameters()) {
        if (std::get<0>(named_args.front()).value != qname{aname.value}) {
            // name mismatch
            return make_error<basic_general_error>(std::get<0>(named_args.front()).location, "argument name mismatch"sv);
        }
        expression_visitor evis{ ctx, expected_result_t{ tp, std::get<2>(named_args.front()) } };
        if (auto opterr = apply_visitor(evis, std::get<1>(named_args.front())); opterr) return std::move(opterr);
        named_args = named_args.subspan(1);
    }
    return {}; // matched
#endif
}

}
