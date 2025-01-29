//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/errors.hpp"
#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class type_mismatch_error : public basic_general_error
{
    variant<entity_identifier, qname_identifier> expected_;

public:
    template <typename ExpectedArgT>
    type_mismatch_error(lex::resource_location loc, entity_identifier actual, ExpectedArgT && expected, lex::resource_location refloc = {})
        : basic_general_error{ std::move(loc), ""sv, actual, std::move(refloc) }
        , expected_{ std::forward<ExpectedArgT>(expected) }
    {
    }

    string_t description(unit const&) const noexcept override;
};

}
