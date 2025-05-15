//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"
//#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

class signature_matcher_visitor : public static_visitor<error_storage>
{
    fn_compiler_context & ctx_;
    functional_binding& binding_;
    entity_identifier eid_;

public:
    signature_matcher_visitor(fn_compiler_context&, functional_binding&, entity_identifier);

    result_type operator()(variable_reference const&) const;
    //result_type operator()(function_call_t const& sp) const;

    // returns true if implicit cast was applied
    //std::expected<bool, error_storage> try_match(fn_compiler_context&, syntax_expression_t const&) const;

    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("signature_matcher_visitor not implemented expression");
    }
};

}
