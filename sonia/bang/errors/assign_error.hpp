//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/errors.hpp"
#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class assign_error : public general_error
{
    location_t location_;
    syntax_expression_t lhs_;

public:
    assign_error(location_t loc, syntax_expression_t const& expr)
        : location_{ std::move(loc) }
        , lhs_{ expr }
    {}

    void visit(error_visitor& vis) const override { vis(*this); }
    location_t location() const noexcept override { return location_; }
    string_t object(unit const&) const noexcept override;
    string_t description(unit const&) const noexcept override;
};

}
