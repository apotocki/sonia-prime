//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <utility>
#include "sonia/variant.hpp"
#include "sonia/small_vector.hpp"
#include "sonia/bang/ast_terms.hpp"

#include "basic_fn_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"

namespace sonia::lang::bang {

class parameter_matcher
{
    using parameter_iteator = basic_fn_pattern::parameters_t::const_iterator;
    parameter_iteator param_bit, param_it, param_end;
    prepared_call const& call;
    prepared_call::session call_session;
    functional_match_descriptor& md;

public:
    parameter_matcher(fn_compiler_context& caller_ctx, prepared_call const&, basic_fn_pattern::parameters_t const&, functional_match_descriptor&);
    error_storage match(fn_compiler_context& callee_ctx);

protected:
    std::expected<expected_result_t, error_storage> resolve_expression_expected_result(fn_compiler_context& callee_ctx, annotated_identifier const&, parameter_constraint_modifier_t, syntax_expression_t const&, entity_identifier& pconstraint_value_eid);
    error_storage handle_positioned_ellipsis(fn_compiler_context& callee_ctx, expected_result_t argexp);

    void finalize_ellipsis(unit&, span<std::pair<annotated_identifier, syntax_expression_result_t>> ellipsis_span);
};

#if 0
class parameter_matcher
{
public:
    explicit parameter_matcher(annotated_identifier name) noexcept
        : internal_names_{ name }
    {}

    inline void push_internal_name(annotated_identifier id) { internal_names_.push_back(std::move(id)); }

protected:
    small_vector<annotated_identifier, 2> internal_names_;
    variant<std::pair<syntax_expression_t, bool>, pattern_t> constraint_;
};

class named_parameter_matcher : public parameter_matcher
{
public:
    inline named_parameter_matcher(annotated_identifier enm, annotated_identifier const* inm)
        : parameter_matcher{ inm ? *inm : enm }
        , external_name_{ enm }
    {}

    inline annotated_identifier const& external_name() const noexcept { return external_name_; }

private:
    annotated_identifier external_name_;
};

struct named_parameter_matcher_less
{
    using is_transparent = void*;

    inline bool operator() (named_parameter_matcher const& l, named_parameter_matcher const& r) const noexcept
    {
        return l.external_name().value < r.external_name().value;
    }

    inline bool operator() (named_parameter_matcher const& l, identifier const& r) const noexcept
    {
        return l.external_name().value < r;
    }

    inline bool operator() (identifier const& l, named_parameter_matcher const& r) const noexcept
    {
        return  l < r.external_name().value;
    }
};
#endif
}
