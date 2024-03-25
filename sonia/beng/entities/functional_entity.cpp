//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "functional_entity.hpp"

#include "sonia/utility/scope_exit.hpp"

#include "../ast/expression_visitor.hpp"

namespace sonia::lang::beng {

function_signature& functional_entity::put_signature(function_signature&& one_more_sig)
{
    // to do: check for duplicated signature
    for (auto const& sig : signatures) {
        if (one_more_sig.position_parameters().size() != sig.position_parameters().size() ||
            one_more_sig.named_parameters().size() != sig.named_parameters().size()) continue;
        if (!sonia::range_equal{}(one_more_sig.position_parameters(), sig.position_parameters())) continue;
        if (!std::ranges::equal(one_more_sig.named_parameters(), sig.named_parameters(), {},
            [](auto const& tpl) { return std::tuple{ std::get<0>(tpl).id, std::get<1>(tpl) }; },
            [](auto const& tpl) { return std::tuple{ std::get<0>(tpl).id, std::get<1>(tpl) }; }
        )) continue;
        throw exception("A procedure with the same signature already exists.");
    }
    
    signatures.emplace_back(std::move(one_more_sig));
    return signatures.back();
}

bool functional_entity::is_matched(fn_compiler_context& ctx,
    function_signature const& sig,
    span<const beng_type> positioned_params,
    span<const std::tuple<annotated_identifier, beng_type>> named_params) const
{
    if (positioned_params.size() != sig.position_parameters().size() || named_params.size() != sig.named_parameters().size()) return false;
    for (beng_type const& param : sig.position_parameters()) {
        if (param != positioned_params.front()) return false;
        positioned_params = positioned_params.subspan(1);
    }
    for (auto const& [aname, tp] : sig.named_parameters()) {
        if (std::get<0>(named_params.front()).id != aname.id) return false;

        if (std::get<1>(named_params.front()) != tp) return false;
        named_params = named_params.subspan(1);
    }
    return true;
}

bool functional_entity::is_matched(fn_compiler_context& ctx, function_signature const& sig,
    span<const expression_t> positioned_args,
    span<const std::tuple<annotated_identifier, expression_t>> named_args,
    std::vector<semantic_expression_type>& result) const
{
    if (positioned_args.size() != sig.position_parameters().size() || named_args.size() != sig.named_parameters().size()) return false;
    for (beng_type const& argt : sig.position_parameters()) {
        expression_visitor evis{ ctx, result, &argt };
        if (!apply_visitor(evis, positioned_args.front())) return false;
        positioned_args = positioned_args.subspan(1);
    }
    for (auto const& [aname, tp] : sig.named_parameters()) {
        if (std::get<0>(named_args.front()).id != aname.id) return false;
        expression_visitor evis{ ctx, result, &tp };
        if (!apply_visitor(evis, std::get<1>(named_args.front()))) return false;
        named_args = named_args.subspan(1);
    }
    return true;
}

bool functional_entity::find(fn_compiler_context& ctx,
    span<const expression_t> positioned_args,
    span<const std::tuple<annotated_identifier, expression_t>> named_args,
    std::vector<semantic_expression_type>& result, beng_type& rtype) const
{
    size_t initial_result_sz = result.size();
    EXCEPTIONAL_SCOPE_EXIT([&result, initial_result_sz]() { result.resize(initial_result_sz); });
    for (auto const& sig: signatures) {
        if (!is_matched(ctx, sig, positioned_args, named_args, result)) {
            result.resize(initial_result_sz);
            continue;
        }
        result.emplace_back(semantic::invoke_function{ name() + sig.mangled_id });
        rtype = sig.fn_type.result;
        return true;
    }
    return false;
}

function_signature const* functional_entity::find(fn_compiler_context& ctx,
    span<const beng_type> position_params,
    span<const std::tuple<annotated_identifier, beng_type>> named_params) const
{
    for (auto const& sig : signatures) {
        if (is_matched(ctx, sig, position_params, named_params)) {
            return &sig;
        }
    }
    return nullptr;
}

}