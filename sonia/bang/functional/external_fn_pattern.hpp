//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "basic_fn_pattern.hpp"

namespace sonia::lang::bang {

class external_function_entity;

class external_fn_pattern : public basic_fn_pattern
{
    size_t extfnid_;
    shared_ptr<external_function_entity> ent_;

public:
    inline explicit external_fn_pattern(size_t extfnid) noexcept
        : extfnid_{ extfnid }
    {}

    void set_result_entity(shared_ptr<external_function_entity> e) { ent_ = std::move(e); }
        
    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, prepared_call const&, expected_result_t const&) const override;

    std::expected<syntax_expression_result_t, error_storage> apply(fn_compiler_context&, semantic::expression_list_t&, functional_match_descriptor&) const override;
};

}
