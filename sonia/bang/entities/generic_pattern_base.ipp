//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "generic_pattern_base.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

template <typename DerivedT>
struct generic_pattern_base<DerivedT>::arg_context_type
{
    fn_compiler_context& ctx;
    prepared_call const& call;
    annotated_identifier const* pargname;
    syntax_expression_t const& argexpr;

    unit& u() const { return ctx.u(); }

    error_storage make_argument_mismatch_error() const
    {
        return make_error<basic_general_error>(pargname ? pargname->location : get_start_location(argexpr), "argument mismatch"sv, argexpr);
    }
};

template <typename DerivedT>
std::expected<functional_match_descriptor_ptr, error_storage> generic_pattern_base<DerivedT>::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& exp) const
{
    return try_match(ctx, call, nullptr);
}

template <typename DerivedT>
template <typename CookieT>
std::expected<functional_match_descriptor_ptr, error_storage> generic_pattern_base<DerivedT>::try_match(fn_compiler_context& ctx, prepared_call const& call, CookieT&& cookie) const
{
    unit& u = ctx.u();
    shared_ptr<functional_match_descriptor> pmd;
    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        auto const& argexpr = arg.value();

        auto res = apply_visitor(base_expression_visitor{ ctx }, argexpr);
        if (!res) return std::unexpected(std::move(res.error()));
        arg_context_type argctx{ ctx, call, pargname, argexpr };
        auto err = apply_visitor(make_functional_visitor<error_storage>([this, &pmd, &argctx, &cookie](auto & v) -> error_storage {
            if constexpr (std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
                if (v == argctx.u().get(builtin_eid::void_)) {
                    return make_error<type_mismatch_error>(get_start_location(argctx.argexpr), v, "not void"sv);
                }
            } else {
                if (argctx.ctx.context_type == argctx.u().get(builtin_eid::void_)) return {}; // skip
            }
            return derived().accept_argument(std::forward<CookieT>(cookie), pmd, argctx, v);
        }), res->first);
        if (err) return std::unexpected(std::move(err));
    }
    if (!pmd) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter(s)"sv));
    }
    return pmd;
}

} // namespace sonia::lang::bang
