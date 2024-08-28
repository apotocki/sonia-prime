//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/shared_ptr.hpp"
#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class external_fn_pattern : public fieldset_pattern
{
    size_t extfnid_;
    function_descriptor fd_;

public:
    external_fn_pattern(function_descriptor&& fn, size_t extfnid)
        : fd_{ std::move(fn) }, extfnid_{ extfnid }
    {}

    fieldset const& get_fieldset() const noexcept override { return fd_; }

    std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context& ctx) const override;
    std::expected<entity_identifier, error_storage> apply(fn_compiler_context& ctx) const override;
};

}
