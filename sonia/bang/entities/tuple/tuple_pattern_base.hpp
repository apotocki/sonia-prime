//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

// Base class for tuple_head_pattern and tuple_tail_pattern
class tuple_pattern_base : public functional::pattern
{
protected:
    // Base match descriptor for tuple patterns (head/tail)
    class tuple_pattern_match_descriptor : public functional_match_descriptor
    {
    public:
        inline tuple_pattern_match_descriptor(prepared_call const& call, entity_signature const& sig, bool is_typename) noexcept
            : functional_match_descriptor{ call }
            , arg_sig{ sig }
            , is_argument_typename{ is_typename }
        {}

        entity_signature const& arg_sig;
        bool is_argument_typename;
    };

public:
    tuple_pattern_base() = default;

    // Shared try_match logic for head/tail
    std::expected<functional_match_descriptor_ptr, error_storage>
    try_match_tuple(fn_compiler_context&, prepared_call const&, expected_result_t const&) const;
};

}