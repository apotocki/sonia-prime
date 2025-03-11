//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

template <typename DerivedT>
class generic_pattern_base : public functional::pattern
{
public:
    generic_pattern_base() = default;

    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, pure_call_t const&, annotated_entity_identifier const&) const override;

protected:
    struct arg_context_type;

    template <typename CookieT>
    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, pure_call_t const&, CookieT&&) const;

    DerivedT const& derived() const { return static_cast<DerivedT const&>(*this); }
};

}
