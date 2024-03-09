//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <list>
#include "beng.hpp"
#include "ast.hpp"
#include "semantic.hpp"

namespace sonia::lang::beng {

class function_entity : public entity
{
public:
    explicit function_entity(qname name) : entity{ std::move(name) } {}

    std::vector<semantic_expression_type> body;
};

class functional_entity : public entity
{
public:
    explicit functional_entity(qname name) : entity { std::move(name) } {}

    std::list<function_signature> signatures;

    function_signature& put_signature(function_signature&& sig);

    virtual bool find(compiler_context&,
        span<const expression_t> positioned_args,
        span<const std::tuple<annotated_identifier, expression_t>> named_args,
        std::vector<semantic_expression_type> & result, beng_generic_type & rtype) const;

    bool is_matched(compiler_context& ctx,
        function_signature const& sig,
        span<const expression_t> positioned_args,
        span<const std::tuple<annotated_identifier, expression_t>> named_args,
        std::vector<semantic_expression_type>& result) const;

   
};

}

#include "compiler.hpp"

namespace sonia::lang::beng {


function_signature& functional_entity::put_signature(function_signature&& one_more_sig)
{
    // to do: check for duplicated signature
    for (auto const& sig : signatures) {
        if (one_more_sig.position_parameters.size() != sig.position_parameters.size() ||
            one_more_sig.named_parameters.size() != sig.named_parameters.size()) continue;
        if (!sonia::range_equal{}(one_more_sig.position_parameters, sig.position_parameters)) continue;
        if (!sonia::range_equal{}(one_more_sig.named_parameters, sig.named_parameters)) continue;
        throw exception("A procedure with the same signature already exists.");
    }
    
    signatures.emplace_back(std::move(one_more_sig));
    return signatures.back();
}

bool functional_entity::is_matched(compiler_context& ctx, function_signature const& sig,
    span<const expression_t> positioned_args,
    span<const std::tuple<annotated_identifier, expression_t>> named_args,
    std::vector<semantic_expression_type>& result) const
{
    if (positioned_args.size() != sig.position_parameters.size() || named_args.size() != sig.named_parameters.size()) return false;
    for (beng_generic_type const& argt : sig.position_parameters) {
        expression_visitor evis{ ctx, result, &argt };
        if (!apply_visitor(evis, positioned_args.front())) return false;
        positioned_args = positioned_args.subspan(1);
    }
    for (auto const& [id, tp] : sig.named_parameters) {
        if (std::get<0>(named_args.front()).id != id) return false;
        expression_visitor evis{ ctx, result, &tp };
        if (!apply_visitor(evis, std::get<1>(named_args.front()))) return false;
        named_args = named_args.subspan(1);
    }
    return true;
}

bool functional_entity::find(compiler_context& ctx,
    span<const expression_t> positioned_args,
    span<const std::tuple<annotated_identifier, expression_t>> named_args,
    std::vector<semantic_expression_type>& result, beng_generic_type& rtype) const
{
    size_t initial_result_sz = result.size();
    EXCEPTIONAL_SCOPE_EXIT([&result, initial_result_sz]() { result.resize(initial_result_sz); });
    for (auto const& sig: signatures) {
        if (!is_matched(ctx, sig, positioned_args, named_args, result)) {
            result.resize(initial_result_sz);
            continue;
        }
        size_t argcount = positioned_args.size() + named_args.size();
        result.emplace_back(semantic::invoke_function{ sig.mangled_name, (uint32_t)argcount });
        rtype = sig.result_type;
        return true;
    }
    return false;
}

}
