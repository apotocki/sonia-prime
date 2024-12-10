//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <expected>

#include "sonia/bang/unit.hpp"
#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

struct ct_expression_visitor : static_visitor<std::expected<entity_identifier, error_storage>>
{
    fn_compiler_context& ctx;

    explicit ct_expression_visitor(fn_compiler_context& c)
        : ctx{ c }
    {}

    result_type operator()(annotated_entity_identifier const& ee) const;


    template <typename T>
    result_type operator()(T const& v) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("ct_expression_visitor not implemented expression");
    }

private:
    unit& u() const noexcept;
};

}
