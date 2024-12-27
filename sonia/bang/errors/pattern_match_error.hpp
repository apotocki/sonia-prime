//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/errors.hpp"
#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class pattern_match_error : public general_error
{
    functional::pattern const& pattern_;
    error_storage reason_;

public:
    pattern_match_error(functional::pattern const& pattern, error_storage reason)
        : pattern_{ pattern }
        , reason_{ reason }
    {
    }

    void visit(error_visitor& vis) const override { vis(*this); }
    lex::resource_location const& location() const noexcept override { return pattern_.location(); }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override;
};

}
