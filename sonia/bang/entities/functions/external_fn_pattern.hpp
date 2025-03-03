//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/shared_ptr.hpp"
#include "basic_fn_pattern.hpp"

namespace sonia::lang::bang {

class external_fn_pattern : public runtime_fn_pattern
{
    size_t extfnid_;

public:
    external_fn_pattern(functional const& fnl, size_t extfnid)
        : runtime_fn_pattern{ fnl }
        , extfnid_{ extfnid }
    {}


    //fieldset_t const& get_fieldset() const noexcept override { return fd_; }
    
    std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context&, functional_match_descriptor&) const override;
    //std::expected<entity_identifier, error_storage> apply(fn_compiler_context&, functional::match_descriptor&) const override;

protected:
    shared_ptr<entity> build(fn_compiler_context&, functional_match_descriptor&, entity_signature&&) const;
};

}
