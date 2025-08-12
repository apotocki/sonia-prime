//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

template <typename DerivedT>
class generic_get_pattern_matcher
{
    inline DerivedT & derived() noexcept { return static_cast<DerivedT&>(*this); }

protected:
    fn_compiler_context& ctx;
    prepared_call const& call;
    expected_result_t const& exp;

public:
    inline generic_get_pattern_matcher(
        fn_compiler_context& ctx_val,
        prepared_call const& call_val,
        expected_result_t const& exp_val) noexcept
        : ctx{ ctx_val }, call{ call_val }, exp{ exp_val }
    {}

    std::expected<functional_match_descriptor_ptr, error_storage> try_match();
    
    inline error_storage check_type_compatibility() const { return {}; }
    
    prepared_call::argument_descriptor_t slf_arg_descr;
    prepared_call::argument_descriptor_t property_arg_descr;
    entity_identifier slftype;
};

template <typename DerivedT>
std::expected<functional_match_descriptor_ptr, error_storage> generic_get_pattern_matcher<DerivedT>::try_match()
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);

    auto slf_arg = call_session.use_named_argument(u.get(builtin_id::self), expected_result_t{}, &slf_arg_descr);
    if (!slf_arg) {
        if (!slf_arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument: `self`"sv));
        }
        return std::unexpected(std::move(slf_arg.error()));
    }

    syntax_expression_result_t& slf_arg_er = slf_arg->first;
    if (slf_arg_er.is_const_result) {
        entity const& slf_entity = get_entity(u, slf_arg_er.value());
        slftype = slf_entity.get_type();
    } else {
        slftype = slf_arg_er.type();
    }
    if (auto err = derived().check_type_compatibility(); err) {
        return std::unexpected(std::move(err));
    }

    alt_error prop_errors;
    auto property_arg = call_session.use_named_argument(u.get(builtin_id::property), expected_result_t{ u.get(builtin_eid::integer) }, &property_arg_descr);
    if (!property_arg && property_arg.error()) {
        prop_errors.alternatives.emplace_back(std::move(property_arg.error()));
        call_session.reuse_argument(get<1>(property_arg_descr));
        property_arg = call_session.use_named_argument(u.get(builtin_id::property), expected_result_t{ u.get(builtin_eid::identifier) }, &property_arg_descr);
    }
    if (!property_arg) {
        if (!property_arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument: `property`"sv));
        }
        if (prop_errors.alternatives.empty()) {
            return std::unexpected(std::move(property_arg.error()));
        } else {
            prop_errors.alternatives.emplace_back(std::move(property_arg.error()));
            return std::unexpected(make_error<alt_error>(std::move(prop_errors)));
        }
    }

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    auto optpmd = derived().create_match_descriptor();
    if (optpmd) {
        functional_match_descriptor& md = **optpmd;
        md.emplace_back(0, slf_arg_er);
        md.emplace_back(1, property_arg->first);
    }
    return std::move(optpmd);
}

}
